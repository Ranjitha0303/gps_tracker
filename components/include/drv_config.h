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

#ifndef _DRV_CONFIG_H_
#define _DRV_CONFIG_H_

// Auto generated. Don't edit it manually!

/**
 * debughost rx buffer size in bytes
 *
 * This is the buffer before parsing host packet format.
 */
#define CONFIG_DEBUGHOST_RX_BUF_SIZE 0x200

/**
 * whether to support charger
 */
#define CONFIG_SUPPORT_BATTERY_CHARGER

/**
 * whether to support general spi
 */
/* #undef CONFIG_SUPPORT_GENERAL_SPI */


/**
 * host command engine packet size
 */
#define CONFIG_HOST_CMD_ENGINE_MAX_PACKET_SIZE 0x2020

/**
 * uart TX baud rate at adaptive mode, before baud rate is detected
 */
#define CONFIG_UART_AUTOMODE_DEFAULT_BAUD 115200

/**
 * enable wcn wifi scan or not
 */
/* #undef CONFIG_WCN_WIFI_SCAN_SUPPORT */

/**
 * whether USB is supported
 */
#define CONFIG_USB_SUPPORT

/**
 * whether USB MSG is supported
 */
#define CONFIG_USB_MSG_SUPPORT

/**
 * whether USB FLOWCTRL is supported
 */
/* #undef CONFIG_USB_FLOWCTRL_SUPPORT */

/**
 * whether USB ACM is supported
 */
#define CONFIG_USB_ACM_SUPPORT

/**
 * whether USB RNDIS is supported
 */
#define CONFIG_USB_RNDIS_SUPPORT

/**
 * whether USB HID is supported
 */
#define CONFIG_USB_HID_SUPPORT

/**
 * whether USB MTP is supported
 */
#define CONFIG_USB_MTP_SUPPORT

/**
 * whether USB ETHER is supported
 */
#define CONFIG_USB_ETHER_SUPPORT

#ifdef CONFIG_USB_SUPPORT

/**
 * usb connect timeout
 */
#define CONFIG_USB_CONNECT_TIMEOUT 10000

/**
 * usb debounce time in milliseconds before enumarating
 */
#define CONFIG_USB_DETECT_DEBOUNCE_TIME 600

/**
 * udc features, lower 8 bits for config->bmAttributes, higher are software defined
 */
#define CONFIG_USB_DEVICE_CONTROLLER_FEATURE 0xE0
// START: add by zhaoyong at 20200119 for nwy usb id
#define CONFIG_USE_NEOWAY_USB_ID
// END: add by zhaoyong at 20200119 for nwy usb id

#ifdef CONFIG_USB_ETHER_SUPPORT
/**
 * usb ether host mac address
 */
#define CONFIG_USB_ETHER_HOST_MAC "\xf0\x4b\xb3\xb9\xeb\xe5"

/**
 * usb ether dev mac address
 */
#define CONFIG_USB_ETHER_DEV_MAC "\xfa\x32\x47\x15\xe1\x88"

/**
 * usb ether support dynamic change host mac address
 */
#define CONFIG_USB_ETHER_DYNAMIC_HOST_MAC_SUPPORT on
#endif

#endif

/**
 * support 8850 fpga board driver
 */
/* #undef CONFIG_8850_FPGA_BOARD */

/**
 * support 8850 ipc V2 for 8850CM V1.1
 */
#define CONFIG_IPC_PLAT_8850CM_V2

/**
 * support 8850 bm board driver
 */
/* #undef CONFIG_8850_BM */

/**
 * support CAMERA driver
 */
#define CONFIG_CAMERA_SUPPORT

/**
 * support CAMERA GC032A
 */
#define CONFIG_CAMERA_GC032A_SUPPORT

/**
 * support CAMERA GC0310
 */
/* #undef CONFIG_CAMERA_GC0310_SUPPORT */

/**
 * support CAMERA BF30A2
 */
/* #undef CONFIG_CAMERA_BF30A2_SUPPORT */

/**
 * support CAMERA GC2145
 */
/* #undef CONFIG_CAMERA_GC2145_SUPPORT */

/**
 * support CAMERA GC0406
 */
/* #undef CONFIG_CAMERA_GC0406_SUPPORT */

/**
 * whether to support camera single buffer
 */
#define CONFIG_CAMERA_SINGLE_BUFFER

/**
 * whether to support camera mipi
 */
/* #undef CONFIG_MIPI_SUPPORT */

/**
 * 8850 spi camera Mclk
 */
#define CONFIG_DEFAULT_CAM_MCLK_FREQ 15000000

/**
 * 8850 camera Mclk src freq
 */
#define CONFIG_DEFAULT_CAM_MCLK_SRC_FREQ 500000000

/**
 * 8850 camera Mclk src num
 */
#define CONFIG_CAM_MCLK_SRC_NUM 5

/**
 * 8850 camera pix clk src freq
 */
#define CONFIG_DEFAULT_CAM_PCLK_SRC_FREQ 500000000

/**
 * 8850 camera pix clk src num
 */
#define CONFIG_CAM_PCLK_SRC_NUM 5

/**
 * 8850 camera csi clk src freq
 */
#define CONFIG_DEFAULT_CAM_CCLK_SRC_FREQ 500000000

/**
 * 8850 camera csi clk src num
 */
#define CONFIG_CAM_CCLK_SRC_NUM 5

/**
 * 8850 camera pix clk freq
 */
#define CONFIG_DEFAULT_CAM_PCLK_FREQ 39000000

/**
 * 8850 camera csi clk freq
 */
#define CONFIG_DEFAULT_CAM_CCLK_FREQ 125000000

/**
 * 8850 camera csi Mclk freq
 */
/* #undef CONFIG_DEFAULT_CAM_MIPI_MCLK_FREQ */

/**
 * support LCD driver
 */
#define CONFIG_LCD_SUPPORT

/**
 * support GC9305 panel
 */
#define CONFIG_SUPPORT_LCD_GC9305

/**
 * support GC9306 panel
 */
#define CONFIG_SUPPORT_LCD_GC9306

/**
 * support ST7735S panel
 */
/* #undef CONFIG_SUPPORT_LCD_ST7735S */

/**
 * support ST7789h2 panel
 */
/* #undef CONFIG_SUPPORT_LCD_ST7789h2 */

/**
 * support support MIPI LCD driver
 */
/* #undef CONFIG_MIPI_LCD_SUPPORT */

/**
 * whether headset detect is supported
 */
/* #undef CONFIG_HEADSET_DETECT_SUPPORT */

/**
 * whether sdcard1 detect is supported
 */
#define CONFIG_BOARD_SUPPORT_SDCARD1_DETECT

/**
 * id for SDCARD1 hot plug detect
 */
#define CONFIG_BOARD_SDCARD1_ID 1

/**
 * gpio for SDCARD1 hot plug detect
 */
#define CONFIG_BOARD_SDCARD1_DETECT_GPIO 4

/**
 * whether sdcard2 detect is supported
 */
/* #undef CONFIG_BOARD_SUPPORT_SDCARD2_DETECT */

/**
 * id for SDCARD2 hot plug detect
 */
/* #undef CONFIG_BOARD_SDCARD2_ID */

/**
 * gpio for SDCARD2 hot plug detect
 */
/* #undef CONFIG_BOARD_SDCARD2_DETECT_GPIO */


/**
 * headset thread stack size in bytes
 */
/* #undef CONFIG_HEADSET_STACK_SIZE */

/**
 * support SDMMC driver
 */
/* #undef CONFIG_SDMMC_SUPPORT */

/**
 * support EMMC driver
 */
#define CONFIG_EMMC_SUPPORT

/**
 * SDMMC clock frequency for SD
 */
#define CONFIG_SDMMC_CLK_FREQ_SD 25000000

/**
 * SDMMC clock frequency for SDHC
 */
#define CONFIG_SDMMC_CLK_FREQ_SDHC 50000000

/**
 * support usb host function
 */
#define CONFIG_USB_HOST_SUPPORT

/**
 * support usb device function
 */
#define CONFIG_USB_DEVICE_SUPPORT

/**
 * support usb netcard adaption function
 */
/* #undef CONFIG_USB_NETCARD_ADAPTION_SUPPORT */

/**
 * support SDMMC/EMMC DRIVER
 */
#define CONFIG_SDIO_SUPPORT

/**
 * support SDMMC/EMMC DRIVER
 */
/* #undef CONFIG_SDIO2_SDMMC_SUPPORT */

/**
 * support TEE
 */
/* #undef CONFIG_TEE_SUPPORT */

/**
 * spport emmc
 */
/* #undef CONFIG_SUPPORT_EMMC */

/**
 * 8910 EMMC
 */
/* #undef CONFIG_8910_EMMC_SUPPORT */


/**
 * spport SD2
 */
/* #undef CONFIG_SUPPORT_SD2 */

/**
 * Support large capacity flash
 */
/* #undef CONFIG_SUPPORT_LC_FLASH */

/**
 * Support larger than 16MB General spi flash
 */
/* #undef CONFIG_GENERAL_SPI_LC_FLASH */

/**
 * whether support SPI NAND DRIVER
 */
/* #undef CONFIG_SPI_NAND_DRIVER_SUPPORT */
#endif
