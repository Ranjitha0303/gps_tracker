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

#ifndef _FUPDATE_CONFIG_H_
#define _FUPDATE_CONFIG_H_

#include "hal_config.h"

// Auto generated. Don't edit it manually!

/**
 * string "update pack file name" default "fota.pack"
 * This is the file name of firmware upgrade pack file name. It is
 * located under CONFIG_FS_FOTA_DATA_DIR. The configuration should
 * be the same in application and bootloader.
 *
 * Don't change it unless you know what you are doing.
 */
#define CONFIG_FUPDATE_PACK_FILE_NAME "fota.pack"
/**
 * string "update stage file name" default "fota.stage"
 * This file will record firmware updata stage. It is located under
 * CONFIG_FS_FOTA_DATA_DIR. The configuration should be the same in
 * application and bootloader.
 *
 * Don't change it unless you know what you are doing.
 */
#define CONFIG_FUPDATE_STAGE_FILE_NAME "fota.stage"
/**
 * string "update temporal file name" default "fota.tmp"
 * This is temporal file create during update in bootloader.
 *
 * Don't change it unless you know what you are doing.
 */
#define CONFIG_FUPDATE_TEMP_FILE_NAME "fota.tmp"
/**
 * string "update signature file name" default "fota.sign"
 * This is signature file name of the firmware update pack file.
 * It contains the signature of the update pack file. 
 *
 * Don't change it unless you know what you are doing.
 */
#define CONFIG_FUPDATE_SIGN_FILE_NAME "fota.sign"
/**
 * bool "support flash differential update"
 * Support differential update of flash regions.
 */
#define CONFIG_FUPDATE_SUPPORT_FLASH_PATCH
/**
 * bool "support file system differential update"
 * Support differential update of file system. Only SFFS is supported.
 */
/* #undef CONFIG_FUPDATE_SUPPORT_FS_PATCH */
/**
 * bool "support blocked file"
 */
#define CONFIG_FUPDATE_SUPPORT_BLOCKED_FILE
/**
 * bool "support fota signature check"
 */
#define CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK
/**
 * bool "support fota package old/new version check"
 * If this option is enabled, it will check the versions of
 * new & old firmwares before upgrading the fota package. Forbid new
 * version being updated to old version by default.
 */
/* #undef CONFIG_FUPDATE_SUPPORT_VERSION_CHECK */
/**
 * bool "support fota anti rollback"
 * Version anti-Rollback will be actived if enabled.
 * Config version: menuconfig ->  Application start -> APP_VERSION
 */
/* #undef CONFIG_FUPDATE_SUPPORT_ANTI_ROLLBACK */
/**
 * bool "support fota check tmp space"
 * If this option is enabled, it will check unused flash size, if
 * flash size is not enough for fota, the update process will quit.
 */
#define CONFIG_FUPDATE_SUPPORT_CHECK_TMP_SPACE
/**
 * bool "fota check secure version"
 * If secure boot is enabled, fota will check the secure version of the new pac.
 * If the version < secure version in Efuse, fota upgrade process will be stoped.
 */
/* #undef CONFIG_FUPDATE_CHECK_SECURE_VERSION */

#define FUPDATE_PACK_FILE_NAME CONFIG_FS_FOTA_DATA_DIR "/" CONFIG_FUPDATE_PACK_FILE_NAME
#define FUPDATE_STAGE_FILE_NAME CONFIG_FS_FOTA_DATA_DIR "/" CONFIG_FUPDATE_STAGE_FILE_NAME
#define FUPDATE_TEMP_FILE_NAME CONFIG_FS_FOTA_DATA_DIR "/" CONFIG_FUPDATE_TEMP_FILE_NAME
#define LOG_TAG_FUPDATE OSI_MAKE_LOG_TAG('F', 'U', 'P', 'D')
#define FUPDATE_SIGN_FILE_NAME CONFIG_FS_FOTA_DATA_DIR "/" CONFIG_FUPDATE_SIGN_FILE_NAME
#endif
