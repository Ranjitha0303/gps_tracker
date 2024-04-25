#ifndef __NWY_LOC_EXTERNAL_H__
#define __NWY_LOC_EXTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    NWY_LOC_AUX_GNSS_GPS = 1,
    NWY_LOC_AUX_GNSS_BD = 2,
    NWY_LOC_AUX_GNSS_GPS_BD = 3,
    NWY_LOC_AUX_GNSS_GLONASS = 4,
    NWY_LOC_AUX_GNSS_GPS_GLONASS = 5,
    NWY_LOC_AUX_GNSS_BD_GLONASS = 6,
    NWY_LOC_AUX_GNSS_ALL = 7,

    NWY_LOC_AUX_GNSS_NONE = 100,
} nwy_loc_position_mode_t;

typedef enum
{
    NWY_LOC_HOT_START = 0,
    NWY_LOC_WORM_START = 1,
    NWY_LOC_COLD_START = 2,
    NWY_LOC_NONE_START,
} nwy_loc_startup_mode;

typedef enum
{
    GNSS_OUTPUT_GGA  = 0,
    GNSS_OUTPUT_GSA  = 1,
    GNSS_OUTPUT_GSV  = 2,
    GNSS_OUTPUT_RMC  = 3,
    GNSS_OUTPUT_VTG  = 4,
    GNSS_OUTPUT_GLL  = 5,
    GNSS_OUTPUT_ALL  = 6,
}nwy_loc_output_type;

int nwy_loc_start_navigation();
int nwy_loc_stop_navigation();
int nwy_loc_get_nmea_data(char*data);
int nwy_loc_set_position_mode(nwy_loc_position_mode_t pos_mode);
int nwy_loc_set_startup_mode(nwy_loc_startup_mode startup);
int nwy_loc_nmea_format_mode(uint16 cmd,int16 sel_parameter);
int nwy_loc_get_specific_data(nwy_loc_output_type type,char *nmea_data);
int nwy_loc_set_server(char *str_url,int port,char *user,char *password);
bool nwy_loc_agps_open(bool value);
void nwy_loc_close_uart_nmea_data();

#ifdef __cplusplus
   }
#endif

#endif
