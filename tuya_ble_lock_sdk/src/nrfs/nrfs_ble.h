/**
****************************************************************************
* @file      nrfs_ble.h
* @brief     nrfs_ble
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __NRFS_BLE_H__
#define __NRFS_BLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "nrfs_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define NRFS_BLE_CONN_CFG_TAG            1

#define NRFS_MIN_CONN_INTERVAL               180 //最小可接受的连接间隔
#define NRFS_MAX_CONN_INTERVAL               200
#define NRFS_SLAVE_LATENCY                   0
#define NRFS_CONN_SUP_TIMEOUT                5000
#define NRFS_DEFAULT_CONN_PARAM              {NRFS_MIN_CONN_INTERVAL, NRFS_MAX_CONN_INTERVAL, NRFS_SLAVE_LATENCY, NRFS_CONN_SUP_TIMEOUT}

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint16_t cMin;
    uint16_t cMax;
    uint16_t latency;
    uint16_t timeout;
} nrfs_conn_param_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
ret_code_t nrfs_set_conn_param(void);
ret_code_t nrfs_conn_param_update(uint16_t cMin, uint16_t cMax, uint16_t latency, uint16_t timeout);
void nrfs_ble_stack_init(void);
void nrfs_gap_params_init(void);
void nrfs_gatt_init(void);

ret_code_t nrfs_disconnect(uint16_t conn_handle, uint8_t hci_status_code);
void nrfs_init_bt_mac_addr(void);
ret_code_t nrfs_set_bt_mac_addr(uint8_t *addr);
ret_code_t nrfs_get_bt_mac_addr(uint8_t *addr, uint8_t len);
uint16_t get_m_conn_handle(void);
uint16_t get_m_ble_max_data_len(void);


#ifdef __cplusplus
}
#endif

#endif //__NRFS_BLE_H__
