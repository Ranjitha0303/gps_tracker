#ifndef __NWY_LBS_EXT_H__
#define __NWY_LBS_EXT_H__

#include "nwy_wifi.h"
#include "nwy_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum nwy_pdp_status_type
{
  NWY_PDP_DISCONNECTED,
  NWY_PDP_CONNECTED,
  NWY_PDP_CONNECTTING,
  NWY_PDP_DISCONNECTTING,
}nwy_pdp_status_type;

typedef struct
{
    double lat;
    double lng;
    double alt;
}nwy_cipgsmloc_info_t;

typedef struct{
    char result;    //0 == nwy_cipgsmloc_info_t 1 == errmsg
    union
    {
        nwy_cipgsmloc_info_t data;
        char errmsg[48];
    }info;
}nwy_log_cipgsmloc_result_t;
typedef void (*nwy_loc_cipgsmloc_callback)(nwy_log_cipgsmloc_result_t *text);

typedef struct
{
  unsigned int          mcc;
  unsigned int          mnc;
  unsigned int          lac;
  unsigned int          cell_id;
  char      imei_str[20];
  int       rssi_csq ;

} nwy_lbs_info_t;

int nwy_loc_get_lbs_info(nwy_lbs_info_t* databuf);

bool nwy_loc_cipgsmloc_open(bool value, nwy_loc_cipgsmloc_callback cb);
bool nwy_loc_wifigsmloc_open(bool value, nwy_loc_cipgsmloc_callback cb);

#ifdef __cplusplus
   }
#endif

#endif
