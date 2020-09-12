#include "nrfs_scan_adv.h"
#include "app_port.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
//广播和扫描响应数据
#define     DEFAULT_ADV_DATA            \
            {                           \
                0x02,                   \
                BLE_GAP_AD_TYPE_FLAGS,  \
                BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE|BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED \
            }
            
#define     DEFAULT_SCAN_RSP            \
            {                           \
                0x04,                   \
                BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, \
                'L',                    \
                'K',                    \
                'E'                     \
            }

//广播和扫描参数 //1285
#define     DEFAULT_ADV_PARAM           \
            {                           \
                .advInterval = APP_PORT_ADV_INTERVAL,    \
                .advDuration = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED, \
                .advType     = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED, \
                .advPower    = 0x00,    \
                .advPriPhy   = 0x01,    \
                .advSecPhy   = 0x01,    \
            }
            
#define     DEFAULT_SCAN_PARAM          \
            {                           \
                .scanDur     = 0x01F4,  \
                .maxRes      = 0x0A,    \
                .minRssi     = 0xCE,    \
                .isFiltDuplicated = 0x01, \
                .isActive    = 0x01,    \
            }

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
//Scan
NRF_BLE_SCAN_DEF(m_scan);

//Adv
static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;
static ble_gap_adv_params_t m_adv_params = {0};
static ble_gap_adv_data_t   m_adv_data =
{
    .adv_data =
    {
        .p_data = g_adv_data,
        .len    = DEFAULT_ADV_DATA_LEN,
    },
    .scan_rsp_data =
    {
        .p_data = g_scan_rsp,
        .len    = DEFAULT_SCAN_RSP_LEN,
    }
};
//Scan
static ble_gap_scan_params_t m_scan_param = {0};

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */
//Adv
bool                    g_isAdvtising                  = false;
adv_param_t             g_adv_param                    = DEFAULT_ADV_PARAM;
uint32_t                g_adv_data_len                 = DEFAULT_ADV_DATA_LEN;
uint8_t                 g_adv_data[NRFS_ADV_MAX_LEN]   = DEFAULT_ADV_DATA;
uint32_t                g_scan_rsp_len                 = DEFAULT_SCAN_RSP_LEN;
uint8_t                 g_scan_rsp[NRFS_ADV_MAX_LEN]   = DEFAULT_SCAN_RSP;

//Scan
bool                    g_is_scanning                  = false;
scan_param_t            g_scan_param                   = DEFAULT_SCAN_PARAM;




/*********************************************************
FN: 初始化广播
*/
ret_code_t nrfs_adv_init(void)
{
    ret_code_t err_code;

    m_adv_params.interval = MSEC_TO_UNITS(g_adv_param.advInterval, UNIT_0_625_MS);
    m_adv_params.duration = MSEC_TO_UNITS(g_adv_param.advDuration, UNIT_10_MS);
    m_adv_params.properties.type = g_adv_param.advType;
    m_adv_params.primary_phy = g_adv_param.advPriPhy;
    m_adv_params.secondary_phy = g_adv_param.advSecPhy;
    m_adv_params.filter_policy = BLE_GAP_ADV_FP_ANY;

    m_adv_data.adv_data.len = g_adv_data_len;
    m_adv_data.adv_data.p_data = g_adv_data;
    m_adv_data.scan_rsp_data.len = g_scan_rsp_len;
    m_adv_data.scan_rsp_data.p_data = g_scan_rsp;

    err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &m_adv_params);
    APP_ERROR_CHECK(err_code);
    
    return err_code;
}

/*********************************************************
FN: 启动广播
*/
ret_code_t nrfs_adv_start(void)
{
    ret_code_t err_code;
    if(g_isAdvtising == false)
    {
        err_code = sd_ble_gap_adv_start(m_adv_handle, NRFS_BLE_CONN_CFG_TAG);
        APP_ERROR_CHECK(err_code);
        if(err_code == NRF_SUCCESS)
        {
            NRFS_PRINTF("adv start");
            g_isAdvtising = true;
        }
    }
    else
    {
//        NRFS_PRINTF("Error: Device is advertising");
        err_code = NRF_ERROR_INVALID_STATE;
    }
    return err_code;
}

/*********************************************************
FN: 停止广播
*/
ret_code_t nrfs_adv_stop(void)
{
    ret_code_t err_code;
    if(g_isAdvtising == true)
    {
        err_code = sd_ble_gap_adv_stop(m_adv_handle);
        APP_ERROR_CHECK(err_code);
        if(err_code == NRF_SUCCESS)
        {
            NRFS_PRINTF("adv stop");
            g_isAdvtising = false;
        }
    }
    else
    {
//        NRFS_PRINTF("Error: Device is not advertising");
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}

/*********************************************************
FN: 更新广播状态
*/
ret_code_t nrfs_adv_state_update(bool adv_state)
{
    ret_code_t err_code;
    g_isAdvtising = adv_state;

    return err_code;
}

/*********************************************************
FN: 更新广播和扫描响应数据
*/
ret_code_t nrfs_adv_update_advDataAndScanRsp(void)
{
    ret_code_t err_code;
    bool adv_flag = false;

    if(g_isAdvtising)
    {
        adv_flag = true;
        //停止广播
        err_code = nrfs_adv_stop();
        APP_ERROR_CHECK(err_code);
        if(err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    m_adv_data.adv_data.len = g_adv_data_len;
    m_adv_data.adv_data.p_data = g_adv_data;
    m_adv_data.scan_rsp_data.len = g_scan_rsp_len;
    m_adv_data.scan_rsp_data.p_data = g_scan_rsp;

    err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, NULL);
    APP_ERROR_CHECK(err_code);
    if(err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    if(adv_flag)
    {
        //启动广播
        err_code = nrfs_adv_start();
        APP_ERROR_CHECK(err_code);
    }

    return err_code;
}

/*********************************************************
FN: 设置广播参数
*/
ret_code_t nrfs_adv_param_set(void)
{
    ret_code_t err_code;
    bool adv_flag = false;

    if(g_isAdvtising)
    {
        adv_flag = true;
        //停止广播
        err_code = nrfs_adv_stop();
        APP_ERROR_CHECK(err_code);
        if(err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    m_adv_params.interval = MSEC_TO_UNITS(g_adv_param.advInterval, UNIT_0_625_MS);
    m_adv_params.duration = MSEC_TO_UNITS(g_adv_param.advDuration, UNIT_10_MS);
    m_adv_params.properties.type = g_adv_param.advType;
    m_adv_params.primary_phy = g_adv_param.advPriPhy;
    m_adv_params.secondary_phy = g_adv_param.advSecPhy;
    m_adv_params.filter_policy = BLE_GAP_ADV_FP_ANY;

    err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, NULL, &m_adv_params);
    APP_ERROR_CHECK(err_code);
    if(err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    if(adv_flag)
    {
        //启动广播
        err_code = nrfs_adv_start();
        APP_ERROR_CHECK(err_code);
    }

    return err_code;
}








/*********************************************************
FN: 处理扫描事件
*/
static void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    ret_code_t err_code;

    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_FILTER_MATCH:
        {
            NRFS_PRINTF("NRF_BLE_SCAN_EVT_FILTER_MATCH");
        } 
        break;

        case NRF_BLE_SCAN_EVT_WHITELIST_REQUEST:
        {
            NRFS_PRINTF("NRF_BLE_SCAN_EVT_WHITELIST_REQUEST");
        } 
        break;

        case NRF_BLE_SCAN_EVT_WHITELIST_ADV_REPORT:
        {
            NRFS_PRINTF("NRF_BLE_SCAN_EVT_WHITELIST_ADV_REPORT");
        } 
        break;

        case NRF_BLE_SCAN_EVT_NOT_FOUND:
        {
            ble_gap_evt_adv_report_t const* adv = p_scan_evt->params.p_not_found;

            if(adv->rssi > g_scan_param.minRssi)
            {
            }
        } 
        break;

        case NRF_BLE_SCAN_EVT_SCAN_TIMEOUT:
        {
//            err_code = nrfs_adv_start();
//            APP_ERROR_CHECK(err_code);
        }
        break;

        case NRF_BLE_SCAN_EVT_SCAN_REQ_REPORT:
        {
            NRFS_PRINTF("NRF_BLE_SCAN_EVT_SCAN_REQ_REPORT");
        }
        break;

        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR:
        {
            err_code = p_scan_evt->params.connecting_err.err_code;
            APP_ERROR_CHECK(err_code);
        } 
        break;

        default:
        {
        }
        break;
    }
}

/*********************************************************
FN: 扫描初始化
*/
ret_code_t nrfs_scan_init(void)
{
    ret_code_t err_code;

    nrf_ble_scan_init_t init_scan;
    memset(&init_scan, 0, sizeof(init_scan));
    init_scan.conn_cfg_tag     = NRFS_BLE_CONN_CFG_TAG;
    err_code = nrf_ble_scan_init(&m_scan, &init_scan, scan_evt_handler);
    APP_ERROR_CHECK(err_code);
    
    m_scan_param.active = g_scan_param.isActive;
    m_scan_param.interval = BLE_GAP_SCAN_INTERVAL_MAX;
    m_scan_param.window = BLE_GAP_SCAN_WINDOW_MAX;
    m_scan_param.timeout = g_scan_param.scanDur;
    err_code = nrf_ble_scan_params_set(&m_scan, &m_scan_param);
    APP_ERROR_CHECK(err_code);
    
    return err_code;
}

/*********************************************************
FN: 启动扫描
*/
ret_code_t nrfs_scan_start(void)
{
    ret_code_t err_code;

    err_code = nrfs_adv_stop();
    APP_ERROR_CHECK(err_code);
    if(err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(err_code);
    if(err_code == NRF_SUCCESS)
    {
        g_is_scanning = true;
    }

    return err_code;
}

/*********************************************************
FN: 停止扫描
*/
ret_code_t nrfs_scan_stop(void)
{
    nrf_ble_scan_stop();

    g_is_scanning = false;

    return NRF_SUCCESS;
}

/*********************************************************
FN: 设置扫描参数
*/
ret_code_t nrfs_scan_param_set(void)
{
    ret_code_t err_code;
    m_scan_param.active = g_scan_param.isActive;
    m_scan_param.interval = BLE_GAP_SCAN_INTERVAL_MAX;
    m_scan_param.window = BLE_GAP_SCAN_WINDOW_MAX;
    m_scan_param.timeout = MSEC_TO_UNITS(g_scan_param.scanDur, UNIT_10_MS);
    err_code = nrf_ble_scan_params_set(&m_scan, &m_scan_param);
    APP_ERROR_CHECK(err_code);

    return err_code;
}



































