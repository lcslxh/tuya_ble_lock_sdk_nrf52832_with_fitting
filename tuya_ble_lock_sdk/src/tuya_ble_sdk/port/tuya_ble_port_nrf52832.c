#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tuya_ble_port.h"
#include "tuya_ble_type.h"
#include "mbedtls/aes.h"
#include "mbedtls/md5.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "ble_nus.h"
#include "ble_advertising.h"
#include "nrf_drv_pwm.h"
#include "tuya_ble_internal_config.h"
#include "app_port.h"




/*********************************************************
FN: 
*/
static tuya_ble_status_t nrf_err_code_convert(uint32_t err_code)
{
    tuya_ble_status_t status;
    
    switch (err_code) 
    {
        case NRF_SUCCESS: {
            status = TUYA_BLE_SUCCESS;
        } break;
        
        case NRF_ERROR_INTERNAL: {
            status = TUYA_BLE_ERR_INTERNAL;
        } break;
        
        case NRF_ERROR_NOT_FOUND: {
            status = TUYA_BLE_ERR_NOT_FOUND;
        } break;
        
        case NRF_ERROR_NO_MEM: {
            status = TUYA_BLE_ERR_NO_MEM;
        } break;
        
        case NRF_ERROR_INVALID_ADDR: {
            status = TUYA_BLE_ERR_INVALID_ADDR;
        } break;
        
        case NRF_ERROR_INVALID_PARAM: {
            status = TUYA_BLE_ERR_INVALID_PARAM;
        } break;
        
        case NRF_ERROR_INVALID_STATE: {
            status = TUYA_BLE_ERR_INVALID_STATE;
        } break;
        
        case NRF_ERROR_INVALID_LENGTH: {
            status = TUYA_BLE_ERR_INVALID_LENGTH;
        } break;
        
        case NRF_ERROR_DATA_SIZE: {
            status = TUYA_BLE_ERR_DATA_SIZE;
        } break;
        
        case NRF_ERROR_BUSY: {
            status = TUYA_BLE_ERR_BUSY;
        } break;
        
        case NRF_ERROR_TIMEOUT: {
            status = TUYA_BLE_ERR_TIMEOUT;
        } break;
        
        default: {
            status = TUYA_BLE_ERR_UNKNOWN;
        } break;
    }
    return status;
}


/*********************************************************
FN: updata advertising data
*/
tuya_ble_status_t tuya_ble_gap_advertising_adv_data_update(uint8_t const* p_ad_data, uint8_t ad_len)
{
    ret_code_t err_code;
    g_adv_data_len = ad_len;
    memcpy(g_adv_data, p_ad_data, ad_len);
    
    err_code = nrfs_adv_update_advDataAndScanRsp();
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: updata scan response data
*/
tuya_ble_status_t tuya_ble_gap_advertising_scan_rsp_data_update(uint8_t const *p_sr_data, uint8_t sr_len)
{
    ret_code_t err_code;
    g_scan_rsp_len = sr_len;
    memcpy(g_scan_rsp, p_sr_data, sr_len);
    
    err_code = nrfs_adv_update_advDataAndScanRsp();
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: disconnect
*/
tuya_ble_status_t tuya_ble_gap_disconnect(void)
{
    ret_code_t err_code;
    uint16_t conn_handle = get_m_conn_handle();
    err_code = nrfs_disconnect(conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: get bt mac
*/
tuya_ble_status_t tuya_ble_gap_address_get(tuya_ble_addr_t mac)
{
    ret_code_t err_code;
    err_code = nrfs_get_bt_mac_addr(mac, BLE_GAP_ADDR_LEN);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: send data to phone
*/
tuya_ble_status_t tuya_ble_gatt_send_data(const uint8_t *p_data, uint16_t len)
{
    ret_code_t err_code;
    uint8_t data_len = len;
    if(data_len > TUYA_BLE_DATA_MTU_MAX)
    {
        data_len = TUYA_BLE_DATA_MTU_MAX;
    }
    
    uint16_t conn_handle = get_m_conn_handle();
    err_code = nrfs_svc_send_data(conn_handle, (void*)p_data, data_len);
    return nrf_err_code_convert(err_code);
}


/*********************************************************
FN: uart init
*/
tuya_ble_status_t tuya_ble_uart_init(void)
{
    //nrfs_uart_init();
    return TUYA_BLE_SUCCESS;
}

/*********************************************************
FN: uart send data
*/
tuya_ble_status_t tuya_ble_uart_send_data(const uint8_t *p_data, uint16_t len)
{
    ret_code_t err_code;
    err_code = nrfs_uart_send(p_data, len);
    return nrf_err_code_convert(err_code);
}


/*********************************************************
FN: timer creat
*/
tuya_ble_status_t tuya_ble_timer_create(void** p_timer_id, uint32_t timeout_value_ms, tuya_ble_timer_mode mode, tuya_ble_timer_handler_t timeout_handler)
{
    ret_code_t err_code;
    nrfs_timer_create(p_timer_id, timeout_value_ms, (app_timer_mode_t)mode, timeout_handler);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_timer_delete(void* timer_id)
{
    ret_code_t err_code;
    err_code = nrfs_timer_delete(timer_id);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_timer_start(void* timer_id)
{
    ret_code_t err_code;
    err_code = nrfs_timer_start(timer_id);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_timer_stop(void* timer_id)
{
    ret_code_t err_code;
    err_code = nrfs_timer_stop(timer_id);
    return nrf_err_code_convert(err_code);
}


/*********************************************************
FN: 
*/
void tuya_ble_device_delay_ms(uint32_t ms)
{
    nrfs_delay_ms(ms);
}

/*********************************************************
FN: 
*/
void tuya_ble_device_delay_us(uint32_t us)
{
    nrfs_delay_us(us);
}

/*********************************************************
FN: reset
*/
tuya_ble_status_t tuya_ble_device_reset(void)
{
    nrfs_system_reset();
    return TUYA_BLE_SUCCESS;
}

/*********************************************************
FN: 
*/
void tuya_ble_device_enter_critical(void)
{
   nrfs_enter_critical();
}

/*********************************************************
FN: 
*/
void tuya_ble_device_exit_critical(void)
{
   nrfs_exit_critical();
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_rand_generator(uint8_t* p_buf, uint8_t len)
{
    uint32_t cnt=len/4;
    uint8_t  remain = len%4;
    int32_t temp;
    for(uint32_t i=0; i<cnt; i++)
    {
        temp = rand();
        memcpy(p_buf,(uint8_t *)&temp,4);
        p_buf += 4;
    }
    temp = rand();
    memcpy(p_buf,(uint8_t *)&temp,remain);

    return TUYA_BLE_SUCCESS;
}


/*********************************************************
FN: not use now
*/
tuya_ble_status_t tuya_ble_rtc_get_timestamp(uint32_t *timestamp, int32_t *timezone)
{
    *timestamp = 0;
    *timezone = 0;
    return TUYA_BLE_SUCCESS;
}

/*********************************************************
FN: not use now
*/
tuya_ble_status_t tuya_ble_rtc_set_timestamp(uint32_t timestamp, int32_t timezone)
{
    return TUYA_BLE_SUCCESS;
}


#if (TUYA_BLE_DEVICE_REGISTER_FROM_BLE)

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_nv_init(void)
{
//    nrfs_flash_init();
    return TUYA_BLE_SUCCESS;
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_nv_erase(uint32_t addr, uint32_t size)
{
    ret_code_t err_code;
    
    uint32_t erase_pages = size / TUYA_NV_ERASE_MIN_SIZE;
    if (size % TUYA_NV_ERASE_MIN_SIZE != 0) {
        erase_pages++;
    }
    
    err_code =  nrfs_flash_erase(addr, erase_pages, true);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_nv_write(uint32_t addr, const uint8_t* p_data, uint32_t size)
{
    ret_code_t err_code;
    err_code =  nrfs_flash_write(addr, (void*)p_data, size, true);
    return nrf_err_code_convert(err_code);
}

/*********************************************************
FN: 
*/
tuya_ble_status_t tuya_ble_nv_read(uint32_t addr, uint8_t* p_data, uint32_t size)
{
    ret_code_t err_code;
    err_code =  nrfs_flash_read(addr, (void*)p_data, size);
    return nrf_err_code_convert(err_code);
}

#endif


#if TUYA_BLE_USE_OS

bool tuya_ble_os_task_create(void **pp_handle, const char *p_name, void (*p_routine)(void *),void *p_param, uint16_t stack_size, uint16_t priority)
{
    return os_task_create(pp_handle, p_name, p_routine,p_param, stack_size, priority);
}

bool tuya_ble_os_task_delete(void *p_handle)
{
    return os_task_delete(p_handle);
}

bool tuya_ble_os_task_suspend(void *p_handle)
{
    return os_task_suspend(p_handle);
}

bool tuya_ble_os_task_resume(void *p_handle)
{
    return os_task_resume(p_handle);
}

bool tuya_ble_os_msg_queue_create(void **pp_handle, uint32_t msg_num, uint32_t msg_size)
{
    return os_msg_queue_create(pp_handle, msg_num, msg_size);
}

bool tuya_ble_os_msg_queue_delete(void *p_handle)
{
    return os_msg_queue_delete(p_handle);
}

bool tuya_ble_os_msg_queue_peek(void *p_handle, uint32_t *p_msg_num)
{
    return os_msg_queue_peek(p_handle, p_msg_num);
}

bool tuya_ble_os_msg_queue_send(void *p_handle, void *p_msg, uint32_t wait_ms)
{
    return os_msg_send(p_handle, p_msg, wait_ms);
}

bool tuya_ble_os_msg_queue_recv(void *p_handle, void *p_msg, uint32_t wait_ms)
{
    return os_msg_recv(p_handle, p_msg, wait_ms);
}

#endif


/*********************************************************
FN: 
*/
bool tuya_ble_aes128_ecb_encrypt(uint8_t *key,uint8_t *input,uint16_t input_len,uint8_t *output)
{
    uint16_t length;
    mbedtls_aes_context aes_ctx;
    //
    if(input_len%16)
    {
        return false;
    }

    length = input_len;

    mbedtls_aes_init(&aes_ctx);

    mbedtls_aes_setkey_enc(&aes_ctx, key, 128);

    while( length > 0 )
    {
        mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, input, output );
        input  += 16;
        output += 16;
        length -= 16;
    }

    mbedtls_aes_free(&aes_ctx);

    return true;
}

/*********************************************************
FN: 
*/
bool tuya_ble_aes128_ecb_decrypt(uint8_t *key,uint8_t *input,uint16_t input_len,uint8_t *output)
{
    uint16_t length;
    mbedtls_aes_context aes_ctx;
    //
    if(input_len%16)
    {
        return false;
    }

    length = input_len;

    mbedtls_aes_init(&aes_ctx);

    mbedtls_aes_setkey_dec(&aes_ctx, key, 128);

    while( length > 0 )
    {
        mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, input, output );
        input  += 16;
        output += 16;
        length -= 16;
    }

    mbedtls_aes_free(&aes_ctx);

    return true;
}

/*********************************************************
FN: 
*/
bool tuya_ble_aes128_cbc_encrypt(uint8_t *key,uint8_t *iv,uint8_t *input,uint16_t input_len,uint8_t *output)
{
    mbedtls_aes_context aes_ctx;
    //
    if(input_len%16)
    {
        return false;
    }

    mbedtls_aes_init(&aes_ctx);

    mbedtls_aes_setkey_enc(&aes_ctx, key, 128);
    
    mbedtls_aes_crypt_cbc(&aes_ctx,MBEDTLS_AES_ENCRYPT,input_len,iv,input,output);
    //
    mbedtls_aes_free(&aes_ctx);

    return true;
}

/*********************************************************
FN: 
*/
bool tuya_ble_aes128_cbc_decrypt(uint8_t *key,uint8_t *iv,uint8_t *input,uint16_t input_len,uint8_t *output)
{
    mbedtls_aes_context aes_ctx;
    //
    if(input_len%16)
    {
        return false;
    }

    mbedtls_aes_init(&aes_ctx);

    mbedtls_aes_setkey_dec(&aes_ctx, key, 128);
    
    mbedtls_aes_crypt_cbc(&aes_ctx,MBEDTLS_AES_DECRYPT,input_len,iv,input,output);
    //
    mbedtls_aes_free(&aes_ctx);

    return true;
}

/*********************************************************
FN: 
*/
bool tuya_ble_md5_crypt(uint8_t *input,uint16_t input_len,uint8_t *output)
{
    mbedtls_md5_context md5_ctx;
    mbedtls_md5_init(&md5_ctx);
    mbedtls_md5_starts(&md5_ctx);
    mbedtls_md5_update(&md5_ctx, input, input_len);
    mbedtls_md5_finish(&md5_ctx, output);
    mbedtls_md5_free(&md5_ctx);    
    
    return true;
}


/*********************************************************
FN: 
*/
void tuya_ble_app_production_test_process(uint8_t* p_in_data, uint16_t in_len, uint8_t* p_out_data, uint16_t* out_len)
{
    app_port_factory_test_process(p_in_data, in_len, p_out_data, out_len);
}













