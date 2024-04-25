/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _ATS_CONFIG_H_
#define _ATS_CONFIG_H_

#include "srv_config.h"
#include "tts_config.h"

// Auto generated. Don't edit it manually!

#define GMI_ID "UNISOC"
#define GMM_ID "8850CM_cat1bis_plus-release"
#define GMR_ID "DEVEL-gc455257d-dirty"
#define GMT_ID "20240305_092643"

/**
 * Whether to support coap AT commands
 */
/* #undef CONFIG_AT_LIBCOAP_SUPPORT */

/**
 * Whether to support http AT commands
 */
#define CONFIG_AT_HTTP_SUPPORT

/**
 * Whether to support http2 test AT commands
 */
/* #undef CONFIG_AT_HTTP2_SUPPORT */

/**
 * Whether to support alic AT commands
 */
/* #undef CONFIG_AT_ALIC_SUPPORT */

/**
 * Whether to support ftp client AT commands
 */
/* #undef CONFIG_AT_FTP_SUPPORT */

/**
 * Whether to support tcp client AT commands
 */
#define CONFIG_AT_TCPIP_SUPPORT

/**
 * Whether to support tcp full AT commands
 */
#define CONFIG_AT_TCPIP_FULL_FUNC_SUPPORT

/**
* Whether to support tcp ctwing AT commands
*/
/* #undef CONFIG_AT_TCPCTWING_SUPPORT */

/**
 * Whether to support udp client AT commands
 */
/* #undef CONFIG_AT_UDP_SUPPORT */

/**
 * Whether to support (refactored) ftp client AT commands
 */
/* #undef CONFIG_AT_NEWFTP_SUPPORT */

/**
 * Whether to support (refactored) ftps client AT commands
 */
/* #undef CONFIG_AT_NEWFTP_SSL_SUPPORT */

/**
 * Whether to support MYFTP AT commands
 */
/* #undef CONFIG_AT_MYFTP_SUPPORT */

/**
 * Whether to support MYFTP (refactored) AT commands
 */
/* #undef CONFIG_AT_NEWMYFTP_SUPPORT */

/**
 * Whether to support MYNET AT commands
 */
/* #undef CONFIG_AT_MYNET_TCPIP_SUPPORT */

/**
 * Whether to support SSL AT commands
 */
/* #undef CONFIG_AT_MBEDTLS_TCPIP_SUPPORT */

/**
 * Whether to support QSSL AT commands
 */
/* #undef CONFIG_AT_USSL_SUPPORT */

/**
 * Whether to support MY AT commands
 */
#define CONFIG_AT_MYINFO_SUPPORT

/**
 * Whether to support +IPR=<n>&W
 *
 * It doesn't conform to specification, but it is needed in many cases.
 */
#define CONFIG_AT_IPR_SUPPORT_ANDW

/**
 * Whether to support CISSDK mipl AT commands
 */
/* #undef CONFIG_AT_CISSDK_MIPL_SUPPORT */

/**
 * Whether to support LWM2M mipl AT commands
 */
/* #undef CONFIG_AT_LWM2M_MIPL_SUPPORT */

/**
 * Whether to support LWM2M mipl SOTA AT commands
 */
/* #undef CONFIG_AT_LWM2M_MIPL_SOTA_SUPPORT */

/**
 * Whether to support CUS mipl AT commands
 */
/* #undef CONFIG_AT_CUS_MIPL_SUPPORT */

/**
 * Whether to support softsim AT commands
 */
/* #undef CONFIG_AT_SSIM_SUPPORT */

/**
 * whether to support DM AT command
 */
/* #undef CONFIG_AT_DM_SUPPORT */


/**
 * whether to support self_regstration AT commands
 */
/* #undef CONFIG_AT_SELFREG_SUPPORT */

/**
 * whether to support DM CUS AT command
 */
/* #undef CONFIG_AT_DM_CUS_SUPPORT */

/**
 * whether to support oceanconnect AT commands
 */
/* #undef CONFIG_AT_OCEANCONNECT_SUPPORT */

/**
 * whether to support MQTT (based on lwIP) AT commands
 */
/* #undef CONFIG_AT_LWIP_MQTT_SUPPORT */

/**
 * whether to support MQTT (based on paho) AT commands
 */
/* #undef CONFIG_AT_PAHO_MQTT_SUPPORT */

/**
 * whether to support CTWINGMQTT (based on paho) AT commands
 */
/* #undef CONFIG_AT_PAHO_MQTT_CTWING_SUPPORT */

/**
 * whether to support AT commands for memory size
 */
#define CONFIG_AT_EMMCDDRSIZE_SUPPORT

/**
 * whether to support audio AT commands
 */
#define CONFIG_AT_AUDIO_SUPPORT

/**
 * whether to use global APN
 *
 * When enabled, global/large APN information will be embedded. Otherwise,
 * local/small APN information will be embedded.
 */
/* #undef CONFIG_AT_GLOBAL_APN_SUPPORT */

/**
 * whether some cc at command can be used
 */
/* #undef CONFIG_AT_CC_COMMAND_SUPPORT */

/**
 * whether D and CDU at command can be used
 */
#define CONFIG_AT_CC_COMMAND_DAILUP_SUPPORT

/**
 * whether some gc at command can be used
 */
#define CONFIG_AT_GC_COMMAND_SUPPORT

/**
 * whether some sms at command can be used
 */
#define CONFIG_AT_SMS_COMMAND_SUPPORT

/**
 * whether some ss at command can be used
 */
/* #undef CONFIG_AT_SS_COMMAND_SUPPORT */

/**
 * whether some pbk at command can be used
 */
/* #undef CONFIG_AT_PBK_COMMAND_SUPPORT */

/**
 * whether some gprs at command can be used
 */
#define CONFIG_AT_GPRS_COMMAND_SUPPORT

/**
 * whether jamming detect parameters saved auto
 */
#define CONFIG_AT_JAMMING_DETECT_PARAM_AUTO_SAVE

/**
 * whether some audio at command can be used
 */
#define CONFIG_AT_AUDIO_COMMAND_SUPPORT

/**
 * whether some bt basic at command can be used
 */
/* #undef CONFIG_AT_BT_CLASSIC_SUPPORT */

/**
 * whether some blueu verify at command can be used
 */
/* #undef CONFIG_AT_BLUEU_VERIFY_SUPPORT */

/**
 * whether some bt app at command can be used
 */
/* #undef CONFIG_AT_BT_APP_SUPPORT */

/**
 * whether some wifi basic at command can be used
 */
#define CONFIG_AT_WIFISCAN_SUPPORT

/**
 * whether wifi sensitivity test command can be used
 */
/* #undef CONFIG_AT_WIFI_SENSITIVITY_TEST_SUPPORT */

/**
 * whether 8850 wifi sensitivity test command can be used
 */
#define CONFIG_AT_8850_WIFI_SENTEST_SUPPORT

/**
 * whether some nbonly at command can be used
 */
/* #undef CONFIG_ATR_NBONLY_API_SUPPORT */

/**
 * whether utest at command can be used
 */
#define CONFIG_AT_UTEST_SUPPORT

/**
 * whether aep CTWING at command can be used
 */
/* #undef CONFIG_AT_CTWING_SUPPORT */

/**
 * whether to support aep CTWING new standard
 */
/* #undef CONFIG_AT_CTWING_NEW_STANDARD */

/**
 * whether to support aep CTWING SELFREG
 */
/* #undef CONFIG_AT_CTWING_SELFREG_SUPPORT */

/**
 * whether unisoc special AT command can be used
 */
#define CONFIG_AT_UNISOC_SPECIAL_SUPPORT

/**
 * whether dynamic create at channel at command can be used
 */
#define CONFIG_AT_U_CREATE_AT_CHANNEL

/**
 * whether armlogel AT command support or not
 */
#define CONFIG_AT_ARMLOG_SUPPORT

/**
 * whether AT command mal api can be used
 */
/* #undef CONFIG_MAL_API_SUPPORT */

/**
 * whether some bt basic at command can be used
 */
/* #undef CONFIG_AT_VSIM_SUPPORT */

/**
 * whether support dtmf key detect function
 */
/* #undef CONFIG_DTMF_KEY_DETECT_SUPPORT */

/**
 * whether to support iperf AT commands
 */
#define CONFIG_AT_IPERF_SUPPORT

/**
 * whether to support customized AT commands
 */
/* #undef CONFIG_AT_CUS_SUPPORT */

/**
 * whether support nbiotrai AT commands
 */
/* #undef CONFIG_AT_NBIOTRAI_SUPPORT */

#endif

#define CONFIG_AT_GPRS_DEFAULT_LTE_CID 1

#define CONFIG_AUDIO_SET_LOOPBACK_GAIN_SUPPORT

/**
 * whether support concat sms
 */
#define CONFIG_CONCAT_SMS_SUPPORT

/**
 * whether support a few SMS
 */
#define CONFIG_LIMITED_NUMBER_SMS_SUPPORT

/**
 * whether AT command IDS can be used
 */
#define CONFIG_AT_IDS_SUPPORT

/**
 * whether AT command CTIOTSM can be used
 */
/* #undef CONFIG_AT_CTIOTSM_SUPPORT */

/* #undef CONFIG_AT_CTIOTAEP_SUPPORT */

/**
 * whether AT command UNIKEY can be used
 */
/* #undef CONFIG_AT_UNIKEY_SUPPORT */

/**
 * whether AT command CTWING_HTTP can be used
 */
/* #undef CONFIG_AT_CTWING_HTTP_SUPPORT */

/**
 * whether AT command UNIMQTT can be used
 */
/* #undef CONFIG_AT_UNI_MQTT_SUPPORT */

/**
 * whether AT CCLK command new style be used
 */
/* #undef CONFIG_AT_CCLK_COMMAND_STYLE_NEW */

/**
 * whether AT command CTWING_HTTP can be used
 */
/* #undef CONFIG_AT_CTWING_HTTP_SUPPORT */

#define CONFIG_AT_SMS_CB_SUPPORT

/**
 * whether disable PM2 on platform
 */
/* #undef CONFIG_AT_PM2_DISABLE */

/**
 * whether disable sntp on platform
 */
#define CONFIG_AT_SNTP_SUPPORT

/**
 * whether AT command EFUSEID can be used
 */

#define CONFIG_AT_EFUSEID_SUPPORT

/**
 * whether enable gnssinfo urc for 8821
 */
/* #undef CONFIG_AT_NTN_NOTIFY_GNSSINPUT */

#define CONFIG_AT_UNSOLICITED_RESULT_CODES_MAX 20

/**
 * whether disable AT STK on platform
 */
#define CONFIG_AT_STK_SUPPORT

/**
 * whether E-CID position function can be used
 */
/* #undef CONFIG_ECID_POSITION_SUPPORT */
/**
 * define SHARE SIM FEATURE
 */
/* #undef CONFIG_SHARE_SIM_SUPPORT */

/**
 * define POC FEATURE
 */
#define CONFIG_AT_AUDIO_POC_SUPPORT
/**
 * define Audio loopback FEATURE
 */
#define CONFIG_AT_AUDIO_LB_SUPPORT
