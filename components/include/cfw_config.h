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

#ifndef _CFW_CONFIG_H_
#define _CFW_CONFIG_H_

#include "hal_config.h"

// Auto generated. Don't edit it manually!
#define __PROCESS_INFO_IN_FLASH CONFIG_PROCESS_INFO_IN_FLASH_8850CM
/* #undef CONFIG_GSM_SUPPORT */
/* #undef CONFIG_CFW_VOLTE_SUPPORT */
/* #undef CONFIG_VOLTE_SUPPORT */
/* #undef CONFIG_LTE_NBIOT_SUPPORT */
#define CONFIG_LTE_SUPPORT
#define CONFIG_LIMITED_NUMBER_SMS_SUPPORT
#define LTE_SUPPORT
/* #undef LTE_NBIOT_SUPPORT */
#define DEDICATED_BEARER_SUPPORT
/* #undef CFW_VOLTE_SUPPORT */
/* #undef VOLTE_SUPPORT */
#define CFW_GPRS_SUPPORT
#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
#define PHONE_SMS_ENTRY_COUNT 20
#else
#ifdef CONFIG_LIMITED_NUMBER_SMS_SUPPORT
#define PHONE_SMS_ENTRY_COUNT 80
#elif defined(CONFIG_8850BM_C1)
#define PHONE_SMS_ENTRY_COUNT 10
#else
#define PHONE_SMS_ENTRY_COUNT 200
#endif
#endif

#define DEFAULT_SIM_SLOT 0

#define CONFIG_NUMBER_OF_SIM 2
/* #undef CONFIG_EVENT_NAME_ENABLED */

#define CONFIG_CFW_PENDING_UTI_COUNT 64

/* #undef CONFIG_CFW_SKIP_NV_SAVE_RESTORE */

/* #undef CONFIG_CFW_CALL_WITH_RPC */
/* #undef CONFIG_CFW_DEBUG_IPFILTER */

/* #undef CONFIG_RPM_SUPPORT */
/* #undef RPM_SUPPORT */

/* #undef CONFIG_CSG_SUPPORT */
/* #undef CSG_SUPPORT */

#define CONFIG_CFW_CM_SRV_SUPPORT
#define CONFIG_CFW_SS_SRV_SUPPORT
#define CONFIG_CFW_PBK_SRV_SUPPORT
#define CONFIG_CFW_CB_SRV_SUPPORT

/* #undef CONFIG_CFW_SUPPORT_GLOBAL_APN */

/* #undef CONFIG_DUAL_SIM_SUPPORT */

#define FUNC_SUPPORT_SIMLOCK
#define CONFIG_FUNC_SUPPORT_SIMLOCK

/* #undef CONFIG_FUNC_SUPPORT_SIMLOCK_AP */
/* #undef CONFIG_FUNC_SUPPORT_SIMLOCK_CP */

#define CONFIG_CFW_SMS_ME_SUPPORT

/* #undef CONFIG_CFW_ON_AP */
/* #undef __PROCESS_INFO_IN_FLASH */

#define CONFIG_CFW_CC_SUPPORT
#define CONFIG_CFW_SS_SUPPORT
/* #undef CONFIG_CFW_STK_SUPPORT */

#endif
