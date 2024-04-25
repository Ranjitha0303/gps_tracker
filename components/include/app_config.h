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

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

// Auto generated. Don't edit it manually!

/**
 * Application version
 */
#define CONFIG_APP_VERSION "2.1"

/**
 * whether to start AT engine
 */
#define CONFIG_APP_START_ATR

/**
 * Whether to disable modem (8811 only)
 */
/* #undef CONFIG_APP_NOT_START_MODEM */

/**
 * whether to enable watchdog
 *
 * By default, watchdog will be enabled when \p BUILD_RELEASE_TYPE is
 * "release".
 */
#define CONFIG_WDT_ENABLE

/**
 * Application watchdog max feed interval (ms)
 */
#define CONFIG_APP_WDT_MAX_INTERVAL 20000

/**
 * Application watchdog normal feed interval (ms)
 */
#define CONFIG_APP_WDT_FEED_INTERVAL 4000

/**
 * Whether to support softsim hot plug
 */
/* #undef CONFIG_APP_SSIM_SUPPORT */

/**
 * Whether to reuse at uart at blue screen
 */
#define CONFIG_ATR_UART_BLUE_SCREEN_DEBUG

/**
 * whether to enable daplink link 8850 AP 
 *
 * By default, it will be enabled when \p BUILD_RELEASE_TYPE is
 * "release". If CONFIG_AP_A5_CLK_AUTO_GATE enable, daplink can't link AP A5.
 */
#define CONFIG_AP_A5_CLK_AUTO_GATE

/**
 * Whether to compress APP librarys for code size reduction.
 */
/* #undef CONFIG_APP_COMPRESSED_SUPPORT */
#endif
