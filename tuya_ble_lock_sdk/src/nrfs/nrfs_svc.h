/**
****************************************************************************
* @file      nrfs_svc.h
* @brief     nrfs_svc
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __NRFS_SVC_H__
#define __NRFS_SVC_H__

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
void nrfs_svc_init(void);
ret_code_t nrfs_svc_send_data(uint16_t conn_handle, uint8_t* buf, uint16_t size);


#ifdef __cplusplus
}
#endif

#endif //__NRFS_SVC_H__
