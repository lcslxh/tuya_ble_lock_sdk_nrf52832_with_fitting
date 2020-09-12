/**
****************************************************************************
* @file      nrfs_uart.h
* @brief     nrfs_uart
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __NRFS_UART_H__
#define __NRFS_UART_H__

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
void nrfs_uart_init(void);
ret_code_t nrfs_uart_send(const uint8_t* buf, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif //__NRFS_UART_H__
