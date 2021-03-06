/******************************************************************************
 *
 * Copyright(c) 2010 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2013 - 2014 Intel Mobile Communications GmbH
 * Copyright(c) 2018        Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#ifndef SRC_CONNECTIVITY_WLAN_DRIVERS_THIRD_PARTY_INTEL_IWLWIFI_IWL_TM_GNL_H_
#define SRC_CONNECTIVITY_WLAN_DRIVERS_THIRD_PARTY_INTEL_IWLWIFI_IWL_TM_GNL_H_

#include <linux/types.h>

#include "fw/testmode.h"

struct iwl_test_trace {
  uint32_t size;
  uint8_t* cpu_addr;
  dma_addr_t dma_addr;
  bool enabled;
};

struct iwl_test {
  struct iwl_test_trace trace;
  bool notify;
};

/**
 * struct iwl_tm_gnl_dev - Devices data base
 * @list:     Linked list to all devices
 * @trans:    Pointer to the owning transport
 * @dev_name:     Pointer to the device name
 * @cmd_handlers: Operation mode specific command handlers.
 *
 * Used to retrieve a device op mode pointer.
 * Device identifier it's name.
 */
struct iwl_tm_gnl_dev {
  struct list_head list;
  struct iwl_test tst;
  struct iwl_dnt* dnt;
  struct iwl_trans* trans;
  const char* dev_name;
  uint32_t nl_events_portid;
};

int iwl_tm_gnl_send_msg(struct iwl_trans* trans, uint32_t cmd, bool check_notify, void* data_out,
                        uint32_t data_len, gfp_t flags);

void iwl_tm_gnl_add(struct iwl_trans* trans);
void iwl_tm_gnl_remove(struct iwl_trans* trans);

int iwl_tm_gnl_init(void);
int iwl_tm_gnl_exit(void);
void iwl_tm_gnl_send_rx(struct iwl_trans* trans, struct iwl_rx_cmd_buffer* rxb);
#endif  // SRC_CONNECTIVITY_WLAN_DRIVERS_THIRD_PARTY_INTEL_IWLWIFI_IWL_TM_GNL_H_
