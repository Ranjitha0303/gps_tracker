#include "nwy_test_cli_adpt.h"
#include "nwy_test_cli_loc.h"

#ifdef NWY_OPEN_TEST_GNSS_LOC
#include "nwy_loc.h"
#endif


#ifdef NWY_OPEN_TEST_GNSS_LOC
void nwy_test_cli_gnss_event_cb(char *nmea_msg)
{
    nwy_test_cli_echo("%s\n", nmea_msg);
}

void nwy_test_cli_gnss_recv_cb_reg(void)
{
    int ret = 0;
    ret = nwy_loc_recv_cb_reg(nwy_test_cli_gnss_event_cb);
    if (ret == 0)
        nwy_test_cli_echo("\r\nTest loc add event handle success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc add event handle error!\r\n");
    return ;
}

void nwy_test_cli_loop_output_stop(void)
{
    nwy_loc_loop_output_stop();
    return ;
}

int nwy_test_cli_gnss_open(void)
{
    int ret = 0;
    ret = nwy_loc_navigation_start();

    if(ret == 0)
        nwy_test_cli_echo("\r\nTest loc open success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc open error!\r\n");

    return ret;
}

int nwy_test_cli_gnss_close(void)
{
    int ret = 0;
    ret = nwy_loc_navigation_stop();

    if(ret == 0)
        nwy_test_cli_echo("\r\nTest loc close success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc close error!\r\n");

    return ret;
}

int nwy_test_cli_gnss_start_mode(void)
{
    char *opt = NULL;
    int ret = 0;
    int startmode = 1;
    opt = nwy_test_cli_input_gets("\r\nPlease input startmode(0-hot start, 1-warm start, 2-cold start):");
    startmode = atoi(opt);
    nwy_test_cli_echo("\r\nTest startmode = %d\r\n", startmode);

    ret = nwy_loc_startup_set(startmode);

    nwy_test_cli_echo("\r\nTest loc startmode success!\r\n");

    return ret;
}

int nwy_test_cli_gnss_mode_set(void)
{
    char *opt = NULL;
    int ret = 0;
    int gnss_mode = 0;
    opt = nwy_test_cli_input_gets("\r\nPlease input loc gnss_mode(0 GPS, 1 BD, 2 GPS+BD):");

    gnss_mode = atoi(opt);
    nwy_test_cli_echo("\r\nTest gnss_mode = %d\r\n", gnss_mode);

    ret = nwy_loc_mode_set(gnss_mode);
    nwy_test_cli_echo("\r\nTest loc set gnss mode sucess!\r\n");
    return ret;
}

void nwy_test_cli_gnss_nmea_get(void)
{
    char *opt = NULL;
    int ret = 0;
    int type = 0;
    char nmea_buf[2048+1] = {0};

    nwy_test_cli_echo("\r\nNMEA type:\r\n");
    nwy_test_cli_echo("01 GGA, 02 GLL, 04 GSA, 08 GSV \r\n");
    nwy_test_cli_echo("16 RMC, 32 VTG, 64 ZDA, 4095 all \r\n");

    opt = nwy_test_cli_input_gets("\r\nPlease input loc nmea type: ");

    type = atoi(opt);
    nwy_test_cli_echo("\r\nTest NMEA type = 0x%x", type);

    memset(nmea_buf, 0, sizeof(nmea_buf));
    ret = nwy_loc_nmea_get(type, nmea_buf);
    if(ret == 0)
    {
        nwy_test_cli_echo("\r\n %s", nmea_buf);
    }
    else
    {
        nwy_test_cli_echo("\r\nTest loc get NMEA error!");
    }

    return;
}


#if 0

int nwy_test_cli_agps_start(void)
{

}
/* AGPS interface */
#endif

#endif

