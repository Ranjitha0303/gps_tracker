/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_iot_log.h
 * @brief Logging macros for the SDK.
 * This file defines common logging macros with log levels to be used within the SDK.
 * These macros can also be used in the IoT application code as a common way to output
 * logs.  The log levels can be tuned by modifying the makefile.  Removing (commenting
 * out) the IOT_* statement in the makefile disables that log level.
 *
 * It is expected that the macros below will be modified or replaced when porting to
 * specific hardware platforms as printf may not be the desired behavior.
 */

#ifndef _IOT_LOG_H
#define _IOT_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <nwy_config.h>
#include "osi_log.h"
/**
 * @brief Debug level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#define ENABLE_IOT_DEBUG
#define ENABLE_IOT_INFO
#define  ENABLE_IOT_WARN
#define  ENABLE_IOT_ERROR
//#define  ENABLE_IOT_TRACE

#ifndef FEATURE_NWY_OPEN_SDK
extern void nwy_log(unsigned int flag, char * fmt, ...);
#endif
void nwy_aws_log_print(const char *format, ...);

#define print nwy_aws_log_print

#ifdef ENABLE_IOT_DEBUG
#define IOT_DEBUG(...)    \
    {\
    print("DEBUG:   %s L#%d ", __func__, __LINE__);  \
    print(__VA_ARGS__); \
    print("\n"); \
    }
#else
#define IOT_DEBUG(...)
#endif

/**
 * @brief Debug level trace logging macro.
 *
 * Macro to print message function entry and exit
 */
#ifdef ENABLE_IOT_TRACE
#define FUNC_ENTRY    \
    {\
    print("FUNC_ENTRY:   %s L#%d \n", __func__, __LINE__);  \
    }
#define FUNC_EXIT    \
    {\
    print("FUNC_EXIT:   %s L#%d \n", __func__, __LINE__);  \
    }
#define FUNC_EXIT_RC(x)    \
    {\
    print("FUNC_EXIT:   %s L#%d Return Code : %d \n", __func__, __LINE__, x);  \
    return x; \
    }
#else
#define FUNC_ENTRY

#define FUNC_EXIT
#define FUNC_EXIT_RC(x) { return x; }
#endif

/**
 * @brief Info level logging macro.
 *
 * Macro to expose desired log message.  Info messages do not include automatic function names and line numbers.
 */
#ifdef ENABLE_IOT_INFO
#define IOT_INFO(...)    \
    {\
    print(__VA_ARGS__); \
    print("\n"); \
    }
#else
#define IOT_INFO(...)
#endif

/**
 * @brief Warn level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef ENABLE_IOT_WARN
#define IOT_WARN(...)   \
    { \
    print("WARN:  %s L#%d ", __func__, __LINE__);  \
    print(__VA_ARGS__); \
    print("\n"); \
    }
#else
#define IOT_WARN(...)
#endif

/**
 * @brief Error level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef ENABLE_IOT_ERROR
#define IOT_ERROR(...)  \
    { \
    print("ERROR: %s L#%d ", __func__, __LINE__); \
    print(__VA_ARGS__); \
    print("\n"); \
    }
#else
#define IOT_ERROR(...)
#endif




#ifdef __cplusplus
}
#endif

#endif // _IOT_LOG_H

