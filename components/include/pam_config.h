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

#ifndef _PAM_CONFIG_H_
#define _PAM_CONFIG_H_

// Auto generated. Don't edit it manually!

#define CONFIG_PAM_HANDSHAKE_LTE_TIMEOUT 10000

#define CONFIG_PAM_GNSS_CONTEXT_TIMEOUT 10000

#define CONFIG_PAM_GNSS_EVENT_MAX_COUNT 50

#define CONFIG_PAM_GNSS_PENDING_EVID_COUNT 64

#define CONFIG_PAM_MEM_FREE_LATER_COUNT 16

#define CONFIG_PAM_WIFISCAN_TIMEOUT 20000

/**
 * whether AT command pam api can be used
 */
/* #undef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT */

#define LOG_TAG_PAM OSI_MAKE_LOG_TAG('P', 'A', 'M', ' ')
#endif