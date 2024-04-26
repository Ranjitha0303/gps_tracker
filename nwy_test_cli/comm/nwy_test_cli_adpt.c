#include "nwy_test_cli_adpt.h"
#include "nwy_test_cli_utils.h"
#include "osi_log.h"
#include "nwy_sms.h"


extern void nwy_test_cli_menu_display();
extern void nwy_test_cli_menu_back();
extern void nwy_test_cli_menu_select(int opt);

static nwy_osi_thread_t nwy_test_cli_thread = NULL;

//for two case handling
//mcu_cutoff_handler_en_t mcu_cutoff_handler_en = send_10sec_en ;

char* nwy_test_spi_name_tab[NWY_OPEN_MAX_SPI_CHANNEL] = {NULL, NAME_SPI_BUS_2};
unsigned int nwy_test_uart_name_tab[NWY_OPEN_MAX_UART_CHANNEL] = {NWY_NAME_UART1, NWY_NAME_UART2, NWY_NAME_UART3};
uint32_t nwy_test_band_lock_tab[NWY_BAND_LOCK_NUM] = {NWY_OPEN_LOCK_AUTO, NWY_OPEN_LOCK_GSM, NWY_OPEN_LOCK_WCDMA, NWY_OPEN_LOCK_LTE};
uint32_t nwy_test_freq_lock_tab[NWY_FREQ_LOCK_NUM] = {NWY_OPEN_LOCK_GSM, NWY_OPEN_LOCK_WCDMA, NWY_OPEN_LOCK_LTE};
unsigned int nwy_test_del_sms_tab[NWY_OPEN_DEL_SMS_NUM] = {NWY_OPEN_DEL_SMS_READ, NWY_OPEN_DEL_SMS_READ_SEND, NWY_OPEN_DEL_SMS_READ_SEND_UNSEND, NWY_OPEN_DEL_SMS_ALL};
void nwy_test_cli_dbg(const char *func, int line, char *fmt, ...)
{
    static char buf[1024];
    va_list args;
    int len = 0;

    memset(buf, 0, sizeof(buf));

    sprintf(buf, "NWY_CLI %s[%d]:", func, line);
    len = strlen(buf);
    va_start(args, fmt);

    vsnprintf(&buf[len], sizeof(buf) - len - 1, fmt, args);
    va_end(args);

    OSI_LOGXI(OSI_LOGPAR_S, 0, "%s\n", buf);
}

int nwy_test_cli_wait_select()
{
    nwy_event_msg_t event;

    while (1)
    {
        memset(&event, 0, sizeof(event));
        nwy_wait_thread_event(nwy_test_cli_thread, &event, 0);
        if (event.id == NWY_EXT_INPUT_RECV_MSG)
        {
            return 1;
        }
    }
}

void nwy_test_cli_select_enter()
{
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));
    event.id = NWY_EXT_INPUT_RECV_MSG;
    nwy_send_thread_event(nwy_test_cli_thread, &event, 0);
}

void nwy_test_cli_send_trans_end()
{
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));
    event.id = NWY_EXT_DATA_REC_END_MSG;
    nwy_send_thread_event(nwy_test_cli_thread, &event, 0);
}

int nwy_test_cli_wait_trans_end()
{
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));
    nwy_wait_thread_event(nwy_test_cli_thread, &event, 0);
    if (event.id == NWY_EXT_DATA_REC_END_MSG)
    {
        return 1;
    }
    return 0;
}

static void nwy_test_cli_main_func(void *param)
{
    char *sptr = NULL;
    char info_string[100] ;
    nwy_usb_serial_reg_recv_cb((nwy_sio_recv_cb_t)nwy_test_cli_sio_data_proc);
    while(1)
    {
        nwy_test_cli_gpio_set_dirt();
        nwy_test_cli_gpio_set_val();
        nwy_sleep(10000);
        nwy_test_cli_gpio_set_val_1();
        nwy_sleep(10000);
    }
    // nwy_test_cli_get_sim_status();
    // nwy_test_cli_data_create();
    //nwy_test_cli_gnss_open_base();
    //nwy_test_cli_mqtt_connect();
    //while(1)
    //{
        // nwy_test_cli_gnss_open_base();
        // nwy_test_cli_mqtt_connect();
        // nwy_test_cli_mqtt_pub(); 
        //nwy_test_cli_gnss_open_base();
        //loc_info.latitude = 2345.988;
        //loc_info.longitude = 234.900;
        //sptr = sprintf(info_string,"%.lf,%.lf", loc_info.latitude, loc_info.longitude);
        // snprintf(info_string, sizeof(info_string), "%.6f %.6f", loc_info.latitude, loc_info.longitude);
        // nwy_test_cli_echo("\r\nMQTT string sent:%s",info_string);
        // nwy_test_cli_mqtt_pub();        
        // nwy_sleep(10000);
    //}
    // while (1)
    // {
    //     nwy_test_cli_menu_display();
    //     sptr = nwy_test_cli_input_gets("\r\nPlease input option: ");
    //     if (sptr[0] == 'q' || sptr[0] == 'Q')
    //     {
    //         nwy_test_cli_menu_back();
    //     }
    //     else
    //     {
    //         nwy_test_cli_menu_select(atoi(sptr));
    //     }
    // }
        // nwy_sleep(1000);
        
        // while(1)
        // {
        //     //nwy_test_cli_mqtt_connect();
        //     nwy_test_cli_mqtt_state();
        //     if(test_connect == 1)
        //     {
        //         nwy_test_cli_mqtt_pub();
        //         nwy_test_cli_echo("\r\ntest_connect == 1\r\n");
        //         nwy_sleep(10000);
        //     }
        //     else
        //     {
        //         nwy_test_cli_mqtt_connect();
        //         nwy_test_cli_mqtt_pub(); 
        //         nwy_test_cli_echo("\r\ntest_connect == 0\r\n"); 
        //         nwy_sleep(10000);              
        //     }
        // }

    //}

    // while(1)
    // {
    //     switch(mcu_cutoff_handler_en)
    // 	{
    // 		case send_10sec_en:
	// 		{
    //             nwy_test_cli_mqtt_state();
    //             if(test_connect == 1)
    //             {
    //                 nwy_test_cli_gnss_open_base();
    //                 nwy_test_cli_mqtt_pub();
    //                 //nwy_test_cli_echo("\r\ntest_connect == 1\r\n");
    //                 nwy_sleep(10000);
    //             }
    //             else
    //             {
    //                 nwy_test_cli_gnss_open_base();
    //                 nwy_test_cli_mqtt_connect();
    //                 nwy_test_cli_mqtt_pub(); 
    //                 //nwy_test_cli_echo("\r\ntest_connect == 0\r\n"); 
    //                 nwy_sleep(10000);              
    //             } 
	// 		}
    // 		case send_1min_en:
	// 		{
    //             nwy_test_cli_mqtt_state();
    //             if(test_connect == 1)
    //             {
    //                 nwy_test_cli_gnss_open_base();
    //                 nwy_test_cli_mqtt_pub();
    //                 //nwy_test_cli_echo("\r\ntest_connect == 1\r\n");
    //                 nwy_sleep(10000);
    //             }
    //             else
    //             {
    //                 nwy_test_cli_gnss_open_base();
    //                 nwy_test_cli_mqtt_connect();
    //                 nwy_test_cli_mqtt_pub(); 
    //                 //nwy_test_cli_echo("\r\ntest_connect == 0\r\n"); 
    //                 nwy_sleep(10000);              
    //             } 
	// 		}
    // 		default :
	// 		{
	// 			mcu_cutoff_handler_en = send_10sec_en ;
	// 			break;
	// 		}
    // 	}
    // }
}

static const char APP_VERSION[65] = "NWY_APP_V1.0.1";
int appimg_enter(void *param)
{
    nwy_sleep(10 * 1000);
	char APP_BUILD_TIME[65]= {0};
    char version[70]={0};

    sprintf(version,"\"%s\"", APP_VERSION);
    sprintf(APP_BUILD_TIME,"\"%s,%s\"", __DATE__,__TIME__);
    OSI_LOGI(0, "nwy_open_app_enter ...");
    if(false == nwy_app_version(version, APP_BUILD_TIME))
    {
      OSI_LOGI(0, "app set version fail");
      return 0;
    }
    nwy_init_sms_option();
    nwy_sms_reg_recv_cb((nwy_sms_recv_cb_t)nwy_sms_test_recv_cb);
    nwy_create_thread(&nwy_test_cli_thread, 1024 * 32, NWY_OSI_PRIORITY_NORMAL, "test-cli", nwy_test_cli_main_func, NULL, 16);
    return 0;
}

void appimg_exit(void)
{
    OSI_LOGI(0x100075cb, "application image exit");
}

void nwy_exit_thread(nwy_osi_thread_t hdl)
{
    return;
}

#ifdef NWY_OPEN_TEST_VIRT_AT
extern void nwy_cli_pull_out_sim(uint8 *data, int len);
extern void nwy_cli_recv_sms(uint8 *data, int len);
extern void nwy_cli_tcpsetup_cb(uint8 *data, int len);
extern void nwy_cli_tcprecvs_cb(uint8 *data, int len);
extern void nwy_cli_tcp_close_cb(uint8 *data, int len);
extern void nwy_cli_tcprecv_cb(uint8 *data, int len);
extern void nwy_cli_acpt_close_cb(char *data);
extern void nwy_cli_udprecv_cb(uint8 *data, int len);
extern void nwy_cli_client_acpt_cb(uint8 *data, int len);
extern void nwy_cli_gprs_disconnect_cb(uint8 *data, int len);
extern void nwy_cli_sms_list_resp_cb(uint8 *data, int len);
#ifdef FEATRUE_NWY_CLIP_RESP_CB_SUPPORT
extern void nwy_cli_clip_resp_cb(uint8 *data, int len);
#endif
extern void nwy_recv_sat_info_cb();
void nwy_cli_init_unsol_reg()
{
    nwy_sdk_at_unsolicited_cb_reg("+EUSIM", nwy_cli_pull_out_sim);
    /* added by wangchen for N58 sms api to test 20200215 begin */
    nwy_sdk_at_unsolicited_cb_reg("+CMT", nwy_cli_recv_sms);
    /* added by wangchen for N58 sms api to test 20200215 end */
    nwy_sdk_at_unsolicited_cb_reg("Connect AcceptSocket=", nwy_cli_client_acpt_cb);
    nwy_sdk_at_unsolicited_cb_reg("+TCPRECV(S): ", nwy_cli_tcprecvs_cb);
    nwy_sdk_at_unsolicited_cb_reg("+CLOSECLIENT: ", nwy_cli_acpt_close_cb);
    nwy_sdk_at_unsolicited_cb_reg("+TCPSETUP: ", nwy_cli_tcpsetup_cb);
    nwy_sdk_at_unsolicited_cb_reg("+TCPRECV: ", nwy_cli_tcprecv_cb);
    nwy_sdk_at_unsolicited_cb_reg("+TCPCLOSE: ", nwy_cli_tcp_close_cb);
    nwy_sdk_at_unsolicited_cb_reg("+UDPRECV: ", nwy_cli_udprecv_cb);
    nwy_sdk_at_unsolicited_cb_reg("GPRS DISCONNECTION", nwy_cli_gprs_disconnect_cb);
    nwy_sdk_at_unsolicited_cb_reg("+CMGL: ", nwy_cli_sms_list_resp_cb);
#ifdef FEATRUE_NWY_CLIP_RESP_CB_SUPPORT
    nwy_sdk_at_unsolicited_cb_reg("+CLIP: ", nwy_cli_clip_resp_cb);
#endif
    nwy_sdk_at_unsolicited_cb_reg("+STKPCI", nwy_recv_sat_info_cb);
}
#endif

int nwy_test_cli_check_uart_mode(uint8_t mode)
{
    return 0;
}

void _gpioisropen(void *param)
{
    NWY_CLI_LOG("nwy gpio isr set success");
}
