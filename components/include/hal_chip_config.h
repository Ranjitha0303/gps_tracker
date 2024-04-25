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

#ifndef _HAL_CHIP_CONFIG_H_
#define _HAL_CHIP_CONFIG_H_

// Auto generated. Don't edit it manually!

/**
 * system ROM load address
 */
#define CONFIG_ROM_SRAM_LOAD_ADDRESS 0x50800000

/**
 * system ROM load maximum size
 */
#define CONFIG_ROM_LOAD_SIZE 0x14000

/**
 * system ROM SRAM data size can be accessed in bootloader
 */
#define CONFIG_ROM_SRAM_DATA_SIZE 0xc00

/**
 * bootloader image address, on AON SRAM
 */
/* #undef CONFIG_BOOT_IMAGE_START */

/**
 * FDL image address, on AON SRAM
 */
#define CONFIG_NORFDL_IMAGE_START 0x50800000

/**
 * AON SRAM code address
 */
#define CONFIG_AON_SRAM_CODE_ADDRESS 0x50800000

/**
 * AON SRAM code size
 */
#define CONFIG_AON_SRAM_CODE_SIZE 0x2000

/**
 * AON SRAM shared memory address
 */
#define CONFIG_AON_SRAM_SHMEM_ADDRESS 0x50802000

/**
 * AON SRAM shared memory size
 */
#define CONFIG_AON_SRAM_SHMEM_SIZE 0x1000

/**
 * AON SRAM for CP address
 */
#define CONFIG_AON_SRAM_CP_ADDRESS 0x50803000

/**
 * AON SRAM for CP size
 */
#define CONFIG_AON_SRAM_CP_SIZE 0x11000

/**
 * CP RAM address
 */
#define CONFIG_CP_RAM_ADDRESS 0x80000000

/**
 * CP RAM size
 */
#define CONFIG_CP_RAM_SIZE 0x2EC000

/**
 * APP RAM address
 */
#define CONFIG_APP_RAM_ADDRESS 0x802ec000

/**
 * APP RAM size
 */
#define CONFIG_APP_RAM_SIZE 0x424000

/**
 * File APPIMG reserved RAM address
 */
#define CONFIG_APP_FILEIMG_RAM_ADDRESS 0x80800000

/**
 * File APPIMG reserved RAM size
 */
#define CONFIG_APP_FILEIMG_RAM_SIZE  0x0

/**
 * Flash APPIMG reserved RAM address
 */
#define CONFIG_APP_FLASHIMG_RAM_ADDRESS 0x80710000

/**
 * Flash APPIMG reserved RAM size
 */
#define CONFIG_APP_FLASHIMG_RAM_SIZE 0xF0000

/**
 * gnss reserved RAM address
 */
#define CONFIG_GNSS_RAM_ADDRESS 0x80710000

/**
 * gnss reserved RAM size
 */
#define CONFIG_GNSS_RAM_SIZE  0x0

/**
 * trust zone tos reserved RAM address
 */
#define CONFIG_APP_TRUSTZONE_TOS_RAM_ADDRESS 0x80800000

/**
 * trust zone tos reserved RAM size
 */
#define CONFIG_APP_TRUSTZONE_TOS_RAM_SIZE  0x0

/**
 * trust zone sml reserved RAM address
 */
#define CONFIG_APP_TRUSTZONE_SML_RAM_ADDRESS 0x80800000

/**
 * trust zone sml reserved RAM size
 */
#define CONFIG_APP_TRUSTZONE_SML_RAM_SIZE  0x0

/**
 * openCPU reserved RAM address
 */
#define CONFIG_OPENCPU_RAM_ADDRESS 0x80710000

/**
 * openCPU reserved RAM size
 */
#define CONFIG_OPENCPU_RAM_SIZE  0x0

/**
 * IRQ stack size
 */
#define CONFIG_IRQ_STACK_SIZE 0x200

/**
 * SVC stack size (8850)
 */
#define CONFIG_SVC_STACK_SIZE 0x600

/**
 * appll frequency
 */
#define CONFIG_DEFAULT_APPLL_FREQ 1000000000

/**
 * APA5 cpu frequency
 */
#define CONFIG_DEFAULT_APA5_FREQ 500000000

/**
 * default AXI bus frequency
 */
#define CONFIG_DEFAULT_AP_BUS_FREQ 250000000

/**
 * config rc26m frequency
 */
#define CONFIG_RC26M_CALIB_FIXED_FREQ 14745600

/**
 * maximum AXI bus frequency
 */
#define CONFIG_MAX_AP_BUS_FREQ 250000000

/**
 * AHB bus frequency
 */
#define CONFIG_DEFAULT_SYSAHB_FREQ 20000000
#endif
