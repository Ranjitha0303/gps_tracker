#ifndef _NWY_PROJECT_H_
#define _NWY_PROJECT_H_

/******************* [VERSION CONFIG] ****************/
#define MFT_STR     "NEOWAY"
#define HW_VER_STR  "30"
#define PRD_STR     "N706"
#define PRJ_STR     "STD-OE_CM"
#define SW_VER_STR  "003"
#define CUS_STR     ""
#define TMP_STR     "T2403050921"
extern char *nwy_ivsn_get(char *);
#define IVSN_STR        nwy_ivsn_get(NULL)                              // "N706-R001-STD-0E-DG_V10-001"
#define CGMR_STR        IVSN_STR                                        // "N706-R001-STD-0E-DG_V10-001"
#define ATI_STR         MFT_STR "\r\n" PRD_STR "\r\n" "V" SW_VER_STR    // "NEOWAY\r\nN706\r\nV001"
#define CGMI_STR        "Neoway Corp Ltd"
#define CGMM_STR        PRD_STR                                         // "N706"
#define OPEN_SDK_VER_STR "N706-OPEN-SDK-01A"

#define nwy_hw_version "V" HW_VER_STR
#define nwy_sfv_time "310124"
#define nwy_hwv_time "200920"
/*****************************************************/

/******************* [BUILD DEFINE] ****************/
/******************* [BUILD DEFINE] ****************/
/* #undef CONFIG_NWY_AT_TEST */
/* #undef CONFIG_NWY_OPEN_SDK_SUPPORT */
#define CONFIG_NWY_RWS_SUPPORT
/* #undef CONFIG_NWY_OPEN_AWS_MQTT_SUPPORT */
/*****************************************************/

/******************* [FUNCTION DEFINE] ****************/
#define FEATURE_NWY_AT
//#define FEATURE_NWY_AT_HTTP_SUPPORT
//#define FEATURE_NWY_AT_FOTA
//#define FEATURE_NWY_AT_FILE_SUPPORT

#define FEATURE_NWY_AT_FILESYS
//#define NWY_AT_STD_CERT_SUPPORT
//#define NWY_AT_STD_TCP_SUPPORT
//#define NWY_AT_STD_UDP_SUPPORT
//#define NWY_AT_STD_TCP_SSL_SUPPORT
//#define NWY_AT_STD_FTP_SUPPORT
#define NWY_AT_UPDATETIME_SUPPORT
#define NWY_AT_PDP_INFO_SUPPORT
#define NWY_AT_WIFI_INFO_SUPPORT
#define NWY_LBS_MODULE_SUPPORT
#define NWY_AT_CIPGSMLOC_SUPPORT
#define NWY_AT_NET_INFO_SUPPORT

#define CONFIG_SOC_8850
#define FEATURE_NWY_NO_SIM_DETECT

//#define NWY_STD_AT_FILE_SUPPORT

//#define FEATURE_NWY_FRM_TEST
//#define NWY_AT_PROC_OLD
//#define NWY_AT_PROC_MQTT
#define CONFIG_MBEDTLS_LIB_SUPPORT
//#define NWY_AT_HTTP_HTTPS_GETRPT

//fota enable
//#define FEATURE_NWY_WEFOTA_ENABLE
#define FEATURE_NWY_FTP_SSL_ENABLE

// ------NWY OPEN SDK ------
#define FEATURE_NWY_OPEN_SDK
#define CONFIG_USE_NEOWAY_USB_ID
// -----GENERAL SPI MOUNT -----
#define FEATURE_NWY_SPI_FLASH_FS
#define FEATURE_8850_8910_API_SAME

//for open camera app test
#define FEATURE_NWY_CAMERA_APP
/*****************************************************/
// hh scan code
#define CONFIG_NWY_HHSCANCODE_SUPPORT
#define FEATURE_NWY_HHSCANCODE_SUPPORT

/*Begin: Add by yaokai for/to GPIO19 ENABLE in 2023-11-1*/
#define FEATURE_NWY_USE_GPIO19_NOT_PAD_I2S
/*End: Add by yaokai for/to GPIO19 ENABLE in 2023-11-1*/

//Use indelta nv
#define FEATURE_NWY_USE_INDELTA_NV
// ------AWS MQTT ------
#define FEATURE_NWY_AWS_MQTT

#define FEATURE_NWY_OPEN_STK_URCSTKPCI_AUTOMODE
/*****************************************************/

#endif/*_NWY_PROJECT_H_*/

