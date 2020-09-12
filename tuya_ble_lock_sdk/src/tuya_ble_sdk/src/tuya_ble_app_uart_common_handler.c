/**
 * \file tuya_ble_app_uart_common_handler.c
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
#include "tuya_ble_app_uart_common_handler.h"



void tuya_ble_uart_common_process(uint8_t *p_in_data,uint16_t in_len,uint8_t *p_out_data,uint16_t *out_len)
{
    uint8_t cmd = p_in_data[3];
//    uint16_t data_len = (p_in_data[4]<<8) + p_in_data[5];
//    uint8_t *data_buffer = p_in_data+6;
    switch(cmd)
    {
        case TUYA_BLE_UART_COMMON_HEART_MSG_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_SEARCH_PID_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_CK_MCU_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_REPORT_WORK_STATE_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_RESET_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_SEND_CMD_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_SEND_STATUS_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_QUERY_STATUS:
            
            break;
        case TUYA_BLE_UART_COMMON_SEND_STORAGE_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_SEND_TIME_SYNC_TYPE:
            
            break;
        case TUYA_BLE_UART_COMMON_MODIFY_ADV_INTERVAL:
            
            break;
        case TUYA_BLE_UART_COMMON_TURNOFF_SYSTEM_TIME:
            
            break;
        case TUYA_BLE_UART_COMMON_ENANBLE_LOWER_POWER:
            
            break;
        case TTUYA_BLE_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN:
            
            break;
        case TUYA_BLE_UART_COMMON_ACTIVE_DISCONNECT:
            
            break;
        case TUYA_BLE_UART_COMMON_MODIFY_BLE_CONN_INTERVER:
            
            break;
        case TUYA_BLE_UART_COMMON_BLE_OTA_STATUS:
            
            break;       
        default:
            break;
    };
    
        
}











