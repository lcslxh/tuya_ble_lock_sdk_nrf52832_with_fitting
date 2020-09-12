/**
****************************************************************************
* @file      nrfs_timer.h
* @brief     nrfs_timer
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __NRFS_TIMER_H__
#define __NRFS_TIMER_H__

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

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
ret_code_t nrfs_timer_create(void** p_timer_id, uint32_t timeout_value_ms, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
ret_code_t nrfs_timer_delete(app_timer_id_t timer_id);
ret_code_t nrfs_timer_start(app_timer_id_t timer_id);
ret_code_t nrfs_timer_stop(app_timer_id_t timer_id);

/*********************************************************  RTC 2  *********************************************************/
void nrfs_local_timer_start(void);
void nrfs_update_timestamp(uint32_t app_timestamp);
uint32_t nrfs_get_app_timestamp_when_update(void);
uint32_t nrfs_get_local_timestamp(void);
uint32_t nrfs_get_timestamp(void);
uint32_t nrfs_get_old_timestamp(uint32_t old_local_timestamp);

/*********************************************************  delay  *********************************************************/
void nrfs_delay_ms(uint32_t ms);
void nrfs_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif //__NRFS_TIMER_H__
