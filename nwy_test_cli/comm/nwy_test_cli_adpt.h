#ifndef __NWY_TEST_CLI_ADPT_H__
#define __NWY_TEST_CLI_ADPT_H__

#include "nwy_osi_api.h"
#include "nwy_usb_serial.h"
#include "nwy_test_cli_utils.h"
#include "nwy_data.h"
#include "nwy_open_socket.h"
#include "nwy_ftp.h"
#ifdef FEATURE_NWY_FTP_SSL_ENABLE
#include "nwy_ftps.h"
#endif
#include "nwy_file.h"
#include "nwy_http.h"
#include "nwy_md5.h"
#include "nwy_pm.h"
#include "nwy_dm.h"
#include "nwy_sms.h"
#include "nwy_vir_at.h"
#include "nwy_network.h"
#include "nwy_sim.h"
#include "nwy_uart.h"
#include "nwy_spi.h"
#include "nwy_gpio_open.h"
#include "nwy_adc.h"
#include "nwy_wifi.h"
#include "nwy_fota.h"
#include "nwy_lbs_ext.h"

#ifdef FEATURE_NWY_CAMERA_APP
#include "nwy_camera.h"
#endif
#ifdef CONFIG_SUPPORT_GNSS
#include "nwy_loc.h"
#endif
#ifdef CONFIG_NWY_SUPPORT_GNSS_EXTERNAL_CHIP
#include "nwy_loc_external.h"
#endif
#include "nwy_config.h"
#include "nwy_audio_api.h"
#ifdef FEATURE_NWY_DUAL_SIM
#include "nwy_poc_dsds.h"
#endif
#include "nwy_voice.h"
#include "nwy_sat.h"
#define NWY_OPEN_CLI_UIS8850
#define NWY_OPEN_TEST_KEYPAD
#define NWY_OPEN_TEST_VIRT_AT
#define NWY_OPEN_TEST_SMS
#define NWY_OPEN_TEST_AT_FWD
#define NWY_OPEN_TEST_HEAP_INFO
#define NWY_OPEN_TEST_CPU_TEMP
#define NWY_OPEN_TEST_SPI
#define NWY_OPEN_TEST_SPI_CFG
#define NWY_OPEN_TEST_ADC
#define NWY_OPEN_TEST_GENERAL_FLASH_MOUNT
#define NWY_OPEN_TEST_GENERAL_FLASH_DATA
//begin:add by wanglongzhen for UART callback register
#define FEATURE_NWY_OPEN_UART_CB
//end:add by wanglongzhen for UART callback register
#ifdef NWY_OPEN_TEST_LITE_RWS
#include "nwy_rws_proc.h"
#endif
#ifdef CONFIG_AUDIO_ENABLE
#define NWY_OPEN_TEST_AUDIO
#endif
#ifdef FEATURE_NWY_AUDIO_SD_PLAY
#define NWY_OPEN_AUDIO_SD_PLAY
#endif
#ifdef FEATURE_NWY_AUDIO_DTMF_APP
#define NWY_OPEN_AUDIO_DTMF_TEST
#endif
#ifdef FEATURE_NWY_CAMERA_APP
#define NWY_OPEN_TEST_CAMERA
#endif
#ifdef CONFIG_BK4819_ENABLE
#define NWY_OPEN_TEST_BK4819
#endif

#ifdef CONFIG_SUPPORT_GNSS
#define NWY_OPEN_TEST_GNSS
#endif
#ifdef CONFIG_NWY_SUPPORT_GNSS_EXTERNAL_CHIP
#define NWY_OPEN_TEST_GNSS_EXTERNAL_CHIP
#endif
#ifdef CFW_VOLTE_SUPPORT
#define NWY_OPEN_TEST_VOICE
#endif
/*Begin by xsl to enable ftps api in open version 20231129*/
#ifdef FEATURE_NWY_FTP_SSL_ENABLE
#define NWY_OPEN_TEST_FTPS
#endif
/*End by xsl to enable ftps api in open version 20231129*/
#define NWY_OPEN_TEST_HTTPPUT
#define NWY_OPEN_TEST_LOCATION
#define NWY_OPEN_TEST_PING
/*Begin by zhaotating to enable cdgdftpdn api in open version 20230307*/
#define NWY_OPEN_TEST_CFGDFTPDN_S
/*End by zhaotating to enable cdgdftpdn api in open version 20230307*/
/*Begin by fky to enable netshare api in open version 20230427*/
#define NWY_OPEN_TEST_NETSHARE_S
/*End by fky to enable netshare api in open version 20230427*/
#define NWY_OPEN_TEST_WIFI
#define NWY_OPEN_TEST_FLOW_S
//this is not support feature
#define NWY_OPEN_TEST_PINPUK_TIMES_NS
#define NWY_OPEN_TEST_SET_NWMODE_NS
//#define NWY_OPEN_TEST_FPLMN_NS
#define NWY_OPEN_TEST_IMS_NS
#define NWY_OPEN_TEST_CS_NS
#define NWY_OPEN_TEST_APP_VER_NS
//#define NWY_OPEN_TEST_FOTA_NS
//#define NWY_OPEN_TEST_CELL_ID_NS
//#define NWY_OPEN_TEST_BASE_LOC_NS
//#define NWY_OPEN_TEST_WIFI_LOC_NS
//#define NWY_OPEN_TEST_PM_AUTO_OFF_NS
//#define NWY_OPEN_TEST_PM_CHARGER_NS
#define NWY_OPEN_TEST_AGPS_NS
#define NWY_OPEN_TEST_SD
#define NWY_OPEN_DEL_SMS_NUM         4
#define NWY_OPEN_DEL_SMS_READ         1
#define NWY_OPEN_DEL_SMS_READ_SEND        2
#define NWY_OPEN_DEL_SMS_READ_SEND_UNSEND    3
#define NWY_OPEN_DEL_SMS_ALL       4
#define NWY_OPEN_DEL_SMS_NS          0xFF
#define NWY_OPEN_TEST_CSIM_NS
#ifdef FEATURE_NWY_DUAL_SIM
    #define NWY_OPEN_TEST_POC_DS
#endif
#define NWY_OPEN_TEST_STK
#define NWY_OPEN_TEST_SIM_SWTICH
#define NWY_OPEN_TEST_SECUREBOOT
void nwy_test_cli_dbg(const char *func, int line, char* fmt, ... );
#define NWY_CLI_LOG(...) nwy_test_cli_dbg(__func__, __LINE__, __VA_ARGS__)

#define RS485_GPIO_PORT (2)
#define RS485_DIR_TX (1) //hight level for send
#define RS485_DIR_RX (0) //low level for recv

#define NWY_OPEN_MAX_SPI_CHANNEL  2
#define NWY_OPEN_MAX_UART_CHANNEL 3
#define NWY_OPEN_UART_CHANNEL_NS  0

#define NWY_BAND_LOCK_NUM         4
#define NWY_FREQ_LOCK_NUM         3
#define NWY_OPEN_LOCK_AUTO        0
//#define NWY_OPEN_LOCK_GSM         2
//#define NWY_OPEN_LOCK_WCDMA       3
#define NWY_OPEN_LOCK_LTE         4
#define NWY_OPEN_LOCK_NS          0xFF

//config the net system not support
#define NWY_OPEN_LOCK_GSM        NWY_OPEN_LOCK_NS
#define NWY_OPEN_LOCK_WCDMA      NWY_OPEN_LOCK_NS
#define NWY_OPEN_TEST_FLASH

int nwy_test_cli_wait_select();
void nwy_test_cli_select_enter();
void nwy_test_cli_send_trans_end(void);
int nwy_test_cli_wait_trans_end(void);
void nwy_cli_init_unsol_reg(void);
void nwy_exit_thread(nwy_osi_thread_t hdl);
int nwy_test_cli_check_uart_mode(uint8_t mode);
void _gpioisropen(void *param);
#endif