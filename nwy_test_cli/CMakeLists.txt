# Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

set(NWY_CLI_DIR ${SOURCE_TOP_DIR}/nwy_test_cli)
include_directories(${NWY_CLI_DIR}/comm)
include_directories(${NWY_CLI_DIR}/modules)
add_appimg(${target} ${flash_ldscript} 
		   ${NWY_CLI_DIR}/comm/nwy_test_cli_adpt.c
           ${NWY_CLI_DIR}/comm/nwy_test_cli_menu.c
           ${NWY_CLI_DIR}/comm/nwy_test_cli_utils.c
           ${NWY_CLI_DIR}/modules/nwy_test_cli_driver.c
           ${NWY_CLI_DIR}/modules/nwy_test_cli_mobile.c
           ${NWY_CLI_DIR}/modules/nwy_test_cli_os.c
	       ${NWY_CLI_DIR}/modules/nwy_test_cli_tcpip.c
	       ${NWY_CLI_DIR}/modules/nwy_test_cli_sdw.c
           ${NWY_CLI_DIR}/modules/nwy_test_cli_lbs.c
		   ${NWY_CLI_DIR}/modules/nwy_test_cli_iot.c
)

