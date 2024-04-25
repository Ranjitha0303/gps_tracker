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

#ifndef _MBEDTLS_CONFIG_H_
#define _MBEDTLS_CONFIG_H_

// Auto generated. Don't edit it manually!

/**
 * When enabled, MBEDTLS_REDUCE_MEMORY will be defined
 */
//NWY qj TD93308 
#define CONFIG_MBEDTLS_REDUCE_MEMORY
//NWY qj TD93308 
/**
 * When enabled, MBEDTLS_LITE_CRYPTS_3 will be defined,
 * delete more algorithms to decrease space, only PSK keyexchange for dtls
 */
/* #undef CONFIG_MBEDTLS_LITE_CRYPTS_3 */
/**
 * When enabled, MBEDTLS_LITE_CRYPTS_2 will be defined,
 * delete more algorithms to decrease space, for example:chachapoly, camellia, chacha20 and so on
 */
#define CONFIG_MBEDTLS_LITE_CRYPTS_2
/**
 * When enabled, MBEDTLS_LITE_CRYPTS_1 will be defined,
 * delete some Infrequent functions and algorithms, for example: X509 create, crt write, des, blowfish and so on
 */
#define CONFIG_MBEDTLS_LITE_CRYPTS_1
/**
 * When enabled, MBEDTLS_SSL_TLS will be defined,
 * if select more than one version ,the version must be continuous
 */
#define CONFIG_MBEDTLS_SSL_TLS
/**
 * When enabled, MBEDTLS_SSL_VERSION_SSL3 will be defined
 */
/* #undef CONFIG_MBEDTLS_SSL_VERSION_SSL3 */
/**
 * When enabled, MBEDTLS_SSL_VERSION_TLS1_0 will be defined
 */
/* #undef CONFIG_MBEDTLS_SSL_VERSION_TLS1_0 */
/**
 * When enabled, MBEDTLS_SSL_VERSION_TLS1_1 will be defined
 */
/* #undef CONFIG_MBEDTLS_SSL_VERSION_TLS1_1 */
/**
 * When enabled, MBEDTLS_SSL_VERSION_TLS1_2 will be defined
 */
#define CONFIG_MBEDTLS_SSL_VERSION_TLS1_2
/**
 * When enabled, MBEDTLS_SSL_VERSION_DTLS will be defined,
 * depends on MBEDTLS_SSL_TLS, depends on (MBEDTLS_SSL_VERSION_TLS1_1 || MBEDTLS_SSL_VERSION_TLS1_2)
 */
#define CONFIG_MBEDTLS_SSL_VERSION_DTLS
/**
 * When enabled, MBEDTLS_SELF_TEST will be defined
 */
/* #undef CONFIG_MBEDTLS_SELF_TEST */
/**
 * When enabled, MBEDTLS_SUPPORT_ECP will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
#define CONFIG_MBEDTLS_SUPPORT_ECP
/**
 * When enabled, MBEDTLS_SUPPORT_SM2 will be defined
 * depends on MBEDTLS_LIB_SUPPORT, depends on MBEDTLS_SUPPORT_ECP
 */
#define CONFIG_MBEDTLS_SUPPORT_SM2
/**
 * When enabled, MBEDTLS_SUPPORT_ECDSA_DETERMINISTIC will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_ECDSA_DETERMINISTIC */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_DHE_PSK will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_DHE_PSK */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDHE_PSK will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDHE_PSK */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_RSA_PSK will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_RSA_PSK */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_DHE_RSA will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_DHE_RSA */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDHE_RSA will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDHE_RSA */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDHE_ECDSA will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDHE_ECDSA */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDH_ECDSA will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDH_ECDSA */
/**
 * When enabled, MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDH_RSA will be defined
 * depends on MBEDTLS_LIB_SUPPORT
 */
/* #undef CONFIG_MBEDTLS_SUPPORT_KEY_EXCHANGE_ECDH_RSA */
#endif
