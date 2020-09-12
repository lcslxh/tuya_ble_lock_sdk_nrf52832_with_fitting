/**
 * \file tuya_ble_app_production_test.c
 *
 * \brief
 */
/*
 *  Copyright (C) 2014-2019, Tuya Inc., All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of tuya ble sdk
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "tuya_ble_type.h"
#include "tuya_ble_heap.h"
#include "tuya_ble_mem.h"
#include "tuya_ble_api.h"
#include "tuya_ble_port.h"
#include "tuya_ble_main.h"
#include "tuya_ble_internal_config.h"
#include "tuya_ble_data_handler.h"
#include "tuya_ble_mutli_tsf_protocol.h"
#include "tuya_ble_utils.h"
#include "tuya_ble_cryption_api.h"
#include "tuya_ble_main.h"
#include "tuya_ble_storage.h"
#include "tuya_ble_app_production_test.h"




__TUYA_BLE_WEAK void tuya_ble_app_production_test_process(uint8_t *p_in_data,uint16_t in_len,uint8_t *p_out_data,uint16_t *out_len)
{
    uint8_t cmd = p_in_data[3];
//    uint16_t data_len = (p_in_data[4]<<8) + p_in_data[5];
//    uint8_t *data_buffer = p_in_data+6;
    switch(cmd)
    {
        case TUYA_BLE_AUC_CMD_ENTER:
            
            break;
        case TUYA_BLE_AUC_CMD_QUERY_HID:
            
            break;
        case TUYA_BLE_AUC_CMD_GPIO_TEST:
            
            break;
        case TUYA_BLE_AUC_CMD_WRITE_AUTH_INFO:
            
            break;
        case TUYA_BLE_AUC_CMD_QUERY_INFO:
            
            break;
        case TUYA_BLE_AUC_CMD_RESET:
            
            break;
        case TUYA_BLE_AUC_CMD_QUERY_FINGERPRINT:
            
            break;
        case TUYA_BLE_AUC_CMD_WRITE_HID:
            
            break;
        case TUYA_BLE_AUC_CMD_RSSI_TEST:
            
            break;
        case TUYA_BLE_AUC_CMD_WRITE_OEM_INFO:
            
            break;     
        default:
            break;
    };
    
        
}




