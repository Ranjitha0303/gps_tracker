/*Begin: Add by huangweigang for OPEN_SDK_API network in 2019.11.13*/
#ifndef _NWY_NETWORK_H_
#define _NWY_NETWORK_H_
//#include "nwy_sim_cache.h"//delete by huangsangen for no this file.
#ifdef __cplusplus
extern "C" {
#endif
#ifndef bool
#define bool char
#endif

typedef uint16 nwy_nw_mode_type_t;

//return value:
#ifndef NWY_SUCCESS
#define NWY_SUCCESS 0
#endif
#ifndef NWY_ERROR
#define NWY_ERROR  -1
#endif

#define FREE_BAND_LOCK 0

#define SIM_FILE_STRUCTURE_BINARY 0
#define SIM_FILE_STRUCTURE_LINEAR 1
#define SIM_FILE_STRUCTURE_CYCLIC 3
#define SIM_FILE_TYPE_EF 4
#define USIM_FILE_STRUCTURE_BINARY 1
#define USIM_FILE_STRUCTURE_LINEAR 2
#define USIM_FILE_STRUCTURE_CYCLIC 6
#define CRSM_READ_BINARY 176
#define CRSM_READ_RECORD 178
#define CRSM_GET_RESPONSE 192
#define CRSM_UPDATE_BINARY 214
#define CRSM_UPDATE_RECORD 220
#define CRSM_STATUS 242

#define DRV_USB_NWY_ECM 1
#define DRV_USB_NWY_RNDIS 0

/*Begin: Add by huangsangen for the para in nwy_sim_cache.h */
#define NWY_SIM_CACHE_TIMER_DESC_MAX 128
#define NWY_SIM_CACHE_TIMER_MAX 8
#define NWY_NW_SCAN_LIST_MAX    10
#define NWY_NW_MNC_MCC_LEN      3
#define NWY_NW_EONS_MAX_LEN     128
#define NWY_NW_FPLMN_LIST_MAX   64
#define NWY_NW_SPN_LEN_MAX      17

#define NWY_CRAD_READ_BINARY 176

typedef struct sys_plmn_id_s
{
  uint8                                identity[3];
    /**< Public Land Mobile Network ID. */
} sys_plmn_id_s_type;

typedef struct
{
    int num;
    sys_plmn_id_s_type fplmn[NWY_NW_FPLMN_LIST_MAX];
} nwy_sim_fplmn_list_t;

typedef struct
{
    uint32 iCommand;
    uint32 iP1;
    uint32 iP2;
    uint32 iP3;
    uint16 nFileIDs;
} sim_cache_async_ctx_t;

typedef enum
{
    NWY_SIM_CACHE_MIN = -1,
    NWY_FPLMN_READ,
    NWY_FPLMN_WRITE,
    NWY_SIM_CACHE_MAX,
}nwy_sim_cache_option_e;

struct nwySimCacheCmd_t;
typedef void (*nwySimCacheAsyncDestroy_t)(struct nwySimCacheCmd_t *cmd);
typedef struct nwySimCacheCmd_t{
    uint32  option; // read/write card file option;read FPLMN.eg.
    uint32  data_len;
    void   *data;
    void    *async_ctx;
    nwySimCacheAsyncDestroy_t async_ctx_destroy;
}nwy_sim_cache_cmd_s;
/*End: Add by huangsangen for the para in nwy_sim_cache.h */

extern void           nwy_sleep(uint32 ms);
extern int            nwy_usbmode(void);
extern uint8_t        nwy_get_sim(void);
extern osiThread_t *  netGetTaskID(void);
extern int            nwy_usbmode_set(int mode);
extern void           nwy_hal_removeEpsApnSuffix(char *apn);
extern uint8_t        AT_GprsGetActivePdpCount(uint8_t nSim);
extern int            del_same(uint32_t *dest, uint32_t data[], int n);
extern uint8_t        Mapping_Creg_From_PsType(uint8_t pstype);
extern int            nwy_get_operator_apn(char* apn,int len,uint8_t* PdpType);
extern void           nwy_fclose(signed int fileHandle);
extern int            nwy_fopen(char* lpFileName, signed int flag);
extern int            nwy_fwrite(signed int fileHandle, uint8_t *writeBuffer, uint32_t numberOfBytesToWrite);


#define NWY_NW_MODE_NONE        0x00
#define NWY_NW_MODE_GSM         0x02
#define NWY_NW_MODE_LTE         0x04
/*Begin:Add by hujun for get neighborLocator info in 2020.11.21*/
#define NWY_NEIGHBOR_LOCATOR_COUNT_MAX  7
/*End:Add by hujun for get neighborLocator info in 2020.11.21*/


/*Begin:Add by zhanghao for get and set CGDFTPDN info in 2020.11.19*/
#define NWY_NW_APN_MAX_LEN      99
#define NWY_NW_PWD_MAX_LEN      64
#define NWY_NW_USER_MAX_LEN     64
/*end:Add by zhanghao for get and set CGDFTPDN info in 2020.11.19*/

typedef enum
{
    NWY_NW_RADIO_TECH_NONE = -1,
    NWY_NW_RADIO_TECH_GSM = 0,
    NWY_NW_RADIO_TECH_LTE = 7,
}nwy_nw_radio_type_t;

typedef enum
{
    NWY_NW_SERVICE_NONE = 0,
    NWY_NW_SERVICE_LIMITED = 1,
    NWY_NW_SERVICE_FULL = 2,
}nwy_nw_service_type_t;

typedef enum
{
    NWY_NW_ROAM_STATE_OFF = 0,
    NWY_NW_ROAM_STATE_ON = 1,
}nwy_nw_roam_state_t;

typedef struct
{
    nwy_nw_radio_type_t radio_tech;
    nwy_nw_roam_state_t roam_state;
    nwy_nw_service_type_t regs_state;
}nwy_nw_common_regs_type_t;

typedef struct
{
    uint16 data_regs_valid;
    nwy_nw_common_regs_type_t data_regs;
    uint16 voice_regs_valid;
    nwy_nw_common_regs_type_t voice_regs;
}nwy_nw_regs_info_type_t;

typedef enum
{
    NWY_NW_REGS_NONE = -1,
    NWY_NW_REGS_DATA_IND = 0,
    NWY_NW_REGS_VOICE_IND = 1,
}nwy_nw_regs_ind_type_t;


typedef struct
{
    char long_eons[NWY_NW_EONS_MAX_LEN + 1];
    char short_eons[NWY_NW_EONS_MAX_LEN + 1];
    char mcc[NWY_NW_MNC_MCC_LEN + 1];
    char mnc[NWY_NW_MNC_MCC_LEN + 1];
    char spn[NWY_NW_SPN_LEN_MAX + 1];
}nwy_nw_operator_name_t;

typedef enum
{
    NWY_NW_NET_STAT_UNKNOW = 0,
    NWY_NW_NET_STAT_AVAILABLE,
    NWY_NW_NET_STAT_CURRENT,
    NWY_NW_NEY_STAT_FORBIDDEN,
}nwy_nw_net_status_t;

typedef enum
{
    NWY_NW_NET_RAT_UNKNOW,
    NWY_NW_NET_RAT_GSM,
    NWY_NE_NET_RAT_LTE,
}nwy_nw_net_rat_t;

typedef struct
{
    nwy_nw_net_status_t net_status;
    nwy_nw_net_rat_t net_rat;
    nwy_nw_operator_name_t net_name;
}nwy_nw_net_scan_item_t;

typedef struct
{
    int result;
    int num;
    nwy_nw_net_scan_item_t net_list[NWY_NW_SCAN_LIST_MAX];
}nwy_nw_net_scan_list_t;

typedef struct
{
    char try_auto_at_fail;
}netSelAsyncCtx_t;

typedef struct
{
    char mcc[NWY_NW_MNC_MCC_LEN + 1];
    char mnc[NWY_NW_MNC_MCC_LEN + 1];
    nwy_nw_net_rat_t net_rat;
}nwy_nw_net_select_param_t;

typedef struct
{
    char mcc[NWY_NW_MNC_MCC_LEN + 1];
    char mnc[NWY_NW_MNC_MCC_LEN + 1];
}nwy_nw_plmn_list_t;

typedef struct
{
    int num;
    nwy_nw_plmn_list_t fplmn[NWY_NW_FPLMN_LIST_MAX];
}nwy_nw_fplmn_list_t;
/* added by wangchen for write FPLMN 20201105 begin */
typedef struct
{
    int num;
    sys_plmn_id_s_type fplmn[NWY_NW_FPLMN_LIST_MAX];
}nwy_sim_write_fplmn_list_t;
/* added by wangchen for write FPLMN 20201105 end */

/*Begin:Add by huangweigang for get NET info in 2020.11.14*/
typedef struct 
{
    uint8_t is_over;
    uint8_t curr_rat;
    uint8_t ulBler;
    uint8_t dlBler;
    int16_t rsrp;    //LTE
    int16_t rsrq;    //LTE
    int16_t SINR;    //LTE
    uint32_t cellId;   ///< default value 0xff
    uint16_t pcid;   //LTE
    int16_t rxPower;   //Dl receiving power; Real value(Unit:dbm)=rxPower divide 16.
    int16_t txPower;   //UE transmits power, -46~23 dbm
}nwy_serving_cell_info;
/*End:Add bu huangweigang for get NET info in 2020.11.14*/

typedef struct
{
    int mcc;
    int mnc;
    uint8_t imsi[16];    ///< imsi[0]:length of IMSI
    uint8_t roamingFlag; ///< 1:roaming,0:non roaming
    uint16_t bandInfo;   ///< band1-band256
    uint32_t dlEarfcn;   ///< 0-262143, invalid value 0xffffffff
    uint16_t pcid;
    int16_t rsrp;
    int16_t rsrq;
    int16_t Srxlev;
    uint16_t bandwidth; ///< default value 0xffff
    uint16_t SINR;
    uint8_t ulBler;
    uint8_t dlBler;
    uint32_t cellId;   ///< default value 0xff
    uint16_t tac;      //serving tac(from SIB1)
    uint16_t netMode;  //0--tdd  1--fdd
    int16_t rxPower;   //Dl receiving power; Real value(Unit:dbm)=rxPower divide 16.
    int16_t txPower;   //UE transmits power, -46~23 dbm
} nwy_lte_scell_t;

typedef struct 
{
    int mcc;
    int mnc;
    uint32_t Arfcn; //Absolute Radio Frequency Channel Number ,[0..1023].
    uint32_t Lac; //Location Area Identification.
    int32_t Cellid;
    uint8_t Bsic;      //Synchronisation code,[0..63].
    uint8_t NetMode;   //Network Mode of Operation (GPRS only),[0..2].
    uint8_t AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].
    uint8_t MaxTxPWR;  //Max allowed transmit power, [0..31].
    int8_t C1Value;    //Cell quality ,[-33..110].
    uint8_t BER;       //Bit Error Rate [0..255],255 = non significant.
    uint8_t RxQualFull;   //receive quality full  [0~7]
    uint8_t RxQualSub;    //receive quality sub   [0~7]
    int16_t C2;           //Value of the cell reselection criterium   [-96~173]
    uint8_t CurrBand;     //current band used,possible value see below
    uint8_t RxLevAccMin; //receive level access minimum  [0~63]
    uint8_t RxDbm;       //indicates current rx power
    uint8_t TxDbm;       //indicates current tx power
    uint8_t Maio;
    uint8_t Hsn;        /**< Hopping sequence number. Value 0 means cyclic hopping is done*/
    uint8_t RxLevSub;
    uint8_t RxLevFull;
    uint8_t RxLev;
    int8_t rssi;
} nwy_gsm_scell_t;

/*Begin: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */
typedef struct
{
    int mcc;
    int mnc;
    uint32_t Lac;
    uint32_t Arfcn;
    //uint8_t imsi[16];    ///< imsi[0]:length of IMSI
    //uint8_t roamingFlag; ///< 1:roaming,0:non roaming
    uint16_t bandInfo;   ///< band1-band256
    uint16_t URAID;/**< URA identity */
    uint8_t RAC;/**< Routing area code */
    uint32_t ci;
    uint16_t PSC;/**< Primary scrambling code for FDD or Cell parameter id for TDD */
    int16_t rscp;
    int16_t rssi;
    int16_t srxlev;
    int16_t squal;
    uint16_t phych;// 0 is DPCH, 1 is FDPCH. 0xFF is invalid
    uint16_t sf;
    uint8_t slot;
    int16_t ECio;//ecio = rscp-rssi;
    int16_t rxPower;   //Dl receiving power; Real value(Unit:dbm)=rxPower divide 16.
    int16_t txPower;   //UE transmits power, -46~23 dbm
} nwy_umts_scell_t;
/*End: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */


typedef struct 
{
    nwy_lte_scell_t lte_scell_info;
    nwy_gsm_scell_t gsm_scell_info;
/*Begin: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */
    nwy_umts_scell_t umts_scell_info;
/*Begin: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */
}nwy_serving_cell_info_v2;


typedef struct
{
    int mcc;
    int mnc;
    uint32_t frequency;
    uint16_t pcid; ///< if not decode BSIC,apply 0xffff
    uint16_t bandWidth;
    int16_t rsrp;
    int16_t rsrq;
    int16_t Srxlev;
    uint32_t cellId;              //default value 0xff
    uint16_t tac;                 //serving tac(from SIB1)
} nwy_lte_ncell_t;

typedef struct 
{
    int mcc;
    int mnc;
    uint32_t Arfcn;    //Absolute Radio Frequency Channel Number ,[0..1023].
    uint32_t Cellid; //Cell Identity.
    uint8_t AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].==RSSI
    uint8_t Bsic;      //Synchronisation code,[0..63].
    uint8_t MaxTxPWR;  //Max allowed transmit power, [0..31].
    uint8_t RxLevAM;   //RxLev Access Min in RxLev [0..63].
    int8_t C1Value;    //Cell quality ,[-33..110].see 05.08
    uint32_t lac;    //Location Area Identification.
    int16_t C2;        //Value of the cell reselection criterium   [-96~173]
    int16_t C31;
    int16_t C32;
    int rssi;
} nwy_gsm_ncell_t;

/*Begin: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */
typedef struct
{
    int mcc;
    int mnc;
    uint32_t Lac;
    uint32_t Arfcn;
    uint32_t ci;
    uint16_t PSC;
    int16_t rscp;
    int16_t srxlev;
    int16_t ECno;
    int16_t thresh_xhigh;
    int16_t thresh_xlow;
} nwy_umts_ncell_t;
/*End: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */

typedef struct
{
    int ncell_num;
    nwy_lte_ncell_t lte_ncell_info[NWY_NEIGHBOR_LOCATOR_COUNT_MAX - 1];
    nwy_gsm_ncell_t gsm_ncell_info[NWY_NEIGHBOR_LOCATOR_COUNT_MAX - 1];
} nwy_neighbour_cell_info_v2;

/*Begin:Add by hujun for get neighborLocator info in 2020.11.21*/
typedef struct nwy_locator_info {
    uint32_t cellid;
    int mcc;
    int mnc;
    uint32_t lac;
    int rssi;
} nwy_locator_info_t;

/*Begin: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */
typedef struct
{
    int ncell_num;
    nwy_lte_ncell_t lteNcell[NWY_NEIGHBOR_LOCATOR_COUNT_MAX - 1];
}nwy_lte_ncell_ind;
typedef struct
{
    int ncell_num;
    nwy_umts_ncell_t umtsNcell[NWY_NEIGHBOR_LOCATOR_COUNT_MAX - 1];
}nwy_umts_ncell_ind;
typedef struct
{
    int ncell_num;
    nwy_gsm_ncell_t gsmsNcell[NWY_NEIGHBOR_LOCATOR_COUNT_MAX - 1];
}nwy_gsm_ncell_ind;
typedef struct
{
    nwy_umts_ncell_ind interRATUtra;
    nwy_gsm_ncell_ind interRATGsm;
}nwy_lteInterRat_ind;
typedef struct
{
    nwy_lte_ncell_ind interRatLte;
    nwy_gsm_ncell_ind interRatGsm;
}nwy_UmtsInterRat_ind;

typedef struct
{
     nwy_lte_ncell_ind interRatLTE;
      nwy_umts_ncell_ind interRatUTRA;
}nwy_GsmInterRat_ind;


typedef struct
{
    nwy_lte_ncell_ind lteIntraNcell;
    nwy_lte_ncell_ind lteInterNcell;
    nwy_lteInterRat_ind lteInterRatinfo;
} nwy_lte_ncell_info;

typedef struct
{
    nwy_umts_ncell_ind umtsIntraNcell;
    nwy_umts_ncell_ind umtsInterNcell;
    nwy_UmtsInterRat_ind umtsInterRatinfo;
} nwy_umts_ncell_info;

typedef struct
{
    nwy_gsm_ncell_ind gsm_ncell_info;
    nwy_GsmInterRat_ind gsmInterRatinfo;
} nwy_gsm_ncell_info;
typedef struct
{
    
    nwy_lte_ncell_info lte_ncell;
    nwy_umts_ncell_info umts_ncell;
    nwy_gsm_ncell_info gsm_ncell;
} nwy_neighbour_cell_info;
/*Begin: Add by huangsangen in 2022.6.20 for match the modify of test_cli in 1803 */

typedef struct nwy_scanned_locator_info {
    int num;
    nwy_locator_info_t locator_info[NWY_NEIGHBOR_LOCATOR_COUNT_MAX];
    /*Begin: Add by huangweigang for more info in 2021.7.30*/
    uint8_t curr_rat; //2:gsm 4:lte
    nwy_serving_cell_info_v2 scell_info;
    nwy_neighbour_cell_info ncell;//modify for match the modify of test_cli in 1803
    /*Begin: Add by huangweigang for more info in 2021.7.30*/
} nwy_scanned_locator_info_t;

typedef void (*nwy_locator_info_destroy)(void *cmd);
typedef void (*nwy_locator_report_cb)(void *infor, int num);

typedef struct
{
    uint8_t currReqType;
    uint8_t currNetMode;
    nwy_locator_report_cb report_cb;
} nwy_bcchContext_t;

typedef struct nwy_locator_info_cmd{
    void    *async_ctx;
    nwy_locator_info_destroy async_ctx_destroy;
}nwy_locator_info_cmd_t;
/*End:Add by hujun for get neighborLocator info in 2020.11.21*/


/*Begin:Add by zhanghao for get and set CGDFTPDN info in 2020.11.19*/
typedef struct
{
    uint8_t pdpType;
    uint8_t authProt;
    char apn[NWY_NW_APN_MAX_LEN + 1];
    char userName[NWY_NW_PWD_MAX_LEN + 1];
    char password[NWY_NW_USER_MAX_LEN + 1];
}nwy_nw_cfgdftpdn_t;
/*end:Add by zhanghao for get and set CGDFTPDN info in 2020.11.19*/

typedef void (*nwy_nw_cb_func)
(
    nwy_nw_regs_ind_type_t ind_type,
    void *ind_struct
);

typedef void (*nwy_nw_net_scan_cb_func)
(
    nwy_nw_net_scan_list_t *net_list
);

typedef enum
{
  OPEN_NET_AUTO = 0,
  OPEN_NET_GSM = 2,//8850 not support
  OPEN_NET_WCDMA = 3,//8850 not support
  OPEN_NET_LTE = 4
}nwy_open_net_e;

typedef struct 
{
  nwy_open_net_e net;
  int band;
  uint32_t freq;
} nwy_open_freq_t;

//basic info of network
int nwy_nw_get_signal_rssi(uint8_t *rssi);
int nwy_nw_get_signal_csq(uint8_t *csq_val);
int nwy_nw_get_operator_name(nwy_nw_operator_name_t *opt_name);
int nwy_nw_get_register_info(nwy_nw_regs_info_type_t *p_regs_info);
int nwy_nw_get_network_mode(nwy_nw_mode_type_t *p_mode);
int nwy_nw_manual_network_select(nwy_nw_net_select_param_t *net_select);
int nwy_nw_manual_network_scan(nwy_nw_net_scan_cb_func scan_cb);
void net_scan_cb(nwy_nw_net_scan_list_t *net_list);
int nwy_nw_unregister_callback_func();
int nwy_nw_register_callback_func(nwy_nw_cb_func nwy_cb);

//state of net domain
int nwy_nw_set_radio_st(int fun);
int nwy_nw_get_radio_st(int *status);
int nwy_nw_get_ps_st(int *ps_status);

//msg of serving/neignborhood cell
int nwy_nw_util_get_netmsg(nwy_serving_cell_info *pNetmsg);
int nwy_nw_get_netmsg(nwy_serving_cell_info *pNetmsg);
int nwy_nw_get_neighborLocatorInfo(nwy_locator_report_cb report_cb);
int nwy_nw_get_lacid(char lac[], char cid[]);


//control of net_select
int nwy_nw_band_lock(uint32_t act, const char *set_band);//Please set act = 4.
int nwy_nw_freq_lock(nwy_open_freq_t *freq_info, int n);
int nwy_nw_write_forbidden_plmn(nwy_sim_write_fplmn_list_t write_fplmn);
int nwy_nw_get_forbidden_plmn(nwy_nw_fplmn_list_t *fplmn_list);

//apn、netshare
char* nwy_nw_get_default_pdn_apn();
int nwy_nw_get_cfgdftpdn_info(nwy_nw_cfgdftpdn_t* cfgdftpdn_info);
int nwy_nw_set_cfgdftpdn_info(nwy_nw_cfgdftpdn_t cfgdftpdn_info);
int nwy_nw_set_netsharemode(int share_mode);
int nwy_nw_get_netsharemode();
int nwy_nw_set_netshareact(int ucid,int action,int auto_connect,char* apn_p,char* user_name_p ,char* passwd_p,int auth_type,int ip_type);
int nwy_nw_get_netshareact(int* action_p,int* auto_connect_p,char* apn_p,char* user_name_p,char* passwd_p,int* ip_type_p,int* share_mode_p);

#ifdef __cplusplus
   }
#endif

#endif



