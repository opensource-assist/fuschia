// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

use {
    crate::error::*,
    crate::types::*,
    crate::utils::{Signal, SignalWatcher},
    std::collections::{HashMap, HashSet},
};

/// An object stored in the cloud.
pub struct Object {
    /// The data associated to this object.
    pub data: Vec<u8>,
}

/// Stores the set of devices.
pub struct DeviceSet {
    /// The set of fingerprints present on the cloud.
    fingerprints: HashSet<Fingerprint>,
    /// Signal for cloud erasure.
    erasure_signal: Signal,
}

/// Stores the state of a page.
pub struct PageCloud {
    /// The set of objects uploaded by the clients.
    objects: HashMap<ObjectId, Object>,
    /// The ids of commits uploaded by the clients, stored in upload order.
    commit_log: Vec<CommitId>,
    /// The set of commits uploaded by the clients.
    commits: HashMap<CommitId, Commit>,
    /// Signal for new commits.
    commit_signal: Signal,
}

/// Stores the state of the cloud.
pub struct Cloud {
    pages: HashMap<PageId, PageCloud>,
    device_set: DeviceSet,
}

pub type PageCloudWatcher = SignalWatcher;
impl PageCloud {
    /// Creates a new, empty page.
    pub fn new() -> PageCloud {
        PageCloud {
            objects: HashMap::new(),
            commit_log: Vec::new(),
            commits: HashMap::new(),
            commit_signal: Signal::new(),
        }
    }

    /// Returns the given object, or ObjectNotFound.
    pub fn get_object(&self, id: &ObjectId) -> Result<&Object, ClientError> {
        if let Some(object) = self.objects.get(id) {
            Ok(object)
        } else {
            Err(client_error(Status::NotFound)
                .with_explanation(format!("Object not found: {:?}", id)))
        }
    }

    /// Adds an object to the cloud. The object may already be
    /// present.
    pub fn add_object(&mut self, id: ObjectId, object: Object) -> Result<(), ClientError> {
        self.objects.insert(id, object);
        Ok(())
    }

    /// Atomically adds a series a commits to the cloud and updates
    /// the commit log. Commits that were already present are not
    /// re-added to the log.
    pub fn add_commits(&mut self, commits: Vec<(Commit, Option<Diff>)>) -> Result<(), ClientError> {
        let mut will_insert: Vec<CommitId> = Vec::new();

        for (commit, _diff) in commits.iter() {
            if let Some(_existing) = self.commits.get(&commit.id) {
                continue;
            };
            will_insert.push(commit.id.clone())
        }

        // Mutate the data structure.
        self.commit_log.append(&mut will_insert);
        for commit in commits.into_iter() {
            self.commits.insert(commit.0.id.clone(), commit.0);
        }

        self.commit_signal.signal_and_rearm();

        Ok(())
    }

    /// Returns a future that will wake up on new commits, or None if
    /// position is not after the latest commit.
    pub fn watch(&self, position: Token) -> Option<PageCloudWatcher> {
        if position.0 < self.commit_log.len() {
            None
        } else {
            // We ignore cancellations, because extra watch notifications are OK.
            Some(self.commit_signal.watch())
        }
    }

    /// Returns a vector of new commits after position and a new
    /// position.
    pub fn get_commits(&self, position: Token) -> Option<(Token, Vec<&Commit>)> {
        if position.0 >= self.commit_log.len() {
            return None;
        };

        let new_commits = self.commit_log[position.0..]
            .iter()
            .map(|id| self.commits.get(id).expect("Unknown commit in commit log."))
            .collect();
        Some((Token(self.commit_log.len()), new_commits))
    }

    /// Returns a diff from one of the diff bases.
    pub fn get_diff(
        &self,
        _commit: CommitId,
        _possible_bases: Vec<CommitId>,
    ) -> Result<Diff, ClientError> {
        Err(client_error(Status::NotSupported).with_explanation("get_diff"))
    }
}

impl Cloud {
    /// Creates a new, empty cloud storage.
    pub fn new() -> Cloud {
        Self { device_set: DeviceSet::new(), pages: HashMap::new() }
    }

    /// Returns the page with the given id. The page is created if
    /// absent.
    pub fn get_page(&mut self, id: PageId) -> &mut PageCloud {
        self.pages.entry(id).or_insert_with(|| PageCloud::new())
    }

    /// Returns the device set.
    pub fn get_device_set(&mut self) -> &mut DeviceSet {
        &mut self.device_set
    }
}

pub type DeviceSetWatcher = SignalWatcher;
impl DeviceSet {
    /// Creates a new, empty device set.
    pub fn new() -> DeviceSet {
        DeviceSet { fingerprints: HashSet::new(), erasure_signal: Signal::new() }
    }

    /// Adds a fingerprint to the set. Nothing happens if the
    /// fingerprint is already present.
    pub fn set_fingerprint(&mut self, fingerprint: Fingerprint) {
        self.fingerprints.insert(fingerprint);
    }

    /// Checks that a fingerprint is present in the cloud.
    pub fn check_fingerprint(&self, fingerprint: &Fingerprint) -> bool {
        self.fingerprints.contains(fingerprint)
    }

    /// Erases all fingerprints and calls the watchers.
    pub fn erase(&mut self) {
        self.fingerprints.clear();
        self.erasure_signal.signal_and_rearm()
    }

    /// If `fingerprint` is present on the cloud, returns a future that
    /// completes when the cloud is erased. Otherwise, returns
    /// `None`.
    pub fn watch(&self, fingerprint: &Fingerprint) -> Option<DeviceSetWatcher> {
        if !self.fingerprints.contains(fingerprint) {
            return None;
        }
        Some(self.erasure_signal.watch())
    }
}
