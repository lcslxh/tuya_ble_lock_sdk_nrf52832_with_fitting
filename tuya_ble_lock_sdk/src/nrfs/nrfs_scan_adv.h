/**
****************************************************************************
* @file      nrfs_scan_adv.h
* @brief     nrfs_scan_adv
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __NRFS_SCAN_ADV_H__
#define __NRFS_SCAN_ADV_H__

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
#define  NRFS_ADV_MAX_LEN        31
#define  DEFAULT_ADV_DATA_LEN    3
#define  DEFAULT_SCAN_RSP_LEN    5

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint16_t advInterval;
    uint16_t advDuration;
    uint8_t  advType;
    uint8_t  advPower;
    uint8_t  advPriPhy;
    uint8_t  advSecPhy;
//    uint8_t  advExtend;
} adv_param_t;

typedef struct
{
    uint16_t scanDur;
    uint8_t  maxRes;
    int8_t   minRssi;
    uint8_t  isFiltDuplicated;
    uint8_t  isActive;
} scan_param_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */
//Adv
extern bool                    g_isAdvtising;
extern adv_param_t             g_adv_param;
extern uint32_t                g_adv_data_len;
extern uint8_t                 g_adv_data[];
extern uint32_t                g_scan_rsp_len;
extern uint8_t                 g_scan_rsp[];

//Scan
extern bool                    g_is_scanning;
extern scan_param_t            g_scan_param;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
ret_code_t nrfs_adv_init(void);
ret_code_t nrfs_adv_start(void);
ret_code_t nrfs_adv_stop(void);
ret_code_t nrfs_adv_state_update(bool adv_state);
ret_code_t nrfs_adv_update_advDataAndScanRsp(void);
ret_code_t nrfs_adv_param_set(void);
ret_code_t nrfs_scan_init(void);
ret_code_t nrfs_scan_start(void);
ret_code_t nrfs_scan_stop(void);
ret_code_t nrfs_scan_param_set(void);


#ifdef __cplusplus
}
#endif

#endif //__NRFS_SCAN_ADV_H__
