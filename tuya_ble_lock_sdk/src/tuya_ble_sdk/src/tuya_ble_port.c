/**
 * \file tuya_ble_port.c
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

#include "tuya_ble_port.h"
#include "tuya_ble_type.h"
#include "tuya_ble_internal_config.h"

/*
 *@brief  Start advertising with params set by @ref tuya_ble_gap_adv_data_set.  
 *@param    
 *@return Operation result.
 *@retval TUYA_BLE_SUCCESS Start request success.
 *@retval other Start request failed.        
 *@note     
 *           
 * */
 /*
__WEAK tuya_ble_status_t tuya_ble_gap_adv_start(void)
{
	return TUYA_BLE_SUCCESS;
}
*/
/*
 *@brief    
 *@param    
 *        
 *@note
 
static const uint8_t adv_data[] =
{
    0x02,             // length 
    GAP_ADTYPE_FLAGS, // type="Flags" 
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
    
    0x0B,             // length 
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,// Local name 
    'I', 'P', 'C', '_', 'C', 'A', 'M', 'E', 'R', 'A',
    0x03,
    GAP_ADTYPE_16BIT_MORE,
    0x00, 0xA3,
}; 

static const uint8_t scan_rsp_data[] =
{    
    0x0E,                             // length 
    GAP_ADTYPE_MANUFACTURER_SPECIFIC, // Manufacturer Specific Data 
    HI_WORD(COMPANY_ID),
    LO_WORD(COMPANY_ID),
    0x00,0x02,
    LO_WORD(DEVICE_TYPE),
    'I', 'P', 'C','C', 'A', 'M', 'E', 'R',
    
};
 
 *           
 * */
 /*
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_adv_data_set(uint8_t const * p_ad_data, uint8_t ad_len,uint8_t const *p_sr_data, uint8_t sr_len)
{
	return TUYA_BLE_SUCCESS;
}
*/
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_advertising_adv_data_update(uint8_t const * p_ad_data, uint8_t ad_len)
{
	return TUYA_BLE_SUCCESS;
}

__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_advertising_scan_rsp_data_update(uint8_t const *p_sr_data, uint8_t sr_len)
{
	return TUYA_BLE_SUCCESS;
}

/*
 *@brief  Stop advertising.  
 *@param    
 *        
 *@note     
 *           
 * */
/*
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_adv_stop(void)
{
	return TUYA_BLE_SUCCESS;
}
*/
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
/*
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_update_conn_params(tuya_ble_gap_conn_param_t conn_params)
{
	return TUYA_BLE_SUCCESS;
}
*/
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_disconnect(void)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gap_address_get(tuya_ble_addr_t mac)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_gatt_send_data(const uint8_t *p_data,uint16_t len)
{
	return TUYA_BLE_SUCCESS;
}

 
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */ 
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_timer_create(void** p_timer_id,uint32_t timeout_value_ms, tuya_ble_timer_mode mode,tuya_ble_timer_handler_t timeout_handler)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_timer_delete(void* timer_id)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_timer_start(void* timer_id)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_timer_stop(void* timer_id)
{
	return TUYA_BLE_SUCCESS;
}

/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_rand_generator(uint8_t* p_buf, uint8_t len)
{
	return TUYA_BLE_SUCCESS;
}
 
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_device_reset(void)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK void tuya_ble_device_enter_critical(void)
{
	
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK void tuya_ble_device_exit_critical(void)
{
	
}
 
 /*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_rtc_get_timestamp(uint32_t *timestamp,int32_t *timezone)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_rtc_set_timestamp(uint32_t timestamp,int32_t timezone)
{
	return TUYA_BLE_SUCCESS;
}

#if (TUYA_BLE_DEVICE_REGISTER_FROM_BLE)
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_nv_init(void)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_nv_erase(uint32_t addr,uint32_t size)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_nv_write(uint32_t addr,const uint8_t *p_data, uint32_t size)
{
	return TUYA_BLE_SUCCESS;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_nv_read(uint32_t addr,uint8_t *p_data, uint32_t size)
{
	return TUYA_BLE_SUCCESS;
}
#endif


/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_uart_init(void)
{
    return TUYA_BLE_SUCCESS;
}

/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK tuya_ble_status_t tuya_ble_uart_send_data(const uint8_t *p_data,uint16_t len)
{
    return TUYA_BLE_SUCCESS;
}


#if TUYA_BLE_USE_OS

/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_task_create(void **pp_handle, const char *p_name, void (*p_routine)(void *),void *p_param, uint16_t stack_size, uint16_t priority)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_task_delete(void *p_handle)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_task_suspend(void *p_handle)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_task_resume(void *p_handle)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_msg_queue_create(void **pp_handle, uint32_t msg_num, uint32_t msg_size)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_msg_queue_delete(void *p_handle)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_msg_queue_peek(void *p_handle, uint32_t *p_msg_num)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_msg_queue_send(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	return true;
}
/*
 *@brief    
 *@param    
 *        
 *@note     
 *           
 * */
__TUYA_BLE_WEAK bool tuya_ble_os_msg_queue_recv(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	return true;
}

#endif


/**
    * @brief  128 bit AES ECB encryption on speicified plaintext and keys
    * @param  input    specifed plain text to be encypted
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @param  key          keys to encrypt the plaintext
    * @param  output    output buffer to store encrypted data
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
    */
__TUYA_BLE_WEAK bool tuya_ble_aes128_ecb_encrypt(uint8_t *key,uint8_t *input,uint16_t input_len,uint8_t *output)
{
	return true;
}

/**
    * @brief  128 bit AES ECB decryption on speicified encrypted data and keys
    * @param  input    specifed encypted data to be decypted
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
*/
__TUYA_BLE_WEAK bool tuya_ble_aes128_ecb_decrypt(uint8_t *key,uint8_t *input,uint16_t input_len,uint8_t *output)
{
	return true;
}
/**
    * @brief  128 bit AES CBC encryption on speicified plaintext and keys
    * @param  input    specifed plain text to be encypted
    * @param  key          keys to encrypt the plaintext
    * @param  output    output buffer to store encrypted data
    * @param  iv         initialization vector (IV) for CBC mode
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
    */
__TUYA_BLE_WEAK bool tuya_ble_aes128_cbc_encrypt(uint8_t *key,uint8_t *iv,uint8_t *input,uint16_t input_len,uint8_t *output)
{
	return true;
}
/**
    * @brief  128 bit AES CBC descryption on speicified plaintext and keys
    * @param  input    specifed encypted data to be decypted
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @param  iv         initialization vector (IV) for CBC mode
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
    */
__TUYA_BLE_WEAK bool tuya_ble_aes128_cbc_decrypt(uint8_t *key,uint8_t *iv,uint8_t *input,uint16_t input_len,uint8_t *output)
{
	return true;
}
/**
    * @brief  MD5 checksum 
    * @param  input    specifed plain text to be encypted
    * @param  output    output buffer to store md5 result data,output data len is always 16
    * @param  input_len    byte length of the data to be encypted
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   
    */
__TUYA_BLE_WEAK bool tuya_ble_md5_crypt(uint8_t *input,uint16_t input_len,uint8_t *output)
{
	return true;
}




