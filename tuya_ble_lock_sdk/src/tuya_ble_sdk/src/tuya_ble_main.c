/**
 * \file tuya_ble_main.c
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
#include "tuya_ble_cryption_api.h"
#include "tuya_ble_data_handler.h"
#include "tuya_ble_storage.h"
#include "tuya_ble_sdk_version.h"
#include "tuya_ble_event.h"
#include "tuya_ble_utils.h"
#include "tuya_ble_app_uart_common_handler.h"
#include "tuya_ble_app_production_test.h"

tuya_ble_parameters_settings_t tuya_ble_current_para;


static volatile tuya_ble_connect_status_t tuya_ble_connect_status = UNKNOW_STATUS;

#if TUYA_BLE_USE_OS
uint32_t m_queue_table[TUYA_BLE_MAX_CALLBACKS];
#else
tuya_ble_callback_t m_cb_table[TUYA_BLE_MAX_CALLBACKS];
#endif


void tuya_ble_connect_status_set(tuya_ble_connect_status_t status)
{
    tuya_ble_device_enter_critical();
    tuya_ble_connect_status = status;
    tuya_ble_device_exit_critical();

}

tuya_ble_connect_status_t tuya_ble_connect_status_get(void)
{
    return  tuya_ble_connect_status;
}


static void tuya_ble_handle_device_info_update_evt(tuya_ble_evt_param_t *evt)
{
//    tuya_ble_cb_evt_param_t event;
//    tuya_ble_connect_status_t current_connect_status;

    switch(evt->device_info_data.type)
    {
    case DEVICE_INFO_TYPE_PID:
        tuya_ble_current_para.pid_type = TUYA_BLE_PRODUCT_ID_TYPE_PID;
        tuya_ble_current_para.pid_len = evt->device_info_data.len;
        memcpy(tuya_ble_current_para.pid,evt->device_info_data.data,tuya_ble_current_para.pid_len);
        tuya_ble_adv_change();
        break;

    case DEVICE_INFO_TYPE_PRODUCT_KEY:
        tuya_ble_current_para.pid_type = TUYA_BLE_PRODUCT_ID_TYPE_PRODUCT_KEY;
        tuya_ble_current_para.pid_len = evt->device_info_data.len;
        memcpy(tuya_ble_current_para.pid,evt->device_info_data.data,tuya_ble_current_para.pid_len);
        tuya_ble_adv_change();
        break;
    case DEVICE_INFO_TYPE_LOGIN_KEY:
#if (!TUYA_BLE_DEVICE_REGISTER_FROM_BLE)
        if(memcmp(tuya_ble_current_para.sys_settings.login_key,evt->device_info_data.data,LOGIN_KEY_LEN))
        {
            memcpy(tuya_ble_current_para.sys_settings.login_key,evt->device_info_data.data,LOGIN_KEY_LEN);

            tuya_ble_storage_save_sys_settings();
        }
#endif
        break;
    case DEVICE_INFO_TYPE_BOUND:

#if (!TUYA_BLE_DEVICE_REGISTER_FROM_BLE)
        if(tuya_ble_current_para.sys_settings.bound_flag != evt->device_info_data.data[0])
        {
            tuya_ble_current_para.sys_settings.bound_flag = evt->device_info_data.data[0];
            tuya_ble_storage_save_sys_settings();
            tuya_ble_adv_change();
            current_connect_status = tuya_ble_connect_status_get();
            if(tuya_ble_current_para.sys_settings.bound_flag==1) //0->1
            {
                if(current_connect_status==UNBONDING_CONN)
                {
                    tuya_ble_connect_status_set(BONDING_CONN);
                }
                else if(current_connect_status==UNBONDING_UNAUTH_CONN)
                {
                    tuya_ble_connect_status_set(BONDING_UNAUTH_CONN);
                }
                else if(current_connect_status==UNBONDING_UNCONN)
                {
                    tuya_ble_connect_status_set(BONDING_CONN);
                }
                else
                {

                }

            }
            else  //1->0
            {
                if(current_connect_status==BONDING_CONN)
                {
                    tuya_ble_connect_status_set(UNBONDING_CONN);
                }
                else if(current_connect_status==BONDING_UNAUTH_CONN)
                {
                    tuya_ble_connect_status_set(UNBONDING_UNAUTH_CONN);
                }
                else if(current_connect_status==BONDING_UNCONN)
                {
                    tuya_ble_connect_status_set(UNBONDING_CONN);
                }
                else
                {

                }
            }

            event.evt = TUYA_BLE_CB_EVT_CONNECTE_STATUS;
            event.connect_status = tuya_ble_connect_status_get();
            if(tuya_ble_cb_event_send(&event)!=0)
            {
                TUYA_BLE_LOG("TUYA_BLE_LOG :tuya ble send cb event failed.\n");
            }
            else
            {
                TUYA_BLE_LOG("TUYA_BLE_LOG :tuya ble send cb event succeed.\n");
            }

        }
#endif
        break;
    default:
        break;
    }

}


static void tuya_ble_handle_dp_data_reported_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t encry_mode = 0;
    if(tuya_ble_pair_rand_valid_get()==1)
    {
        encry_mode = ENCRYPTION_MODE_SESSION_KEY;
    }
    else
    {
        encry_mode = ENCRYPTION_MODE_KEY_4;
    }
    tuya_ble_commData_send(FRM_STAT_REPORT,0,evt->reported_data.p_data,evt->reported_data.data_len,encry_mode);

    if(evt->reported_data.p_data)
    {
        tuya_ble_free(evt->reported_data.p_data);
    }
}


static void tuya_ble_handle_dp_data_with_time_reported_evt(tuya_ble_evt_param_t *evt)
{
    static uint8_t data_buffer[TUYA_BLE_AIR_FRAME_MAX];
    memset(data_buffer,0,TUYA_BLE_AIR_FRAME_MAX);
    uint16_t data_len;

    uint8_t encry_mode = 0;
    if(tuya_ble_pair_rand_valid_get()==1)
    {
        encry_mode = ENCRYPTION_MODE_SESSION_KEY;
    }
    else
    {
        encry_mode = ENCRYPTION_MODE_KEY_4;
    }

    data_buffer[0] = 1;
    data_buffer[1] = evt->reported_with_time_data.timestamp>>24;
    data_buffer[2] = evt->reported_with_time_data.timestamp>>16;
    data_buffer[3] = evt->reported_with_time_data.timestamp>>8;
    data_buffer[4] = evt->reported_with_time_data.timestamp;

    memcpy(&data_buffer[5],evt->reported_with_time_data.p_data,evt->reported_with_time_data.data_len);

    data_len = 5+evt->reported_with_time_data.data_len;

    tuya_ble_commData_send(FRM_STAT_WITH_TIME_REPORT,0,data_buffer,data_len,encry_mode);
    if(evt->reported_with_time_data.p_data)
    {
        tuya_ble_free(evt->reported_with_time_data.p_data);
    }
}


static void tuya_ble_handle_factory_reset_evt(tuya_ble_evt_param_t *evt)
{
    tuya_ble_current_para.sys_settings.bound_flag = 0;
    tuya_ble_storage_save_sys_settings();
    tuya_ble_adv_change();
}


static void tuya_ble_handle_ota_response_evt(tuya_ble_evt_param_t *evt)
{
    uint16_t ota_cmd_type = 0;

    uint8_t encry_mode = 0;
    if(tuya_ble_pair_rand_valid_get()==1)
    {
        encry_mode = ENCRYPTION_MODE_SESSION_KEY;
    }
    else
    {
        encry_mode = ENCRYPTION_MODE_KEY_4;
    }

    switch (evt->ota_response_data.type)
    {
    case TUYA_BLE_OTA_REQ:
        ota_cmd_type = FRM_OTA_START_REQ;
        break;
    case TUYA_BLE_OTA_FILE_INFO:
        ota_cmd_type = FRM_OTA_FILE_INFOR_REQ;
        break;
    case TUYA_BLE_OTA_FILE_OFFSET_REQ :
        ota_cmd_type = FRM_OTA_FILE_OFFSET_REQ;
        break;
    case TUYA_BLE_OTA_DATA :
        ota_cmd_type = FRM_OTA_DATA_REQ;
        break;
    case TUYA_BLE_OTA_END :
        ota_cmd_type = FRM_OTA_END_REQ;
        break;
    default:
        break;
    }

    if(ota_cmd_type!=0)
    {
        tuya_ble_commData_send(ota_cmd_type,0,evt->ota_response_data.p_data,evt->ota_response_data.data_len,encry_mode);
    }

    if(evt->ota_response_data.p_data)
    {
        tuya_ble_free(evt->ota_response_data.p_data);
    }
}


static void tuya_ble_handle_data_passthrough_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t encry_mode = 0;

    if(tuya_ble_current_para.sys_settings.bound_flag)
    {
        if(tuya_ble_pair_rand_valid_get()==1)
        {
            encry_mode = ENCRYPTION_MODE_SESSION_KEY;
        }
        else
        {
            encry_mode = ENCRYPTION_MODE_KEY_4;
        }
    }
    else
    {
        encry_mode = ENCRYPTION_MODE_KEY_2;
    }

    tuya_ble_commData_send(FRM_DATA_PASSTHROUGH_REQ,0,evt->passthrough_data.p_data,evt->passthrough_data.data_len,encry_mode);

    if(evt->passthrough_data.p_data)
    {
        tuya_ble_free(evt->passthrough_data.p_data);
    }
}


static void tuya_ble_handle_uart_cmd_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t sum;
    uint8_t *uart_send_buffer;
    uint16_t uart_send_len;

    TUYA_BLE_HEXDUMP("received uart cmd data",16,(uint8_t*)evt->uart_cmd_data.p_data,evt->uart_cmd_data.data_len);//
//    tuya_ble_uart_send_data((uint8_t*)evt->uart_cmd_data.p_data,evt->uart_cmd_data.data_len);

#if (TUYA_BLE_DEVICE_REGISTER_FROM_BLE)

    if(evt->uart_cmd_data.data_len>0)
    {
        sum = check_sum(evt->uart_cmd_data.p_data,evt->uart_cmd_data.data_len-1);
        if(sum==evt->uart_cmd_data.p_data[evt->uart_cmd_data.data_len-1] || (evt->uart_cmd_data.p_data[3] == 0xF1))
        {
            uart_send_len = 0;
            uart_send_buffer=(uint8_t *)tuya_ble_malloc(256);
            if(uart_send_buffer!=NULL)
            {
                switch(evt->uart_cmd_data.p_data[0])
                {
                case 0x55:
                    tuya_ble_uart_common_process(evt->uart_cmd_data.p_data,evt->uart_cmd_data.data_len,uart_send_buffer,&uart_send_len);
                    if(uart_send_len>0)
                    {                        
                        tuya_ble_uart_send_data(uart_send_buffer,uart_send_len);
                    }
                    break;
                case 0x66:
                    tuya_ble_app_production_test_process(evt->uart_cmd_data.p_data,evt->uart_cmd_data.data_len,uart_send_buffer,&uart_send_len);
                    if(uart_send_len>0)
                    {                        
                        tuya_ble_uart_send_data(uart_send_buffer,uart_send_len);
                    }
                    break;
                default:
                    break;
                };
                
                tuya_ble_free(uart_send_buffer);
            }
            else
            {
                TUYA_BLE_LOG("tuya_ble_handle_uart_cmd_evt tuya_MemGet uart send buffer fail.");
            }
        }
        else
        {
            TUYA_BLE_LOG("uart receive data check_sum error , receive sum = 0x%02x ; cal sum = 0x%02x",evt->uart_cmd_data.p_data[evt->uart_cmd_data.data_len-1],sum);
        }
    }

#endif

    if(evt->uart_cmd_data.p_data)
    {
        tuya_ble_free(evt->uart_cmd_data.p_data);
    }
}



static void tuya_ble_handle_ble_cmd_evt(tuya_ble_evt_param_t *evt)
{

    tuya_ble_evt_process(evt->ble_cmd_data.cmd,evt->ble_cmd_data.p_data,evt->ble_cmd_data.data_len);
    if(evt->ble_cmd_data.p_data)
    {
        tuya_ble_free(evt->ble_cmd_data.p_data);
    }
}


static void tuya_ble_handle_net_config_response_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t encry_mode = 0;
    uint8_t data[2];

    if(tuya_ble_pair_rand_valid_get()==1)
    {
        encry_mode = ENCRYPTION_MODE_KEY_2;
    }
    else
    {
        encry_mode = ENCRYPTION_MODE_KEY_1;
    }


    data[0] = ((int16_t)(evt->net_config_response_data.result_code))>>8;
    data[1] = (int16_t)(evt->net_config_response_data.result_code);

    tuya_ble_commData_send(FRM_NET_CONFIG_RESPONSE_REPORT_REQ,0,data,2,encry_mode);

}

#if (!TUYA_BLE_DEVICE_REGISTER_FROM_BLE)
static void tuya_ble_handle_unbound_response_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t encry_mode = 0;
    uint8_t result;

    encry_mode = ENCRYPTION_MODE_SESSION_KEY;

    result = evt->ubound_res_data.result_code;

    tuya_ble_commData_send(FRM_UNBONDING_RESP,0,&result,1,encry_mode);
}

static void tuya_ble_handle_anomaly_unbound_response_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t encry_mode = 0;
    uint8_t result;

    encry_mode = ENCRYPTION_MODE_SESSION_KEY;

    result = evt->anomaly_ubound_res_data.result_code;

    tuya_ble_commData_send(FRM_ANOMALY_UNBONDING_RESP,0,&result,1,encry_mode);
}

static void tuya_ble_handle_device_reset_response_evt(tuya_ble_evt_param_t *evt)
{
    uint8_t encry_mode = 0;
    uint8_t result;

    encry_mode = ENCRYPTION_MODE_SESSION_KEY;

    result = evt->device_reset_res_data.result_code;

    tuya_ble_commData_send(FRM_DEVICE_RESET_RESP,0,&result,1,encry_mode);
}

#endif


extern void tuya_ble_connect_monitor_timer_start(void);

static void tuya_ble_handle_connect_change_evt(tuya_ble_evt_param_t *evt)
{
    tuya_ble_cb_evt_param_t event;
    uint8_t send_cb_flag = 1;

    if(evt->connect_change_evt==TUYA_BLE_CONNECTED)
    {
        TUYA_BLE_LOG("Connected\n");
        tuya_ble_reset_ble_sn();
        if(tuya_ble_current_para.sys_settings.bound_flag!=1)
        {
            tuya_ble_connect_status_set(UNBONDING_UNAUTH_CONN);
        }
        else
        {
            //send_cb_flag = 0;
            tuya_ble_connect_status_set(BONDING_UNAUTH_CONN);
        }
        
        tuya_ble_connect_monitor_timer_start();

    }
    else if(evt->connect_change_evt==TUYA_BLE_DISCONNECTED)
    {
        TUYA_BLE_LOG("Disonnected\n");

        tuya_ble_reset_ble_sn();

        tuya_ble_pair_rand_clear();

        if(tuya_ble_current_para.sys_settings.bound_flag==1)
        {
            tuya_ble_connect_status_set(BONDING_UNCONN);
        }
        else
        {
            tuya_ble_connect_status_set(UNBONDING_UNCONN);
        }
    }
    else
    {
        TUYA_BLE_LOG("unknown connect_change_evt!\n");
    }

    if(send_cb_flag)
    {
        event.evt = TUYA_BLE_CB_EVT_CONNECTE_STATUS;
        event.connect_status = tuya_ble_connect_status_get();
        if(tuya_ble_cb_event_send(&event)!=0)
        {
            TUYA_BLE_LOG("TUYA_BLE_LOG :tuya ble send cb event failed.\n");
        }
        else
        {
            TUYA_BLE_LOG("TUYA_BLE_LOG :tuya ble send cb event succeed.\n");
        }

    }

}


void tuya_ble_event_process(tuya_ble_evt_param_t *tuya_ble_evt)
{
    switch(tuya_ble_evt->hdr.event)
    {
    case TUYA_BLE_EVT_DEVICE_INFO_UPDATE:
        tuya_ble_handle_device_info_update_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_DP_DATA_REPORTED:
        tuya_ble_handle_dp_data_reported_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_FACTORY_RESET:
        tuya_ble_handle_factory_reset_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_OTA_RESPONSE:
        tuya_ble_handle_ota_response_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_DATA_PASSTHROUGH:
        tuya_ble_handle_data_passthrough_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_MTU_DATA_RECEIVE:
        // TUYA_BLE_LOG("ble receive data event\n");
        // tuya_ble_commData_send_mtu(tuya_evt.ble_rev_event.data,tuya_evt.ble_rev_event.len);
//        TUYA_BLE_HEXDUMP("received ble mtu data",20,(uint8_t*)tuya_ble_evt->mtu_data.data,tuya_ble_evt->mtu_data.len);//
        tuya_ble_commonData_rx_proc(tuya_ble_evt->mtu_data.data,tuya_ble_evt->mtu_data.len);
        break;
    case TUYA_BLE_EVT_DP_DATA_WITH_TIME_REPORTED:
        tuya_ble_handle_dp_data_with_time_reported_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_UART_CMD:
        tuya_ble_handle_uart_cmd_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_BLE_CMD:
        tuya_ble_handle_ble_cmd_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_NET_CONFIG_RESPONSE:
        tuya_ble_handle_net_config_response_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_CUSTOM:
        tuya_ble_evt->custom_evt.custom_event_handler(tuya_ble_evt->custom_evt.data);
        break;
    case TUYA_BLE_EVT_CONNECT_STATUS_UPDATE:
        tuya_ble_handle_connect_change_evt(tuya_ble_evt);
        break;
#if (!TUYA_BLE_DEVICE_REGISTER_FROM_BLE)
    case TUYA_BLE_EVT_UNBOUND_RESPONSE:
        tuya_ble_handle_unbound_response_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_ANOMALY_UNBOUND_RESPONSE:
        tuya_ble_handle_anomaly_unbound_response_evt(tuya_ble_evt);
        break;
    case TUYA_BLE_EVT_DEVICE_RESET_RESPONSE:
        tuya_ble_handle_device_reset_response_evt(tuya_ble_evt);
        break;
#endif
    default:
        break;
    }
}




#if TUYA_BLE_USE_OS

void *tuya_ble_task_handle;   //!< APP Task handle
void *tuya_ble_queue_handle;  //!< Event queue handle

#define TUYA_TASK_STACK_SIZE              256 * 10   //!<  Task stack size

static void tuya_ble_main_task(void *p_param)
{
    tuya_ble_evt_param_t tuya_ble_evt;
    uint32_t i = 0;

    while (true)
    {
        if (tuya_ble_os_msg_queue_recv(tuya_ble_queue_handle, &tuya_ble_evt, 0xFFFFFFFF) == true)
        {

            //TUYA_BLE_LOG("TUYA_RECEIVE_EVT-0x%04x\n",tuya_ble_evt.hdr.event);
            tuya_ble_event_process(&tuya_ble_evt);
            /*
             switch(tuya_ble_evt.hdr.event)
             {
             case TUYA_BLE_EVT_DEVICE_INFO_UPDATE:
                 tuya_ble_handle_device_info_update_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_DP_DATA_REPORTED:
                 tuya_ble_handle_dp_data_reported_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_FACTORY_RESET:
                 tuya_ble_handle_factory_reset_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_OTA_RESPONSE:
                 tuya_ble_handle_ota_response_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_DATA_PASSTHROUGH:
                 tuya_ble_handle_data_passthrough_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_MTU_DATA_RECEIVE:
                 // TUYA_BLE_LOG("ble receive data event\n");
                 // tuya_ble_gatt_send_data(tuya_ble_evt.mtu_data.data,tuya_ble_evt.mtu_data.len);
                 // tuya_ble_commData_send_mtu(tuya_evt.ble_rev_event.data,tuya_evt.ble_rev_event.len);
                 //TUYA_BLE_HEXDUMP("received ble mtu data",20,(uint8_t*)tuya_ble_evt.mtu_data.data,tuya_ble_evt.mtu_data.len);//
                 tuya_ble_commonData_rx_proc(tuya_ble_evt.mtu_data.data,tuya_ble_evt.mtu_data.len);
                 break;
             case TUYA_BLE_EVT_DP_DATA_WITH_TIME_REPORTED:
                 tuya_ble_handle_dp_data_with_time_reported_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_UART_CMD:
                 tuya_ble_handle_uart_cmd_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_BLE_CMD:
                 tuya_ble_handle_ble_cmd_evt(&tuya_ble_evt);
                 break;
             case TUYA_BLE_EVT_NET_CONFIG_RESPONSE:
                 tuya_ble_handle_net_config_response_evt(&tuya_ble_evt);
                 break;
             default:
                 break;
             }
             */
        }

    }

}

#else

void tuya_ble_main_tasks_exec(void)
{
    tuya_sched_execute();
}
#endif




void tuya_ble_event_init(void)
{
#if TUYA_BLE_USE_OS
    tuya_ble_os_task_create(&tuya_ble_task_handle, "tuya", tuya_ble_main_task, 0, TUYA_TASK_STACK_SIZE,TUYA_BLE_TASK_PRIORITY);
    tuya_ble_os_msg_queue_create(&tuya_ble_queue_handle, MAX_NUMBER_OF_TUYA_MESSAGE, sizeof(tuya_ble_evt_param_t));

#else
    tuya_ble_event_queue_init();
#endif

#if TUYA_BLE_USE_OS
    for(uint8_t i= 0; i<TUYA_BLE_MAX_CALLBACKS; i++)
    {
        m_queue_table[i] = 0;
    }
#else
    for(uint8_t i= 0; i<TUYA_BLE_MAX_CALLBACKS; i++)
    {
        m_cb_table[i] = NULL;
    }
#endif

}


uint8_t tuya_ble_event_send(tuya_ble_evt_param_t *evt)
{
    static tuya_ble_evt_param_t event;
    memcpy(&event,evt,sizeof(tuya_ble_evt_param_t));
#if TUYA_BLE_USE_OS
    if(tuya_ble_os_msg_queue_send(tuya_ble_queue_handle, &event, 0))
    {
        return 0;
    }
    else
    {
        return 1;
    }
#else
    if(tuya_ble_message_send(&event)==TUYA_BLE_SUCCESS)
    {
        return 0;
    }
    else
    {
        return 1;
    }
#endif
}


uint8_t tuya_ble_custom_event_send(tuya_ble_custom_evt_t evt)
{
    static tuya_ble_evt_param_t event;

#if TUYA_BLE_USE_OS
    event.hdr.event = TUYA_BLE_EVT_CUSTOM;
    event.custom_evt = evt;
    if(tuya_ble_os_msg_queue_send(tuya_ble_queue_handle, &event, 0))
    {
        return 0;
    }
    else
    {
        return 1;
    }
#else
    tuya_ble_device_enter_critical();
    event.hdr.event = TUYA_BLE_EVT_CUSTOM;
    event.custom_evt = evt;

    if(tuya_ble_message_send(&event)==TUYA_BLE_SUCCESS)
    {
        tuya_ble_device_exit_critical();
        return 0;
    }
    else
    {
        tuya_ble_device_exit_critical();
        return 1;
    }
#endif
}



tuya_ble_status_t tuya_ble_inter_event_response(tuya_ble_cb_evt_param_t *param)
{

    switch (param->evt)
    {
    case TUYA_BLE_CB_EVT_CONNECTE_STATUS:
        break;
    case TUYA_BLE_CB_EVT_DP_WRITE:
        if(param->dp_write_data.p_data)
        {
            tuya_ble_free(param->dp_write_data.p_data);
        }
        break;
    case TUYA_BLE_CB_EVT_DP_QUERY:
        if(param->dp_query_data.p_data)
        {
            tuya_ble_free(param->dp_query_data.p_data);
        }
        break;
    case TUYA_BLE_CB_EVT_OTA_DATA:
        if(param->ota_data.p_data)
        {
            tuya_ble_free(param->ota_data.p_data);
        }
        break;
    case TUYA_BLE_CB_EVT_NETWORK_INFO:
        if(param->network_data.p_data)
        {
            tuya_ble_free(param->network_data.p_data);
        }
        break;
    case TUYA_BLE_CB_EVT_WIFI_SSID:
        if(param->wifi_info_data.p_data)
        {
            tuya_ble_free(param->wifi_info_data.p_data);
        }
        break;
    case TUYA_BLE_CB_EVT_TIME_STAMP:
        break;
    case TUYA_BLE_CB_EVT_TIME_NORMAL:
        break;
    case TUYA_BLE_CB_EVT_DATA_PASSTHROUGH:

        if(param->ble_passthrough_data.p_data)
        {
            tuya_ble_free(param->ble_passthrough_data.p_data);
        }
        break;
    default:
        break;
    }

    return TUYA_BLE_SUCCESS;
}



uint8_t tuya_ble_cb_event_send(tuya_ble_cb_evt_param_t *evt)
{
    static tuya_ble_cb_evt_param_t event;
    // memset(&event,0,sizeof(tuya_ble_evt_param_t));
    memcpy(&event,evt,sizeof(tuya_ble_cb_evt_param_t));
#if TUYA_BLE_USE_OS
    if(m_queue_table[0])
    {
        if(tuya_ble_os_msg_queue_send((void *)m_queue_table[0], &event, 0))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
#else
    tuya_ble_callback_t fun;
    if(m_cb_table[0])
    {
        fun = m_cb_table[0];
        fun(evt);
        tuya_ble_inter_event_response(evt);
    }
#endif
    return 0;

}


#if (TUYA_BLE_PROTOCOL_VERSION_HIGN==3)

/** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */

#define TUYA_BLE_ADV_DATA_LEN  (12+TUYA_BLE_PRODUCT_ID_MAX_LEN)

static const uint8_t adv_data_const[TUYA_BLE_ADV_DATA_LEN] =
{
    0x02,
    0x01,
    0x06,
    0x03,
    0x02,
    0x01, 0xA2,
    0x14,
    0x16,
    0x01, 0xA2,
    0x00,         //id type 00-pid 01-product key
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief  GAP - scan response data (max size = 31 bytes) */

#define TUYA_BLE_SCAN_RSP_DATA_LEN  30
static const uint8_t scan_rsp_data_const[TUYA_BLE_SCAN_RSP_DATA_LEN] =
{
    0x03,
    0x09,
    0x4C, 0x4A,
    0x19,             /* length */
    0xFF,
    0xD0,
    0x07,
    0x00, //bond flag bit7 （8）
    0x03, //protocol version
    0x01, //Encry Mode （10）
    0x00,0x00, //communication way bit0-mesh bit1-wifi bit2-zigbee bit3-NB
    0x00, //data type
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


void tuya_ble_adv_change(void)
{

//    extern uint8_t p_id[PRODUCT_ID_LEN];
    static uint8_t aes_buf[200];
    uint8_t aes_key[16];
    uint8_t encry_device_id[DEVICE_ID_LEN];
//    uint8_t device_type;

    //
    static uint8_t adv_data[TUYA_BLE_ADV_DATA_LEN];
    static uint8_t scan_rsp_data[TUYA_BLE_SCAN_RSP_DATA_LEN];

    memcpy(adv_data,adv_data_const,TUYA_BLE_ADV_DATA_LEN);
    memcpy(&scan_rsp_data,scan_rsp_data_const,TUYA_BLE_SCAN_RSP_DATA_LEN);

    adv_data[7] = 4+tuya_ble_current_para.pid_len;
    adv_data[11] = tuya_ble_current_para.pid_type;

    scan_rsp_data[9] = TUYA_BLE_PROTOCOL_VERSION_HIGN;
    
    scan_rsp_data[10] = TUYA_BLE_SECURE_CONNECTION_TYPE;
    
    scan_rsp_data[11] = TUYA_BLE_DEVICE_COMMUNICATION_ABILITY>>8;
    scan_rsp_data[12] = TUYA_BLE_DEVICE_COMMUNICATION_ABILITY;

    if(tuya_ble_current_para.sys_settings.bound_flag == 1)
    {
        scan_rsp_data[8] |=0x80 ;
        //
        memcpy(aes_key,tuya_ble_current_para.sys_settings.login_key,LOGIN_KEY_LEN);
        memcpy(aes_key+LOGIN_KEY_LEN,tuya_ble_current_para.auth_settings.device_id,16-LOGIN_KEY_LEN);

        tuya_ble_encrypt_old_with_key(aes_key,tuya_ble_current_para.auth_settings.device_id,DEVICE_ID_LEN,aes_buf);

        memcpy(&adv_data[12],(uint8_t *)(aes_buf+1),tuya_ble_current_para.pid_len);

        memset(aes_key,0,sizeof(aes_key));
        memcpy(aes_key,&adv_data[12],tuya_ble_current_para.pid_len);

        tuya_ble_device_id_encrypt(aes_key,tuya_ble_current_para.pid_len,tuya_ble_current_para.auth_settings.device_id,DEVICE_ID_LEN,encry_device_id);

        memcpy(&scan_rsp_data[14],encry_device_id,DEVICE_ID_LEN);

    }
    else
    {
        scan_rsp_data[8] &=(~0x80);

        memcpy(&adv_data[12],tuya_ble_current_para.pid,tuya_ble_current_para.pid_len);
        tuya_ble_device_id_encrypt(tuya_ble_current_para.pid,tuya_ble_current_para.pid_len,tuya_ble_current_para.auth_settings.device_id,DEVICE_ID_LEN,encry_device_id);


        memcpy(&scan_rsp_data[14],encry_device_id,DEVICE_ID_LEN);
    }
    tuya_ble_gap_advertising_adv_data_update(adv_data,sizeof(adv_data));
    tuya_ble_gap_advertising_scan_rsp_data_update(scan_rsp_data,sizeof(scan_rsp_data));

}



#else

/** @brief  GAP - scan response data (max size = 31 bytes) */
static const uint8_t scan_rsp_data_const[30] =
{
    /* Manufacturer Specific Data */
    0x1D,             /* length */
    0xFF,
    0x59,
    0x02,
    0x00,0x02,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


/** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
static const uint8_t adv_data_const[20] =
{
    /* Flags */
    0x02,             /* length */
    0x01, /* type="Flags" */
    0x05,
    /* Local name */
    0x0C,             /* length */
    0x09,
    'T', 'U', 'Y', 'A', '_', 'C', 'O', 'M', 'M', 'O','N',
    0x03,
    0x02,
    0x01, 0xA2,
};



void tuya_ble_adv_change(void)
{
//    extern uint8_t p_id[PRODUCT_ID_LEN];
    static uint8_t aes_buf[200];
    uint8_t aes_key[16];
    uint8_t encry_device_id[DEVICE_ID_LEN];
    // uint8_t device_type;

    //scanrsp
    uint8_t scan_rsp_data[30];
    memcpy(&scan_rsp_data,scan_rsp_data_const,sizeof(scan_rsp_data_const));

    //device_type = (TUYA_BLE_DEVICE_TYPE&0x07)<<4;

    scan_rsp_data[4] &=(~0x70);
    //scan_rsp_data[4] |= device_type;

    if(tuya_ble_current_para.sys_settings.bound_flag == 1)
    {
        scan_rsp_data[4] |=0x80 ;
        //
        memcpy(aes_key,tuya_ble_current_para.sys_settings.login_key,LOGIN_KEY_LEN);
        memcpy(aes_key+LOGIN_KEY_LEN,tuya_ble_current_para.auth_settings.device_id,16-LOGIN_KEY_LEN);
//        ty_aes_encrypt_with_key(aes_key, tuya_para.auth_settings.device_id,DEVICE_ID_LEN,aes_buf);
//        memcpy(&(adv_data.scan_rsp_data.p_data[4]),aes_buf+1,PRODUCT_ID_LEN);
        tuya_ble_encrypt_old_with_key(aes_key,tuya_ble_current_para.auth_settings.device_id,DEVICE_ID_LEN,aes_buf);

        memcpy(&scan_rsp_data[6],(uint8_t *)(aes_buf+1),8);

        memset(aes_key,0,sizeof(aes_key));
        memcpy(aes_key,&scan_rsp_data[6],8);

        tuya_ble_device_id_encrypt(aes_key,8,tuya_ble_current_para.auth_settings.device_id,DEVICE_ID_LEN,encry_device_id);

        memcpy(&scan_rsp_data[6+8],encry_device_id,DEVICE_ID_LEN);

    }
    else
    {
        scan_rsp_data[4] &=(~0x80);
        memcpy(&scan_rsp_data[6],tuya_ble_current_para.pid,8);
        tuya_ble_device_id_encrypt(tuya_ble_current_para.pid,8,tuya_ble_current_para.auth_settings.device_id,DEVICE_ID_LEN,encry_device_id);

        memcpy(&scan_rsp_data[6+8],encry_device_id,DEVICE_ID_LEN);
    }
    TUYA_BLE_LOG("adv_change_bound_flag1-%d\n",tuya_ble_current_para.sys_settings.bound_flag);
//   tuya_ble_gap_advertising_adv_data_update(adv_data_const,sizeof(adv_data_const));
    tuya_ble_gap_advertising_scan_rsp_data_update(scan_rsp_data,sizeof(scan_rsp_data));

}

#endif



