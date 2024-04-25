#ifndef __NWY_TEST_CLI_LOC_H__
#define __NWY_TEST_CLI_LOC_H__

#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "nwy_test_cli_adpt.h"

#ifdef NWY_OPEN_TEST_GNSS_LOC
#include "nwy_loc.h"

void nwy_test_cli_gnss_event_cb(char *nmea_msg);
#endif


#endif
