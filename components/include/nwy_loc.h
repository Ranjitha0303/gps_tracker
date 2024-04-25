#ifndef __NWY_LOC_H__
#define __NWY_LOC_H__

#include "nwy_wifi.h"
#include "pam_config.h"
#include "gnss_config.h"
#include "nwy_config.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_SUPPORT_GNSS
typedef struct
{
    uint8_t gpsWorkMode;
    uint8_t start_mode;
    uint8_t satellite_type;
    uint8_t nsapi;
    uint8_t wifiscan_switch;
    uint16_t gnss_sleep;
    uint16_t lte_sleep;
    uint8_t gnss_wifi_flag;
    uint16_t output_format;
    uint16_t r_mode;
    uint32_t time_interval;
} nwyLocSetting_t;

typedef struct
{
    uint8_t nSatelliteType;
    uint8_t nStartMode;
} NWY_GNSS_START_INFO;

typedef struct
{
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
} NwyGnssTimeStamp;

/********************Defined  Tick Time ***************************************/
typedef struct _NWY_GNSS_SCI_TICK_TIME
{
    uint32_t milliseconds;
    uint32_t second;
} NWY_GNSS_SCI_TICK_TIME_T;

typedef struct
{
    float longitude;        
    float latitude;         
    float altitude;         
    NwyGnssTimeStamp utc_time; 
    uint32_t ttff_time;     
    uint8_t satellite_num;  
    float speed;            
    float course;           
    uint16_t clockdrift;
    NWY_GNSS_SCI_TICK_TIME_T m_nSystemTimeStamp; // RTC or any other system time
    uint32_t m_nUTCTime;                     // second
    uint8_t m_nUncertaintySemiMajor;
    uint8_t m_nUncertaintySemiMinor;
    float m_nBearing;            
    uint16_t m_nDirection;       // UP=0, Down=1
    float m_nHorizontalVelocity; // m/s
    float m_nVerticalVelocity;   // m/s
    uint32_t fix_flag;
    float m_nHorizontalAccuracy;
    float m_nPDOP; 
    float m_nHDOP; 
    float m_nVDOP; 
} NWY_GNSS_INFO_DATA_T;

typedef struct
{
    uint16_t m_nSatelliteIdentifier; 
    uint16_t m_cn0;                  
    uint16_t m_nElevation;           
    uint16_t m_nAzimuth;             
    uint8_t m_IsUsed;
} NWY_GNSS_SATELLITE_INFO_T;

typedef struct
{
    uint8_t satellite_num;
    NWY_GNSS_SATELLITE_INFO_T satellite_info[32];
} NWY_GNSS_SATELLITE_INFO_LIST_T;

typedef struct
{
    NWY_GNSS_INFO_DATA_T gnss_data_info;
    NWY_GNSS_SATELLITE_INFO_LIST_T satellite_list_info;
} NWY_GNSS_READ_INFO_CNF_T;

typedef struct
{
    uint8_t mode;
    unsigned errcode;
    NWY_GNSS_READ_INFO_CNF_T *locGnsInfo;
} NWY_GNSS_INFO_T;


typedef struct
{
    uint16_t length;
    uint16_t type;  // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8_t nmea_data[3072]; 
} NWY_GNSS_OUTPUT_INFO_IND_T;
/********************************************************************************/

#if 0
typedef struct
{
    uint8_t startMode;
    uint8_t useAgnss;
} NWY_GNSS_STARTMODE_AGNSS_T;

typedef struct
{
    uint8_t nHslRetry;
    uint8_t nSim;
    uint8_t pSatelliteType;
    uint16_t nOutputFormat;
    uint32_t nTimeInterval;
    unsigned errcode;
    NWY_GNSS_STARTMODE_AGNSS_T gnssStartInfo;
    NWY_GNSS_READ_INFO_CNF_T *locGnsInfo;
} nwyGnssStartCtx_t;
#endif

typedef enum
{
    NWY_GNSS_WIFISCAN_NONE,
    NWY_GNSS_HANDLE,
    NWY_WIFISCAN_HANDLE,
} NWY_GNSS_WIFISCAN_FLAG;

typedef void (*nwy_loc_ind_event_func)(NWY_GNSS_OUTPUT_INFO_IND_T *ind_msg);

int nwy_loc_delete_aiding_data(void);
int nwy_loc_output_format(uint16_t nOutputFormat, uint32_t nTimeInterval);
int nwy_loc_set_position_mode(uint8_t nSatType);

int nwy_loc_start_navigation(uint8_t nStartMode);
int nwy_loc_nmea_data_ind(nwy_loc_ind_event_func ind_handler);
int nwy_loc_nmea_Info_parse(NWY_GNSS_INFO_T *locGnsInfo);

int nwy_loc_stop_navigation(void);
int nwy_loc_resume(uint8_t enable);
int nwy_loc_handle_priority(uint8_t priority);

#endif

#ifdef __cplusplus
   }
#endif

#endif
