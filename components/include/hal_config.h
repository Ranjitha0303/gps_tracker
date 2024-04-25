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

#ifndef _HAL_CONFIG_H_
#define _HAL_CONFIG_H_

#include "hal_chip_config.h"

// Auto generated. Don't edit it manually!

/**
 * SOC name
 */
#define CONFIG_SOC "8850"

/**
 * build target
 */
#define CONFIG_BUILD_TARGET "8850CM_cat1bis_plus"

/**
 * whether it is 8910 series
 */
/* #undef CONFIG_SOC_8910 */

/**
 * whether it is 8811 series
 */
/* #undef CONFIG_SOC_8811 */

/**
 * whether it is 8821 series
 */
/* #undef CONFIG_SOC_8821 */

/**
 * whether it is 8850 series
 */
#define CONFIG_SOC_8850

/**
 * whether it is FPGA, not real chip
 *
 * This will be defined when developing in FPGA. For SOC independent codes,
 * and it is needed to distinguish FPGA and real chip, this macro can be
 * used.
 */
/* #undef CONFIG_RUN_ON_FPGA */

/**
 * whether it is ARM CPUs
 */
#define CONFIG_CPU_ARM

/**
 * whether it is ARMv7-A CPUs
 */
#define CONFIG_CPU_ARMV7A

/**
 * whether it is ARM Cortex A5
 */
#define CONFIG_CPU_ARM_CA5

/**
 * whether it is ARMv7-M CPUs
 */
/* #undef CONFIG_CPU_ARMV7M */

/**
 * whether it is ARM Cortex M4F
 */
/* #undef CONFIG_CPU_ARM_CM4F */

/**
 * whether it is ARMv8-M CPUs
 */
/* #undef CONFIG_CPU_ARMV8M */

/**
 * whether it is ARM Cortex M33F
 */
/* #undef CONFIG_CPU_ARM_CM33F */

/**
 * I-cache and D-cache line size
 *
 * It is assumed that I-cache line size is the same with D-cache line size.
 */
#define CONFIG_CACHE_LINE_SIZE 32

/**
 * whether there is MMU
 */
#define CONFIG_HAVE_MMU

/**
 * SOC have hardware LZMA
 */
#define CONFIG_HAVE_LZMA

/**
 * SOC have hardware spinlock
 */
#define CONFIG_HAVE_SPINLOCK

/**
 * whether there is PSRAM
 */
#define CONFIG_USE_PSRAM

/**
 * whether it is 8811 CD
 */
/* #undef CONFIG_CHIP_8811CD */

/**
 * whether it is 8821CD OPENPHONE
 */
/* #undef CONFIG_8821CD_OPENPHONE */

/**
 * whether it is 8821CD V2
 */
/* #undef CONFIG_8821CD_V2 */

/**
 * whether it is QPI 4bits PSRAM (8811)
 */
/* #undef CONFIG_QPI_PSRAM */

/**
 * whether it is OPI 8bits PSRAM (8811)
 */
/* #undef CONFIG_OPI_PSRAM */

/**
 * whether there is DDR
 */
/* #undef CONFIG_USE_DDR */

/**
 * whether there is PSRAM or DDR
 */
#define CONFIG_HAVE_EXT_RAM

/**
 * PSRAM or DDR size, shall match CONFIG_RAM_xx
 */
#define CONFIG_RAM_SIZE 0x800000

/**
 * whether NOR flash is used
 */
#define CONFIG_NOR_FLASH

/**
 * NOR flash size, shall match CONFIG_FLASH_xx
 */
#define CONFIG_FLASH_SIZE 0x800000

/**
 * SRAM size
 *
 * - 8955/8909: system SRAM size
 * - 8811: total SRAM size
 * - 8910: total SRAM size
 */
#define CONFIG_SRAM_SIZE 0x4000

/**
 * aon SRAM size (8850)
 */
#define CONFIG_AON_SRAM_SIZE 0x14000

/**
 * cp SRAM size (8850)
 */
#define CONFIG_CP_SRAM_SIZE 0x40000

/**
 * simage header size (used in 8811)
 */
#define CONFIG_SIMAGE_HEADER_SIZE 0x100

/**
 * uimage header size (used in 8910)
 */
/* #undef CONFIG_UIMAGE_HEADER_SIZE */

/**
 * system ROM physical address
 *
 * It may be undefined when system ROM won't be accessed
 */
#define CONFIG_ROM_PHY_ADDRESS 0x0

/**
 * NBROM physical address
 */
/* #undef CONFIG_NBROM_PHY_ADDRESS */

/**
 * system SRAM physical address
 */
#define CONFIG_SRAM_PHY_ADDRESS 0x00100000

/**
 * aon SRAM physical address (8850)
 */
#define CONFIG_AON_SRAM_PHY_ADDRESS 0x50800000

/**
 * cp SRAM physical address (8850)
 */
#define CONFIG_CP_SRAM_PHY_ADDRESS 0x10100000

/**
 * NOR flash physical address
 */
#define CONFIG_NOR_PHY_ADDRESS 0x60000000

/**
 * external NOR flash physical address
 */
#define CONFIG_NOR_EXT_PHY_ADDRESS 0x70000000

/**
 * PSRAM or DDR physical address
 */
#define CONFIG_RAM_PHY_ADDRESS 0x80000000

/* #undef CONFIG_RAM_HEADER_SIZE */

/**
 * GIC base address
 */
#define CONFIG_GIC_BASE_ADDRESS 0x00801000

/**
 * GIC CPU interface offset
 */
#define CONFIG_GIC_CPU_INTERFACE_OFFSET 0x00001000

/**
 * GIC priority bits, unique level is (1<<CONFIG_GIC_PRIORITY_BITS)
 */
#define CONFIG_GIC_PRIORITY_BITS 5

/**
 * NVIC priority bits, unique level is (1<<CONFIG_NVIC_PRIORITY_BITS)
 */
/* #undef CONFIG_NVIC_PRIORITY_BITS */

/**
 * Interrupt count
 */
#define CONFIG_IRQ_COUNT 97

/**
 * system ROM size
 */
#define CONFIG_ROM_SIZE 0x10000

/**
 * NBROM size
 */
/* #undef CONFIG_NBROM_SIZE */

/**
 * whether dual bootloader is supported
 *
 * Dual bootloader needs system ROM support.
 */
//#define CONFIG_DUAL_BOOTLOADER_ENABLE


/**
 * NOR flash address of bootloader
 *
 * When dual bootloader is supported, it is the same as
 * \p CONFIG_BOOT1_FLASH_ADDRESS.
 */
#define CONFIG_BOOT_FLASH_ADDRESS 0x60020000

/**
 * NOR flash address of boot1 in dual bootloader
 */
/* #undef CONFIG_BOOT1_FLASH_ADDRESS */

/**
 * NOR flash address of boot2 in dual bootloader
 */
/* #undef CONFIG_BOOT2_FLASH_ADDRESS */

/**
 * bootloader reserved size on NOR flash
 *
 * When dual bootloader is supported, this is the total reserved NOR flash
 * size for bootloader.
 */
#define CONFIG_BOOT_FLASH_SIZE 0x10000

/**
 * bootloader maximum size
 *
 * When dual bootloader is enabled, it is \p CONFIG_BOOT_FLASH_SIZE/2.
 * When dual bootloader isn't enabled, it is \p CONFIG_BOOT_FLASH_SIZE.
 *
 * The image header size is included.
 */
/* #undef CONFIG_BOOTLOADER_SIZE */

/**
 * support switching betweend gsm and lte during runtime
 */
/* #undef CONFIG_GSM_LTE_SWITCH_SUPPORT */

/**
 * support decrease the ipc dl and ul speed to 2M bps
 */
/* #undef CONFIG_DL_UL_SPEED_2MBPS */


/**
 * support limit the ipc buf size
 */
/* #undef CONFIG_IPC_SIZE_LIMIT */

/**
 * tz support(8850)
 */
/* #undef CONFIG_TRUSTZONE_SUPPORT */

/**
 * NOR flash address of spl
 *
 * When dual bootloader is supported, it is the same as
 * \p CONFIG_SPL1_FLASH_ADDRESS.
 */
#define CONFIG_SPL_FLASH_ADDRESS 0x60000000

/**
 * NOR flash address of spl1 in dual bootloader
 */
#define CONFIG_SPL1_FLASH_ADDRESS 0x60000000

/**
 * NOR flash address of spl2 in dual bootloader
 */
#define CONFIG_SPL2_FLASH_ADDRESS 0x60010000

/**
 * spl reserved size on NOR flash
 *
 * When dual spl is supported, this is the total reserved NOR flash
 * size for bootloader.
 */
#define CONFIG_SPL_FLASH_SIZE 0x20000

/**
 * spl reserved size on NOR flash
 * */
#define CONFIG_SPL_SIZE 0x10000
/**
 * NOR flash physical address of sml
 */

/* #undef CONFIG_TZ_SML_FLASH_ADDRESS */

/**
 * sml maximum size
 */

/* #undef CONFIG_TZ_SML_FLASH_SIZE */

/**
 * NOR flash physical address of sml
 */

/* #undef CONFIG_TZ_TOS_FLASH_ADDRESS */

/**
 * sml maximum size
 */

/* #undef CONFIG_TZ_TOS_FLASH_SIZE */

/**
 * NOR flash physical address of tfm
 */

/* #undef CONFIG_TFM_FLASH_ADDRESS */

/**
 * tfm maximum size
 */

/* #undef CONFIG_TFM_FLASH_SIZE */


/**
 * NOR flash physical address of s_storage
 */

/* #undef CONFIG_S_STORAGE_FLASH_ADDRESS */


/**
 * reserved NOR flash size for s_storage
 */

/* #undef CONFIG_S_STORAGE_FLASH_SIZE */

/**
 * NOR flash physical address of application
 */
#define CONFIG_APP_FLASH_ADDRESS 0x60030000

/**
 * reserved NOR flash size for application
 */
#define CONFIG_APP_FLASH_SIZE 0x280000

/**
 * NOR flash address of loadable app image
 */
#define CONFIG_APPIMG_FLASH_ADDRESS 0x602B0000

/**
 * reserved NOR flash size for loadable app image
 */
#define CONFIG_APPIMG_FLASH_SIZE 0x220000

/**
 * NOR flash physical address of openCPU
 */
/* #undef CONFIG_OPENCPU_FLASH_ADDRESS */

/**
 * reserved NOR flash size for openCPU
 */
/* #undef CONFIG_OPENCPU_FLASH_SIZE */

/**
 * NOR flash address of system file system
 *
 * The mount point of system file system is \p CONFIG_FS_SYS_MOUNT_POINT.
 */
#define CONFIG_FS_SYS_FLASH_ADDRESS 0x604d0000

/**
 * reserved NOR flash size for system file system
 */
#define CONFIG_FS_SYS_FLASH_SIZE 0x250000

/**
 * NOR flash address of modem image file system
 *
 * The mount point of modem image file system is \p CONFIG_FS_MODEM_MOUNT_POINT.
 */
#define CONFIG_FS_MODEM_FLASH_ADDRESS 0x60720000

/**
 * reserved NOR flash size for modem image file system
 */
#define CONFIG_FS_MODEM_FLASH_SIZE 0xc0000

/**
 * NOR flash address of factory file system
 *
 * The mount point of factory file system is \p CONFIG_FS_FACTORY_MOUNT_POINT.
 */
#define CONFIG_FS_FACTORY_FLASH_ADDRESS 0x607e0000

/**
 * reserved NOR flash size for factory file system
 */
#define CONFIG_FS_FACTORY_FLASH_SIZE 0x20000

/**
 * external NOR flash physical address of application
 */
/* #undef CONFIG_APP_FLASH2_ADDRESS */

/**
 * reserved external NOR flash size for application
 */
/* #undef CONFIG_APP_FLASH2_SIZE */

/**
 * mount point of modem image file system
 */
#define CONFIG_FS_MODEM_MOUNT_POINT "/modem"

/**
 * mount point of system file system
 */
#define CONFIG_FS_SYS_MOUNT_POINT "/"

/**
 * mount point of factory file system
 */
#define CONFIG_FS_FACTORY_MOUNT_POINT "/factory"

/**
 * directory for modem running NV
 */
#define CONFIG_FS_MODEM_NVM_DIR "/runningnv"

/**
 * directory for application NV
 */
#define CONFIG_FS_AP_NVM_DIR "/nvm"

/**
 * directory for firmware update
 *
 * Firmware update related files will be located under this directory,
 * including the differential pack file, and intermediate files.
 */
#define CONFIG_FS_FOTA_DATA_DIR "/fota"

/**
 * whether loading app image from NOR flash is enabled
 */
#define CONFIG_APPIMG_LOAD_FLASH

/**
 * whether loading app image from file is enabled
 *
 * When loading app image from file, the image will be loaded to RAM, and
 * execute on RAM.
 */
/* #undef CONFIG_APPIMG_LOAD_FILE */

/**
 * whether loading boot image is disabled
 */
/* #undef CONFIG_BOOT_FLASH_DISABLE */

/**
 * Whether to verify app image signature when secure boot is enabled
 */
#define CONFIG_APPIMG_SIGCHECK_ENABLE

/**
 * loadable app image file name
 */
/* #undef CONFIG_APPIMG_LOAD_FILE_NAME */

/**
 * reserved memory size for blue screen. For ARM v7A, the size is blue screen
 * data size plus blue screen stack size. For ARM v8M, it is just the blue
 * screen data size.
 */
#define CONFIG_BLUE_SCREEN_SIZE 0x600

/**
 * whether to enable blue screen backup
 */
#define CONFIG_BSCORE_ENABLE

/**
 * whether blue screen backup located on sram
 */
/* #undef CONFIG_BSCORE_ON_SRAM */

/**
 * blue screen core total size
 */
#define CONFIG_BSCORE_SIZE 0x2000

/**
 * blue screen core stack dump ahead offset
 */
#define CONFIG_BSCORE_STACK_EXTRA 0x200

/**
 * blue screen core stack dump size
 */
#define CONFIG_BSCORE_STACK_SIZE 0xE00

/**
 * blue screen core cp stack dump size
 */
#define CONFIG_BSCORE_CP_STACK_SIZE 0x600

/**
 * blue screen core profile size
 */
#define CONFIG_BSCORE_PROFILE_SIZE 0x400

/**
 * enable med encrypt data or not (8811)
 */
/* #undef CONFIG_MED_CODE_ENCRYPT */

/**
 * Power down flash in low power mode (8910)
 */
/* #undef CONFIG_FLASH_LP_POWER_DOWN */

/**
 * Load flash and psram infomation to share mem
 */
#define CONFIG_SHAREMEM_EMMCDDR_INFO

/**
 * PSRAM enter half sleep in low power mode (8910)
 */
/* #undef CONFIG_PSRAM_LP_HALF_SLEEP */

/**
 * whether to support PM2 cold boot sleep mode
 */
/* #undef CONFIG_PSM_PM2BOOT_ENABLE */

/**
 * whether to support RC32k PSM sleep mode
 */
/* #undef CONFIG_RC32k_SLEEP_ENABLE */

/**
 * whether to check cp images signature
 */
/* #undef CONFIG_CPBIN_SIGCHECK */

/**
 * keyled power is used for RF switch (8910)
 */
/* #undef CONFIG_BOARD_KPLED_USED_FOR_RF_SWITCH */

/**
 * force pull up for gpio4 in iomux (8910)
 */
/* #undef CONFIG_GPIO4_FORCE_PULL_UP */

/**
 * gpio number for pmic interrupt (8910)
 */
/* #undef CONFIG_PMIC_INTR_GPIO */

/**
 * enable camera power at initialization (8910)
 */
/* #undef CONFIG_CAMA_POWER_ON_INIT */

/**
 * whether camera clock is needed for audio (8910)
 */
/* #undef CONFIG_CAMA_CLK_FOR_AUDIO */

/**
 * use 26MHz DCXO, rather than VCXO (8909)
 */
/* #undef CONFIG_CRYSTAL_WITHOUT_OSCILLATOR */

/**
 * RF use LDO (8909)
 */
/* #undef CONFIG_RF_LDO_MODE */

/**
 * hardware supported SIM count
 *
 * This is not the supported SIM count of protocol stack and application.
 */
/* #undef CONFIG_HAL_SIM_COUNT */

/**
 * whether GSM is supported
 */
/* #undef CONFIG_GSM_SUPPORT */

/**
 * whether NBIOT is supported
 */
/* #undef CONFIG_LTE_NBIOT_SUPPORT */

/**
 * whether to enter RF calibration mode by gpio
 */
/* #undef CONFIG_BOARD_ENTER_CALIB_BY_GPIO */

/**
 * the gpio for detect entering RF calibration mode
 */
/* #undef CONFIG_BOARD_ENTER_CALIB_GPIO */

/**
 * whether to support SIM1 hot plug
 */
#define CONFIG_BOARD_SUPPORT_SIM1_DETECT

/**
 * gpio for SIM1 hot plug detect
 */
#define CONFIG_BOARD_SIM1_DETECT_GPIO 8

/**
 * whether to support SIM2 hot plug
 */
/* #undef CONFIG_BOARD_SUPPORT_SIM2_DETECT */

/**
 * gpio for SIM2 hot plug detect
 */
/* #undef CONFIG_BOARD_SIM2_DETECT_GPIO */

/**
 * whether debughost event is enabled
 */
#define CONFIG_ENABLE_DEBUGHOST_EVENT

/**
 * whether power key is tied to GND
 */
/* #undef CONFIG_PWRKEY_TIE_GND */

/**
 * whether power key can wakeup psm mode
 */
#define CONFIG_PWRKEY_WAKEUP_PSM

/**
 * whether power key can power up after shutdown (8910)
 */
#define CONFIG_PWRKEY_POWERUP

/**
 * whether wakeup pin (psm_ext_int) can power up after shutdown
 */
/* #undef CONFIG_WAKEUP_PIN_POWERUP */

/**
 * whether charger can power up after shutdown
 */
#define CONFIG_CHARGER_POWERUP

/**
 * whether normal power off
 */
/* #undef CONFIG_SHUTDOWN_NORMAL_POWER_OFF */

/**
 * whether there is external flash on board
 */
/* #undef CONFIG_BOARD_WITH_EXT_FLASH */

/**
 * external flash size
 */
/* #undef CONFIG_BOARD_EXT_FLASH_SIZE */

/**
 * external flash frequency
 */
/* #undef CONFIG_BOARD_EXT_FLASH_FREQ */

/**
 * whether external flash working in quad mode
 */
/* #undef CONFIG_BOARD_EXT_FLASH_QUAD_MODE */

/**
 * external flash controller sample delay
 */
/* #undef CONFIG_BOARD_EXT_FLASH_SAMPLE_DELAY */

/**
 * external flash controller clk divider
 */
/* #undef CONFIG_BOARD_EXT_FLASH_CLK_DIVIDER */

/**
 * external flash is connected to gpio 20-25 (8811)
 */
/* #undef CONFIG_BOARD_EXT_FLASH_GPIO20 */

/**
 * whether application will split into external flash
 */
/* #undef CONFIG_APP_FLASH2_ENABLED */

/**
 * whether there are file system on external flash
 */
/* #undef CONFIG_FS_EXT_ENABLED */

/**
 * whether support ims delta nv
 */
/* #undef CONFIG_SUPPORT_IMS_DELTA_NV */

/**
 * whether support inner delta nv
 */
#define CONFIG_SUPPORT_INNER_DELTA_NV

/**
 * whether gpio is used for Vbat_RF switch
 */
/* #undef CONFIG_GPIO_USED_FOR_VBAT_RF_SWITCH */

/**
 * Used gpio x as Vbat_RF swtich
 */
/* #undef CONFIG_GPIO_X_USED_FOR_VBAT_RF */

/**
 * dcdc LV support (8811)
 */
/* #undef CONFIG_LV_SUPPORT */

/**
 * GPIO to indicate PM2 (8811)
 */
/* #undef CONFIG_PM2_IND_GPIO */

/**
 * GPIO to indicate PM3 (8811)
 */
/* #undef CONFIG_PM3_IND_GPIO */

/**
 * sys_wdt timeout
 */
#define CONFIG_SYS_WDT_TIMEOUT 10000

/**
 * sys_wdt feed interval
 */
#define CONFIG_SYS_WDT_FEED_INTERVAL 5000

/**
 * unity output to uart
 */
#define CONFIG_UNITY_OUTPUT_UART

/**
 * unity unit test uart
 */
#define CONFIG_UNITY_UART_NAME DRV_NAME_UART1

/**
 * unity unit test uart baud rate
 */
#define CONFIG_UNITY_UART_BAUD 115200

/**
 * tfm support
 */
/* #undef CONFIG_TFM_SUPPORT */

/**
 * tfm test support
 */
/* #undef CONFIG_TFM_TEST_SUPPORT */

/**
 * tfm test secure framework
 */
/* #undef CONFIG_TEST_FRAMEWORK_S */

/**
 * tfm test non-secure framework
 */
/* #undef CONFIG_TEST_FRAMEWORK_NS */

/* #undef CONFIG_CP_SIGN_ENABLE */

/**
 * ap must wakeup before bt sleeptime expire
 */
/* #undef CONFIG_AP_SLEEP_SYNC_WITH_BT_SLEEPTIME */

/**
 * whether to identify 26M/32k clock by gpio and RTC mode
 */
#define CONFIG_IDENTIFY_CLOCK

/**
 * trace sn space size, for sharing with tfm (8811)
 */
/* #undef CONFIG_TRACE_SN_SIZE */

/**
 * trace sn address, for sharing with tfm (8811)
 */
/* #undef CONFIG_TRACE_SN_START_ADR */

/**
 * whether use 8850 V3 board
 */
/* #undef CONFIG_CHIP_8850_V3_BOARD */

/**
 * whether use 8850 V4 board
 */
#define CONFIG_CHIP_8850_V4_BOARD
/**
 * whether use 8850 V111 board
 */
/* #undef CONFIG_CHIP_8850_V111_BOARD */

/**
* whether use 8850CM target
 */
 #define CONFIG_8850CM_TARGET

 /**
* whether use 8850BM_C1 target
 */
/* #undef CONFIG_8850BM_C1 */

/**
 * lzmar block_size (8850/8910)
 */
#define CONFIG_LZMAR_BLOCK_SIZE 0x10000

/**
 * lzmar dict_size (8850/8910)
 */
#define CONFIG_LZMAR_DICT_SIZE 0x400

/**
 * aps psram die drv config
 */
#define CONFIG_APS_PSRAM_DIE_DRV

#endif
