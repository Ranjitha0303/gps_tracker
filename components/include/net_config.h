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

#ifndef _NET_CONFIG_H_
#define _NET_CONFIG_H_

#include "hal_config.h"

// Auto generated. Don't edit it manually!

/**
 * When enabled, CONFIG_NET_LWIP_DEBUG will be defined
 * If disabled, NO lwip debug will be print
 */
#define CONFIG_NET_LWIP_DEBUG
/**
 * When enabled, CONFIG_NET_TCPIP_SUPPORT will be defined
 */
#define CONFIG_NET_TCPIP_SUPPORT
/**
 * When enabled, CONFIG_NET_LWIP_IPV4_ON will be defined
 * default enabled on all chip
 */
#define CONFIG_NET_LWIP_IPV4_ON
/**
 * When enabled, CONFIG_NET_LWIP_IPV6_ON will be defined
 * default enabled on all chip
 */
#define CONFIG_NET_LWIP_IPV6_ON
/**
 * When enabled, CONFIG_NET_LWIP_RFACK_ON will be defined
 * default enabled on 881, UDP package will get ack after send to network
 */
/* #undef CONFIG_NET_LWIP_RFACK_ON */
/**
 * When enabled, CONFIG_NET_LWIP_TCP_ON will be defined
 * default enabled on all chip
 */
#define CONFIG_NET_LWIP_TCP_ON
/**
 * When enabled, CONFIG_NET_LWIP_PPP_ON will be defined
 * default enabled on all chip
 */
#define CONFIG_NET_LWIP_PPP_ON
/**
 * int value , CONFIG_NET_LOG_BUF_COUNT
 * default is 2
 */
#define CONFIG_NET_LOG_BUF_COUNT 2
/**
 * int value , CONFIG_NET_LOG_BUF_SIZE
 * default is 8192
 */
#define CONFIG_NET_LOG_BUF_SIZE 8192
/**
 * When enabled, CONFIG_NET_TRACE_IP_PACKET will be defined
 * identify of IP packet will be show in trace.
 *It is just for debug, and usually not needed in produciton.
 * IP package will be dump in trace, and can be save to .pcap file
 */
#define CONFIG_NET_TRACE_IP_PACKET
/**
 * When enabled, CONFIG_NET_NAT_SUPPORT will be defined
 * IPV4 NAT support
 */
/* #undef CONFIG_NET_NAT_SUPPORT */
/**
 * When enabled, CONFIG_MBEDTLS_LIB_SUPPORT will be defined
 */
#define CONFIG_MBEDTLS_LIB_SUPPORT
/**
 * When enabled, CONFIG_MBEDTLS_TCPIP_SUPPORT will be defined
 */
/* #undef CONFIG_MBEDTLS_TCPIP_SUPPORT */
/**
 * When enabled, CONFIG_NET_USSL_SUPPORT will be defined
 */
/* #undef CONFIG_NET_USSL_SUPPORT */
/**
 * When enabled, CONFIG_NET_SELF_REGISTE_SUPPORT will be defined
 */
#define CONFIG_NET_SELF_REGISTE_SUPPORT
/**
 * When enabled, CONFIG_NET_LIBCOAP_SUPPORT will be defined
 */
/* #undef CONFIG_NET_LIBCOAP_SUPPORT */
/**
 * When enabled, CONFIG_TINYDTLS_LIB_SUPPORT will be defined
 */
/* #undef CONFIG_TINYDTLS_LIB_SUPPORT */
/**
 * When enabled, CONFIG_MBEDTLS_USE_CE_CRYPTO will be defined
 */
/* #undef CONFIG_MBEDTLS_USE_CE_CRYPTO */
/**
 * When enabled, CONFIG_NET_NBSELFREG_HTTP_SUPPORT will be defined
 */
/* #undef CONFIG_NET_NBSELFREG_HTTP_SUPPORT */
/**
 * When enabled, CONFIG_NET_SELFREG_COAP_SUPPORT will be defined
 */
/* #undef CONFIG_NET_SELFREG_COAP_SUPPORT */
/**
 * When enabled, CONFIG_NET_DM_CISSDK_SUPPORT will be defined
 */
/* #undef CONFIG_NET_DM_CISSDK_SUPPORT */
/**
 * When enabled, CONFIG_NET_DM_LWM2M_SUPPORT will be defined
 */
/* #undef CONFIG_NET_DM_LWM2M_SUPPORT */
/**
 * When enabled, CONFIG_NET_DM_CUS_SUPPORT will be defined
 */
/* #undef CONFIG_NET_DM_CUS_SUPPORT */
/**
 * When enabled, CONFIG_NBSELFREG_CUS_SUPPORT will be defined
 */
/* #undef CONFIG_NBSELFREG_CUS_SUPPORT */
/**
 * When enabled, CONFIG_LWIP_TCP_SACK_SUPPORT will be defined
 */
#define CONFIG_LWIP_TCP_SACK_SUPPORT
/**
 * When enabled, CONFIG_NET_IDS_SUPPORT will be defined
 */
#define CONFIG_NET_IDS_SUPPORT
/**
 * When enabled, CONFIG_NET_DNS_SERVER_SUPPORT will be defined
 */
/* #undef CONFIG_NET_DNS_SERVER_SUPPORT */
/**
 * When enabled, CONFIG_NET_CTIOTSM_SUPPORT will be defined
 */
/* #undef CONFIG_NET_CTIOTSM_SUPPORT */
/**
 * When enabled, CONFIG_NET_CTWING_SELFREG_SUPPORT will be defined
 */
/* #undef CONFIG_NET_CTWING_SELFREG_SUPPORT */
/**
 * When enabled, CONFIG_NET_LIBDMP_SDK_SUPPORT will be defined
 */
/* #undef CONFIG_NET_LIBDMP_SDK_SUPPORT */
/**
 * When enabled, CONFIG_NET_CTIOT_AEP_SUPPORT will be defined
 */
/* #undef CONFIG_NET_CTIOT_AEP_SUPPORT */

/**
 * When enabled, CONFIG_NET_LTE_PSPACKET_SUPPORT will be defined
 */
#define CONFIG_NET_LTE_PSPACKET_SUPPORT
/**
 * When enabled, CONFIG_NET_CTWING_MQTT_SUPPORT will be defined
 */
/* #undef CONFIG_NET_CTWING_MQTT_SUPPORT */
/**
 * When enabled, CONFIG_CTHTTP_API_SUPPORT will be defined
 */
/* #undef CONFIG_CTHTTP_API_SUPPORT */
/**
 * When enabled, CONFIG_NET_SCI_NETIF_SUPPORT will be defined
 */
/* #undef CONFIG_NET_SCI_NETIF_SUPPORT */
/**
 * When enabled, CONFIG_AT_TCPCTWING_SUPPORT will be defined
 */
/* #undef CONFIG_AT_TCPCTWING_SUPPORT */
/**
 * When enabled, CONFIG_MATRIX_SSL_API_SUPPORT will be defined
 */
/* #undef CONFIG_MATRIX_SSL_API_SUPPORT */
/**
 * When enabled, CONFIG_LWIP_IPSEC_SUPPORT will be defined
 */
/* #undef CONFIG_LWIP_IPSEC_SUPPORT */
/**
 * When enabled, CONFIG_NET_IMSHTTP_LIB_SUPPORT will be defined
 */
/* #undef CONFIG_NET_IMSHTTP_LIB_SUPPORT */
/**
 * When enabled, CONFIG_NET_IMSHTTP_SSL_SUPPORT will be defined
 */
/* #undef CONFIG_NET_IMSHTTP_SSL_SUPPORT */
/**
 * When enabled, CONFIG_LWIP_IMS_SUPPORT will be defined
 */
/* #undef CONFIG_LWIP_IMS_SUPPORT */
/**
 * When enabled, CONFIG_NET_LTE_SUPPORT will be defined
 */
#define CONFIG_NET_LTE_SUPPORT
/**
 * When enabled, CONFIG_LWIP_DROP_OVER_RCV_WND will be defined
 */
#define CONFIG_LWIP_DROP_OVER_RCV_WND
/**
 * When enabled, CONFIG_NET_LWIP_ARP_SUPPORT will be defined
 */
/* #undef CONFIG_NET_LWIP_ARP_SUPPORT */
/**
 * When enabled, CONFIG_NET_LWIP_ALTCP_SUPPORT will be defined
 */
/* #undef CONFIG_NET_LWIP_ALTCP_SUPPORT */
/**
 * When enabled, CONFIG_NET_LWIP_SAMPLE_ASSERT will be defined
 */
/* #undef CONFIG_NET_LWIP_SAMPLE_ASSERT */
/**
 * When enabled, CONFIG_NET_NONIP_SUPPORT will be defined
 */
#define CONFIG_NET_NONIP_SUPPORT
/**
 * When enabled, CONFIG_NET_LWIP_MLD_SUPPORT will be defined
 */
#define CONFIG_NET_LWIP_MLD_SUPPORT
#endif
