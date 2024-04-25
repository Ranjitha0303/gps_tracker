#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_lbs_ext.h"
loc_info_t loc_info = {0};
char info_string[100] = {0};
//char info_string[100] = {'a',' ' ,'b'};
#ifdef CONFIG_SUPPORT_GNSS
#include "nwy_loc.h"
#endif
#ifdef CONFIG_NWY_SUPPORT_GNSS_EXTERNAL_CHIP
#include "nwy_loc_external.h"
#endif
#ifdef NWY_OPEN_TEST_GNSS
/**************************GNSS*********************************/
void test_loc_event_cb(NWY_GNSS_OUTPUT_INFO_IND_T * ind_msg)
{
     nwy_test_cli_echo("%s\n",ind_msg->nmea_data);
}

void nwy_test_cli_gnss_open()
{
    int ret = 0;
    char *opt = NULL;
    uint8_t startmode = 1;
    opt = nwy_test_cli_input_gets("\r\nPlease input startmode(1-cold star 0-warm star):");
    startmode = atoi(opt);
    nwy_test_cli_echo("\r\nTest startmode = %d!\r\n", startmode);

    ret = nwy_loc_start_navigation(startmode);

    if (ret == 0)
        nwy_test_cli_echo("\r\nTest loc open success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc open error!\r\n");
}

void nwy_test_cli_gnss_set_position_md()
{
    char *opt = NULL;
    uint8_t nSatType = 0;
    int ret = 0;
	
    opt = nwy_test_cli_input_gets("\r\nPlease input loc nSatType(1,2,3,4,5,8,9,13,16,17,18,19,20,21,24,25,29):");
    nSatType = atoi(opt);
    nwy_test_cli_echo("\r\nTest nSatType = %d!\r\n", nSatType);

    ret = nwy_loc_set_position_mode(nSatType);
    if (ret == 0)
        nwy_test_cli_echo("\r\nTest loc set position success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set position error!\r\n");
}

void nwy_test_cli_gnss_output_format()
{
    char *opt = NULL;
	uint16_t nOutputFormat = 0;
	uint32_t nTimeInterval = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease input loc nmea format(0-8):");
    nOutputFormat = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nPlease input loc nTimeInterval(1-32768):");
    nTimeInterval = atoi(opt);

	nwy_test_cli_echo("\r\nTest nOutputFormat = %d,nTimeInterval = %d!\r\n", nOutputFormat,nTimeInterval);
    ret = nwy_loc_output_format(nOutputFormat,nTimeInterval);
    if (ret == 0)
        nwy_test_cli_echo("\r\nTest loc set output format success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set output format error!\r\n");
}

void nwy_test_cli_gnss_delete_aiding_data()
{
    int ret = 0;

    ret = nwy_loc_delete_aiding_data();
    if (ret == 0)
        nwy_test_cli_echo("\r\nTest loc delete aiding data success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc delete aiding data error!\r\n");
}

void nwy_test_cli_gnss_nmea_info_parse()
{
    int ret = 0;
	char *opt = NULL;
    uint8_t nmode = 0;
    char data_title[32];
    char data[100];
	uint8_t n = 0;
    NWY_GNSS_INFO_T *gnssData = (NWY_GNSS_INFO_T *)malloc(sizeof(NWY_GNSS_INFO_T));
	memset(gnssData,0,sizeof(NWY_GNSS_INFO_T));
		
	opt = nwy_test_cli_input_gets("\r\nPlease input nmode(0-All Information 1-Positioning Information):");
	nmode = atoi(opt);
	gnssData->mode = nmode;
	
    nwy_test_cli_echo("\r\nTest nSatType = %d!\r\n", nmode);
	ret = nwy_loc_nmea_Info_parse(gnssData);
    if (ret == 0)
		nwy_test_cli_echo("\r\nTest loc nmea info parse success!\r\n");
	else
		nwy_test_cli_echo("\r\nTest loc nmea info parse error!\r\n");

	if(ret == 0)
	{
		char *data_title_temp = data_title;
		sprintf(data_title_temp, "GNSS Positioning Information:");
		nwy_test_cli_echo("\r\n %s\r\n", data_title);
		if (nmode == 0)
		{
			NWY_GNSS_INFO_DATA_T *gnssInfo = &(gnssData->locGnsInfo->gnss_data_info);
			char *data_temp1 = data;
			sprintf(data_temp1, "Longitude:%f, Latitude:%f, Altiude:%f", gnssInfo->longitude, gnssInfo->latitude, gnssInfo->altitude);
			nwy_test_cli_echo("\r\n %s\r\n", data);
	
			memset(data, 0, 100);
			NwyGnssTimeStamp *utc_time = &(gnssData->locGnsInfo->gnss_data_info.utc_time);
			char *data_temp2 = data;
			sprintf(data_temp2, "UTC Time: %04d/%02d/%02d  %02d:%02d:%02d", utc_time->wYear, utc_time->wMonth, utc_time->wDay, utc_time->wHour, utc_time->wMinute, utc_time->wSecond);
			nwy_test_cli_echo("\r\n %s\r\n", data);
	
			memset(data, 0, 100);
			char *data_temp3 = data;
			sprintf(data_temp3, "TTFF:%lu, SatelliteNum:%d, Speed:%f, Course:%f, HorizontalAccuracy:%f", gnssInfo->ttff_time, gnssInfo->satellite_num, gnssInfo->speed, gnssInfo->course, gnssInfo->m_nHorizontalAccuracy);
			nwy_test_cli_echo("\r\n %s\r\n", data);
	
			memset(data_title, 0, 32);
			char *satellite_title = data_title;
			sprintf(satellite_title, "Satellite Information:");
			nwy_test_cli_echo("\r\n %s\r\n", data_title);
	
			char *data_temp4 = data;
			NWY_GNSS_SATELLITE_INFO_T *satellite_info = gnssData->locGnsInfo->satellite_list_info.satellite_info;
			for (uint8_t n = 0; n < gnssInfo->satellite_num; n++)
			{
				sprintf(data_temp4, "PRN:%d, CN0:%d, Elevation:%d, Azimuth:%d, Used:%d", satellite_info[n].m_nSatelliteIdentifier, satellite_info[n].m_cn0, satellite_info[n].m_nElevation, satellite_info[n].m_nAzimuth, satellite_info[n].m_IsUsed);
				nwy_test_cli_echo("\r\n %s\r\n", data);
				memset(data, 0, 100);
			}
		}
		else
		{
			NWY_GNSS_INFO_DATA_T *gnssInfo = &(gnssData->locGnsInfo->gnss_data_info);
			char *data_temp1 = data;
			sprintf(data_temp1, "Longitude:%f, Latitude:%f, Altiude:%f", gnssInfo->longitude, gnssInfo->latitude, gnssInfo->altitude);
			nwy_test_cli_echo("\r\n %s\r\n", data);
	
			memset(data, 0, 100);
			NwyGnssTimeStamp *utc_time = &(gnssData->locGnsInfo->gnss_data_info.utc_time);
			char *data_temp2 = data;
			sprintf(data_temp2, "UTC Time: %04d/%02d/%02d  %02d:%02d:%02d", utc_time->wYear, utc_time->wMonth, utc_time->wDay, utc_time->wHour, utc_time->wMinute, utc_time->wSecond);
			nwy_test_cli_echo("\r\n %s\r\n", data);
		}
	}
	else
	{
		nwy_test_cli_echo("\r\nTest loc nmea info parse error!\r\n");
	}
    free(gnssData->locGnsInfo);
    free(gnssData);
}


void nwy_test_cli_gnss_nmea_data()
{
    int ret = 0;
	ret = nwy_loc_nmea_data_ind( test_loc_event_cb);
	if (ret == 0)
		nwy_test_cli_echo("\r\nTest loc get nmea success!\r\n");
	else
		nwy_test_cli_echo("\r\nTest loc get nmea error!\r\n");
}
void nwy_test_cli_gnss_set_priority()
{
    char *opt = NULL;
    uint8_t priority = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease set LTE_GNSS priority(0-3):");
    priority = atoi(opt);
    
    nwy_test_cli_echo("\r\nTest priority = %d!\r\n", priority);
    ret = nwy_loc_handle_priority(priority);
    if (ret == 0)
        nwy_test_cli_echo("\r\nTest set LTE_GNSS priority success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest set LTE_GNSS priority error!\r\n");

}
void nwy_test_cli_gnss_open_resume()
{
    char *opt = NULL;
    uint8_t enable = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease open resume(0-1):1-enable 0-disable");
    enable = atoi(opt);

    nwy_test_cli_echo("\r\nTest resume = %d!\r\n", enable);
    ret = nwy_loc_resume(enable);
    if (ret == 0)
        nwy_test_cli_echo("\r\nTest open resume success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest open resume error!\r\n");

}

void nwy_test_cli_gnss_set_server()
{
#ifdef NWY_OPEN_TEST_AGPS_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *opt = NULL;
    char *AGPS_URL = NULL;
    char *AGPS_USER = NULL;
    char *AGPS_PASS = NULL;
    int port = 0;
    int ret = 0;

    AGPS_URL = nwy_test_cli_input_gets("\r\nPlease input agps server url:");
    opt = nwy_test_cli_input_gets("\r\nPlease input agps server port:");

    port = atoi(opt);
    AGPS_USER = nwy_test_cli_input_gets("\r\nPlease input agps server user:");
    AGPS_PASS = nwy_test_cli_input_gets("\r\nPlease input agps server pass:");

    ret = nwy_loc_set_server(NWY_AGPS_URL, NWY_AGPS_PORT, NWY_AGPS_USER, NWY_AGPS_PASS);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc set server success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set server error!\r\n");
#endif
}

void nwy_test_cli_gnss_open_assisted()
{
#ifdef NWY_OPEN_TEST_AGPS_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *opt = NULL;
    int value = 0;
    int ret = 0;

    if (1 == nwy_test_cli_check_data_connect())
    {
        nwy_pdp_set_status(NWY_PDP_CONNECTED);
    }
    else
    {
        nwy_test_cli_echo("\r\n need module dail at+xiic = 1");
        nwy_test_cli_send_virt_at();
    }

    nwy_loc_cipgsmloc_open(1, nwy_cipgsmloc_cb);

    opt = nwy_test_cli_input_gets("\r\nPlease open A-GPS(1-open,0-close):");
    value = atoi(opt);

    ret = nwy_loc_agps_open(value);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest open A-GPS success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest open A-GPS error!\r\n");
#endif
}

void nwy_test_cli_gnss_close()
{
    int ret = 0;
    ret = nwy_loc_stop_navigation();

    if (ret == 0)
        nwy_test_cli_echo("\r\nTest loc close success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc close error!\r\n");
}
#endif
#ifndef NWY_OPEN_TEST_BASE_LOC_NS
void nwy_cipgsmloc_cb(nwy_log_cipgsmloc_result_t *text)
{
    nwy_log_cipgsmloc_result_t *param = (nwy_log_cipgsmloc_result_t *)text;

    if (NULL == param)
        return;

    nwy_test_cli_echo("\r\n cipgsmloc info\r\n");
    if (0 == param->result)
    {
        // nwy_test_cli_echo("lat %lf \r\n", param->info.data.lat);
        // nwy_test_cli_echo("lng %lf \r\n", param->info.data.lng);
        // nwy_test_cli_echo("accuracy %lf \r\n", param->info.data.alt);
        loc_info.latitude = param->info.data.lat ;
        loc_info.longitude = param->info.data.lng ;
        snprintf(info_string, sizeof(info_string), "%.6f %.6f", loc_info.latitude, loc_info.longitude);
        nwy_test_cli_echo("\r\nMQTT string sent:%s",info_string);
        nwy_test_cli_echo("lat_my_struct %lf \r\n", loc_info.latitude);
        nwy_test_cli_echo("lng_my_struct %lf \r\n", loc_info.longitude);
    }
    else
    {
        nwy_test_cli_echo(" %s\r\n", param->info.errmsg);
    }

    return;
}
#endif

#ifdef NWY_OPEN_TEST_GNSS_EXTERNAL_CHIP
extern int nwy_test_cli_check_data_connect();
void nwy_test_cli_gnss_open()
{
    int ret = 0;
    ret = nwy_loc_start_navigation();

    if (ret == 1)
        nwy_test_cli_echo("\r\nTest loc open success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc open error!\r\n");
}
void nwy_test_cli_gnss_close()
{
    int ret = 0;
    ret = nwy_loc_stop_navigation();

    if (ret == 1)
        nwy_test_cli_echo("\r\nTest loc close success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc close error!\r\n");
}

void nwy_test_cli_gnss_nmea_data()
{
    int ret = 0;
    char nmea_data[2048] = {0};

    memset(nmea_data,0,2048);
    ret = nwy_loc_get_nmea_data(nmea_data);
    if (ret) {
        nwy_test_cli_echo("\r\n get nmea data success\r\n");
    } else {
        nwy_test_cli_echo("\r\n get nmea data fail\r\n");
    }
    nwy_test_cli_echo("\r\n %s", nmea_data);
}

void nwy_test_cli_gnss_nmea_specific_data()
{
    int ret = 0;
    int nmea_type = 0;
    char *opt = NULL;
    char specific_nmea_data[2048] = {0};
    opt = nwy_test_cli_input_gets("\r\n input gnss nmea type(0-6):");
    nmea_type = atoi(opt);
    nwy_test_cli_echo("\r\n nmea type:%d", nmea_type);
    memset(specific_nmea_data,0,2048);
    ret = nwy_loc_get_specific_data(nmea_type,specific_nmea_data);
    if (ret) {
        nwy_test_cli_echo("\r\n get specific nmea data success");
    } else {
        nwy_test_cli_echo("\r\n get specific nmea data fail");
    }
    nwy_test_cli_echo("\r\n %s", specific_nmea_data);
}
void nwy_test_cli_gnss_set_server()
{
    char *opt = NULL;
    char AGPS_URL[128] = {0};
    char AGPS_USER[128] = {0};
    char AGPS_PASS[128] = {0};
    int port = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease input agps server url:");
    memcpy(AGPS_URL, opt, strlen(opt));

    opt = nwy_test_cli_input_gets("\r\nPlease input agps server port:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nPlease input agps server user:");
    memcpy(AGPS_USER, opt, strlen(opt));
    opt = nwy_test_cli_input_gets("\r\nPlease input agps server pass:");
    memcpy(AGPS_PASS, opt, strlen(opt));

    ret = nwy_loc_set_server(AGPS_URL, port, AGPS_USER, AGPS_PASS);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc set server success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set server error!\r\n");
}

void nwy_test_cli_gnss_open_assisted()
{
    char *opt = NULL;
    int value = 0;
    int ret = 0;

    if (1 == nwy_test_cli_check_data_connect())
    {
        nwy_pdp_set_status(NWY_PDP_CONNECTED);
    }
    else
    {
        nwy_test_cli_echo("\r\n need module dail at+xiic = 1");
        nwy_test_cli_send_virt_at();
    }

    opt = nwy_test_cli_input_gets("\r\nPlease open A-GPS(1-open,0-close):");
    value = atoi(opt);

    nwy_loc_start_navigation();
    ret = nwy_loc_agps_open(value);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest open A-GPS success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest open A-GPS error!\r\n");
}

void nwy_test_cli_gnss_set_position_md()
{
    char *opt = NULL;
    uint8_t mode = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease input loc mode(1-GPS 3-GPS+BD 5-GPS+GLO 7-GPS+BD+GLO):");
    mode = atoi(opt);
    nwy_test_cli_echo("\r\nTest mode = %d!\r\n", mode);

    nwy_loc_start_navigation();

    ret = nwy_loc_set_position_mode(mode);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc set position success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set position error!\r\n");
}

void nwy_test_cli_gnss_set_updata_rate()
{
    char *opt = NULL;
    int frequency = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease input loc frequency(1HZ-1000 2HZ-500):");
    frequency = atoi(opt);

    ret = nwy_loc_nmea_format_mode(2, frequency);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc set frequency success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set frequency error!\r\n");
}

void nwy_test_cli_gnss_set_output_fmt()
{
    char *opt = NULL;
    int opt_format = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease input loc opt_format(128-255):");
    opt_format = atoi(opt);

    ret = nwy_loc_nmea_format_mode(3, opt_format);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc set opt_format success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set opt_format error!\r\n");
}

void nwy_test_cli_gnss_set_startup_fmt()
{
    char *opt = NULL;
    int startup_mode = 0;
    int ret = 0;

    opt = nwy_test_cli_input_gets("\r\nPlease input loc startup_mode(0-HOT 1-WARM 2-COLD 4-FAC:");
    startup_mode = atoi(opt);

    ret = nwy_loc_set_startup_mode(startup_mode);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc set startup_mode success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc set startup_mode error!\r\n");
}

#endif

#ifndef NWY_OPEN_TEST_WIFI_LOC_NS
void nwy_wifigsmloc_cb(nwy_log_cipgsmloc_result_t *text)
{
    nwy_log_cipgsmloc_result_t *param = (nwy_log_cipgsmloc_result_t *)text;
    if (NULL == param)
        return;

    if (0 == param->result)
    {
        // nwy_test_cli_echo("lat %lf \r\n", param->info.data.lat);
        // nwy_test_cli_echo("lng %lf \r\n", param->info.data.lng);
        // nwy_test_cli_echo("accuracy %lf \r\n", param->info.data.alt);
        loc_info.latitude = param->info.data.lat ;
        loc_info.longitude = param->info.data.lng ;
        //snprintf(info_string, sizeof(info_string), "%.6f %.6f", loc_info.latitude, loc_info.longitude);
        //nwy_test_cli_echo("\r\nMQTT string sent:%s", info_string);
        nwy_test_cli_echo("lat_my_struct %f \r\n", loc_info.latitude);
        nwy_test_cli_echo("lng_my_struct %f \r\n", loc_info.longitude);
    }
    else
        nwy_test_cli_echo(" %s\r\n", param->info.errmsg);

    return;
}
#endif

void nwy_test_cli_wifi_open_base()
{
#ifdef NWY_OPEN_TEST_WIFI_LOC_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *opt = NULL;
    int value = 0;
    int ret = 0;
    if (1 == nwy_test_cli_check_data_connect())
    {
        nwy_pdp_set_status(NWY_PDP_CONNECTED);
    }
    else
    {
        nwy_test_cli_echo("\r\n need module dail at+xiic = 1");
        nwy_test_cli_send_virt_at();
    }
    opt = nwy_test_cli_input_gets("\r\nPlease open wifi lbs(1-open,0-close):");
    value = atoi(opt);
    ret = nwy_loc_wifigsmloc_open(value, nwy_wifigsmloc_cb);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest open wifi loc success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest open wifi loc error!\r\n");
#endif
}

void nwy_test_cli_gnss_open_base()
{
#ifdef NWY_OPEN_TEST_BASE_LOC_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *opt = NULL;
    int value = 0;
    int ret = 0;
    // opt = nwy_test_cli_input_gets("\r\nPlease open lbs(1-open,0-close):");
    // value = atoi(opt);
    value = 1;
    if (1 == nwy_test_cli_check_data_connect())
    {
        nwy_pdp_set_status(NWY_PDP_CONNECTED);
        nwy_test_cli_echo("\r\nNWY_PDP_CONNECTED\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\n need module dail at+xiic = 1");
        nwy_test_cli_send_virt_at();
        nwy_test_cli_echo("\r\nNWY_PDP_DIS_CONNECTED\r\n");
    }
    ret = nwy_loc_cipgsmloc_open(value, nwy_cipgsmloc_cb);
    if (ret > 0)
        nwy_test_cli_echo("\r\nTest loc open lbs success!\r\n");
    else
        nwy_test_cli_echo("\r\nTest loc open lbs error!\r\n");
#endif
}
/*----------------------------------------*/
