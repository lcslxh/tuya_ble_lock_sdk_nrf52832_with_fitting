#include "nrfs_common.h"
#include "nrf_drv_wdt.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void nrfs_log_init(void)
{
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_ERROR,   ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_WARN,    ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_INFO,    ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG,   ELOG_FMT_LVL);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL);
    /* start EasyLogger */
    elog_start();
}

/*********************************************************
FN: 
*/
void nrfs_log_hexdump(const char *name, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, 8, buf, size);
}

/*********************************************************
FN: 
*/
void nrfs_log_hexdump_for_tuya_ble_sdk(const char *name, uint8_t width, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, width, buf, size);
}

/*********************************************************
FN: 
*/
void nrfs_log_hexdump_empty(const char *name, uint8_t *buf, uint16_t size)
{
    //empty
}

/*********************************************************
FN: 
*/
void nrfs_system_reset(void)
{
    NRF_LOG_FINAL_FLUSH();
    NVIC_SystemReset();
}

/*********************************************************
FN: 
*/
void nrfs_enter_critical(void)
{
   CRITICAL_REGION_ENTER();
}

/*********************************************************
FN: 
*/
void nrfs_exit_critical(void)
{
   CRITICAL_REGION_EXIT();
}




static nrf_drv_wdt_channel_id m_channel_id;

/*********************************************************
FN: 
*/
static void wdt_event_handler(void)
{
    NRFS_PRINTF("123456789");
    //NOTE: The max amount of time we can spend in WDT interrupt is two cycles of 32768[Hz] clock - after that, reset occurs
}

/*********************************************************
FN: 
*/
void nrfs_wdt_init(void)
{
    uint32_t err_code = NRF_SUCCESS;
    //Configure WDT.
    nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
    err_code = nrf_drv_wdt_init(&config, wdt_event_handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
    APP_ERROR_CHECK(err_code);
    nrf_drv_wdt_enable();
}

/*********************************************************
FN: 
*/
void nrfs_wdt_feed(void)
{
    nrf_drv_wdt_channel_feed(m_channel_id);
}




















