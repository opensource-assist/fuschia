# Inspect file format

[TOC]

This document describes the **Component Inspection File Format** (Inspect Format).

Files formatted using the Inspect Format are known as **Inspect Files**,
which commonly have a `.inspect` file extension.

For information on how to change the format. Please see
[Extending the Inspect file format](updating.md).

# Overview

[Component Inspection][inspect] provides components with the ability to
expose structured, hierarchical information about their state at runtime.

Components host a mapped Virtual Memory Object ([VMO]) using the Inspect
Format to expose an **Inspect Hierarchy** containing this internal state.

An Inspect Hierarchy consists of nested **Nodes** containing typed **Properties**.

## Goals

The Inspect Format described in this document has the following goals:

- **Low-overhead mutations to data**

The Inspect File Format allows data to be changed in-place. For instance,
the overhead of incrementing an integer is ~2 atomic increments.

- **Support a non-static hierarchy**

The hierarchy stored in an Inspect File can be modified at
runtime. Children can be added or removed from the hierarchy at any
time. In this way, the hierarchy can closely represent the hierarchy of
objects in the component's working set.

- **Single writer, multiple reader concurrency without explicit synchronization**

Readers operating concurrently with the writer map the VMO and attempt to
take a snapshot of the data. Writers indicate being in a critical section
though a *generation counter* that requires no explicit synchronization
with readers. Readers use the generation counter to determine when a
snapshot of the VMO is consistent and may be safely read.

- **Data may remain available after component termination**

A reader may maintain a handle to the VMO containing Inspect data even
after the writing component terminates.

[inspect]: /docs/development/inspect/README.md

## Terminology

This section defines common terminology used in this document.

* Inspect File - A bounded sequence of bytes using the format described in this document.
* Inspect VMO - An Inspect File stored in a Virtual Memory Object (VMO).
* Block - A sized section of an Inspect File. Blocks have an Index and an Order.
* Index - A unique identifier for a particular Block. `byte_offset = index * 16`
* Order - The size of a block given as a bit shift from the minimum
          size. `size_in_bytes = 16 << order`. Separates blocks into
          classes by their (power of two) size.
* Node  - A named value in the hierarchy under which other values may
          be nested. Only Nodes may be parents in the Hierarchy.
* Property - A named value that contains typed data (e.g. String,
             Integer, etc).
* Hierarchy - A tree of Nodes, descending from a single "root" node, that
              may each contain Properties. An Inspect File contains a
              single Hierarchy.

This document uses MUST, SHOULD/RECOMMENDED, and MAY keywords as defined in [RFC 2119][rfc2119]

All bit field diagrams are stored in little-endian ordering.

[rfc2119]: https://www.ietf.org/rfc/rfc2119.txt

# Blocks

Inspect files are split into a number of `Blocks` whose size must be a
power of 2.

The minimum block size must be 16 bytes (`MIN_BLOCK_SIZE`) and the
maximum block size must be a multiple of 16 bytes. Implementers are
recommended specify a maximum block size less than the size of a page
(typically 4096 bytes). In our reference implementation, the maximum
block size is 2048 bytes (`MAX_BLOCK_SIZE`).

All blocks must be aligned on 16-byte boundaries, and addressing within
the VMO is in terms of an Index, specifying a 16-byte offsets (`offset =
index * 16`).

We use 28 bits for indexes, so Inspect Files may be at most 4GiB.

A `block_header` consists of 16 bytes as follows:

![Figure: Block header organization](blockheader.png)

Each block has an `order`, specifying its size.

If the maximum block size is 2048 bytes, then there are 8 possible block
orders (`NUM_ORDERS`), numbered 0...7, corresponding to blocks of sizes
16, 32, 64, 128, 256, 512, 1024, and 2048 bytes respectively.

Each block also has a type, which is used to determine how the rest of
the bytes in the block are to be interpreted.

## Buddy Allocation

This block layout permits efficient allocation of blocks using [buddy
allocation][buddy]. Buddy allocation is the recommended allocation
strategy, but it is not a requirement for using the Inspect Format.

# Types

All the supported types are defined in
[//zircon/system/ulib/inspect/include/lib/inspect/cpp/vmo/block.h][block.h]
which fall into categories as follows:

enum             | value | type name | category
-----------------|-------|----------------|-------
`kFree`          | 0     | `FREE`             | Internal
`kReserved`      | 1     | `RESERVED`         | Internal
`kHeader`        | 2     | `HEADER`           | Header
`kNodeValue`     | 3     | `NODE_VALUE`       | Value
`kIntValue`      | 4     | `INT_VALUE`        | Value
`kUintValue`     | 5     | `UINT_VALUE`       | Value
`kDoubleValue`   | 6     | `DOUBLE_VALUE`     | Value
`kPropertyValue` | 7     | `PROPERTY_VALUE`   | Value
`kExtent`        | 8     | `EXTENT`           | Extent
`kName`          | 9     | `NAME`             | Name
`kTombstone`     | 10    | `TOMBSTONE`        | Value
`kArrayValue`    | 11    | `ARRAY_VALUE`      | Value
`kLinkValue`     | 12    | `LINK_VALUE`       | Value

* *Internal* - These types are provided for implementing block allocation, and
they must be ignored by readers.

* *Header* - This type allows readers to detect Inspect Files and reason
about snapshot consistency. This block must exist at index 0.

* *Value* - These types appear directly in the hierarchy. Values must have a *Name*
and a parent (which must be a `NODE_VALUE`).

* *Extent* - This type stores long binary data that may not fit in a single block.

* *Name* - This type stores binary data that fits in a single block,
and it is typically used to store the name of values.

Each type interprets the payload differently, as follows:

* [FREE](#free)
* [RESERVED](#reserved)
* [HEADER](#header)
* [Common VALUE fields](#value)
* [NODE\_VALUE](#node)
* [INT\_VALUE](#numeric)
* [UINT\_VALUE](#numeric)
* [DOUBLE\_VALUE](#numeric)
* [PROPERTY\_VALUE](#property)
* [EXTENT](#extent)
* [NAME](#name)
* [TOMBSTONE](#node)
* [ARRAY\_VALUE](#array)
* [LINK](#link)

## FREE {#free}

![Figure: Free block](freeblock.png)

A `FREE` block is available for allocation. Importantly, the zero-valued
block (16 bytes of `\0`) is interpreted as a `FREE` block of order 0,
so buffers may simply be zeroed to free all blocks.

Writer implementations may use the unused bits from 8..63 of `FREE`
blocks for any purpose. Writer implementation must set all other unused
bits to 0.

It is recommended that writers use the location specified above to store
the index of the next free block of the same order. Using this field,
free blocks may create singly linked lists of free blocks of each size
for fast allocation. The end of the list is reached when NextFreeBlock
points to a location that is either not `FREE` or not of the same order
(commonly the Header block at index 0).

## RESERVED {#reserved}

![Figure: Reserved block](reservedblock.png)

`RESERVED` blocks are simply available to be changed to a different
type.  It is an optional transitional state between the allocation of a
block and setting its type that is useful for correctness checking of
implementations (to ensure that blocks that are about to be used are
not treated as free).

## HEADER {#header}

![Figure: Header block](headerblock.png)

There must be one `HEADER` block at the beginning of the file. It consists
of a **Magic Number** ("INSP"), a **Version** (currently 0), and the
**Generation Count** for concurrency control. The first byte of the header
must not be a valid ASCII character.

See the [next section](#concurrency) for how concurrency control must be
implemented using the generation count.

## \*\_VALUE {#value}

![Figure: general value block](generalvalue.png)

Values all start with the same prefix, consisting of the index of the
parent for the value and the index of the name associated with the value.

The payload is interpreted differently depending on the type of value, as
below.

## NODE\_VALUE and TOMBSTONE {#node}

![Figure: Node and Tombstone blocks](objtombblock.png)

Nodes are anchor points for further nesting, and the `ParentID` field
of values must only refer to blocks of type `NODE_VALUE`.

`NODE_VALUE` blocks support optional *reference counting* and *tombstoning*
to permit efficient implementations as follows:

The `Refcount` field may contain the number of values referencing a given
`NODE_VALUE` as their parent. On deletion, the `NODE_VALUE` becomes a new
special type called `TOMBSTONE`. `TOMBSTONE`s are deleted only when their
`Refcount` is 0.

This allows for writer implementations that do not need to explicitly
keep track of children for Nodes, and it prevents the following scenario:

```
// "b" has a parent "a"
Index | Value
0     | HEADER
1     | NODE "a", parent 0
2     | NODE "b", parent 1

/* delete "a", allocate "c" as a child of "b" which reuses index 1 */

// "b"'s parent is now suddenly "c", introducing a cycle!
Index | Value
0     | HEADER
1     | NODE "c", parent 2
2     | NODE "b", parent 1
```

## \{INT,UINT,DOUBLE\}\_VALUE {#numeric}

![Figure: Numeric type block](numericblock.png)

Numeric `VALUE` blocks all contain the 64-bit numeric type inlined into
the second 8 bytes of the block.

## PROPERTY\_VALUE {#property}

![Figure: Property value block](stringblock.png)

General `PROPERTY_VALUE` blocks reference arbitrary byte data across
one or more linked `EXTENT` blocks.

`PROPERTY_VALUE` blocks contain the index of the first `EXTENT` block holding
the binary data, and they contain the total length of the data in bytes
across all extents.

The format flags specify how the byte data should be interpreted,
as follows:

Enum    | Value | Meaning
----    | ----  | ----
kUtf8   | 0     | The byte data may be interpreted as a UTF-8 string.
kBinary | 1     | The byte data is arbitrary binary data and may not be printable.

## EXTENT {#extent}

![Figure: Extent block](extentblock.png)

`EXTENT` blocks contain an arbitrary byte data payload and the index of
the next `EXTENT` in the chain. The byte data for a property is retrieved
by reading each `EXTENT` in order until **Total Length** bytes are read.

## NAME {#name}

![Figure: Name block](nameblock.png)

`NAME` blocks give objects and values a human-readable identifier. They
consist of a UTF-8 payload that fits entirely within the given block.

## ARRAY\_VALUE {#array}

![Figure: Array block](arrayblock.png)

`ARRAY_VALUE` blocks contain an array of specifically 64-bit numeric
values.  The **Stored Value Type** field is interpreted exactly like
the **Type** field, but may only indicate `INT_VALUE`, `UINT_VALUE`, or
`DOUBLE_VALUE`.

Exactly **Count** entries of the given **Stored Value Type** appear in
the bytes at offset 16 into the block.

The **Display Format** field is used to affect how the array should be
displayed, and it is interpreted as follows:

Enum                  | Value | Description
---------             | ----  | ----
kFlat                 | 0     | Display as an ordered flat array with no additional formatting.
kLinearHistogram      | 1     | Interpret the first two entries as `floor` and `step_size` parameters for a linear histogram, as defined below.
kExponentialHistogram | 2     | Interpret the first three entries as `floor`, `initial_step`, and `step_multiplier` for an exponential histogram, as defined below.

### Linear Histogram

The array is a linear histogram that stores its parameters inline and
contains both an overflow and underflow bucket.

The first two elements are parameters `floor` and `step_size`, respectively
(as defined below).

The number of buckets (N) is implicitly `Count - 4`.

The remaining elements are buckets:

```
2:     (-inf, floor),
3:     [floor, floor+step_size),
i+3:   [floor + step_size*i, floor + step_size*(i+1)),
...
N+3:   [floor+step_size*N, +inf)
```

### Exponential Histogram

The array is an exponential histogram that stores its parameters inline
and contains both an overflow and underflow bucket.

The first three elements are parameters `floor`, `initial_step`, and
`step_multiplier` respectively (as defined below).

The number of buckets (N) is implicitly Count - 5.

The remaining are buckets:

```
3:     (-inf, floor),
4:     [floor, floor+initial_step),
i+4:   [floor + initial_step * step_multiplier^i, floor + initial_step * step_multiplier^(i+1))
N+4:   [floor + initial_step * step_multiplier^N, +inf)
```

## LINK\_VALUE {#link}

![Figure: Link block](linkblock.png)

`LINK_VALUE` blocks allow nodes to support children that are present
in a separate Inspect File.

The **Content Index** specifies another `NAME` block whose contents
are a unique identifier referring to another Inspect File. Readers are
expected to obtain a bundle of `(Identifier, File)` pairs (through either
a directory read or another interface) and they may attempt to follow
links by splicing the trees together using the stored identifiers.

The **Disposition Flags** instruct readers on how to splice the trees, as follows:

Enum               | Value | Description
----               | ----  | ----
kChildDisposition  | 0     | The hierarchy stored in the linked file should be a child of the `LINK_VALUE`'s parent.
kInlineDisposition | 1     | The children and properties of the root stored in the linked file should be added to the `LINK_VALUE`'s parent.

For example:

```
// root.inspect
root:
  int_value = 10
  child = LINK("other.inspect")

// other.inspect
root:
  test = "Hello World"
  next:
    value = 0


// kChildDisposition produces:
root:
  int_value = 10
  child:
    test = "Hello World"
    next:
      value = 0

// kInlineDisposition produces:
root:
  int_value = 10
  test = "Hello World"
  next:
    value = 0
```

Note: In all cases the name of the root node in the linked file is ignored.

In the event of a collision in child names between a Node and values being
added by its inline linked child, precedence is reader defined. Most
readers, however, would find it useful to have linked values take
precedence so they may override the original values.

# Concurrency Control {#concurrency}

Writers must use a global version counter so that readers can
detect in-flight modifications and modifications between reads without
communicating with the writer. This supports single-writer multi-reader
concurrency.

The strategy is for writers to increment a global *generation counter*
both when they begin and when they end a write operation.

This is a simple strategy with a significant benefit: between incrementing
the version number for beginning and ending a write the writer can perform
any number of operations on the buffer without regard for atomicity of
data updates.

The main drawback is that reads could be delayed indefinitely due to a
frequently updating writer, but readers can have mitigations in place
in practice.

## Reader Algorithm

Readers use the following algorithm to obtain a consistent snapshot of
an Inspect VMO:

1. Spinlock until the version number is even (no concurrent write),
2. Copy the entire VMO buffer, and
3. Check that the version number from the buffer is equal to the version
number from step 1.

As long as the version numbers match, the client may read their local
copy to construct the shared state.
If the version numbers do not match, the client may retry the whole
process.


## Writer Lock Algorithm

Writers lock an Inspect VMO for modification by doing the following:

```c
atomically_increment(generation_counter, acquire_ordering);
```

This locks the file against concurrent reads by setting the generation to an
odd number. Acquire ordering ensures that loads are not reordered before
this change.

## Writer Unlock Algorithm

Writers unlock an Inspect VMO following modification by doing the
following:

```c
atomically_increment(generation_counter, release_ordering);
```

Unlock the file allowing concurrent reads by setting the generation to
a new even number. Release ordering ensures that writes to the file are
visible before the generation count update is visible.

<!-- xrefs -->
[buddy]: https://en.wikipedia.org/wiki/Buddy_memory_allocation
[VMO]: /docs/concepts/objects/vm_object.md
[block.h]: /zircon/system/ulib/inspect/include/lib/inspect/cpp/vmo/block.h

