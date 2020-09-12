#include "nrfs_svc.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);

/*********************************************************************
 * LOCAL FUNCTION
 */
static void nus_data_handler(ble_nus_evt_t * p_evt);

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: service init
*/
void nrfs_svc_init(void)
{
    ret_code_t err_code;
    // Initialize NUS.
    ble_nus_init_t     nus_init;
    memset(&nus_init, 0, sizeof(nus_init));
    nus_init.data_handler = nus_data_handler;
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}

/*********************************************************
FN: 
*/
static void nus_data_handler(ble_nus_evt_t * p_evt)
{
    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        tuya_ble_gatt_receive_data((void*)p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
    }
}

/*********************************************************
FN: 作为从机向主机发送数据
*/
ret_code_t nrfs_svc_send_data(uint16_t conn_handle, uint8_t* buf, uint16_t size)
{
    ret_code_t err_code;
    uint32_t tmp_count = 100;
    do
    {
        err_code = ble_nus_data_send(&m_nus, buf, &size, conn_handle);
        if ((err_code != NRF_ERROR_INVALID_STATE) &&
            (err_code != NRF_ERROR_RESOURCES) &&
            (err_code != NRF_ERROR_NOT_FOUND) &&
            (err_code != NRF_SUCCESS))
        {
            NRFS_PRINTF("Error: ble nus send mtu error, err_code = %d\n", err_code);
            APP_ERROR_CHECK(err_code);
        }
        nrfs_delay_ms(5);
        tmp_count--;
    } while (err_code == NRF_ERROR_RESOURCES && tmp_count > 0);
    return err_code;
}




















