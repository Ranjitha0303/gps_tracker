#ifndef __NWY_VOICE_H__
#define __NWY_VOICE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t ;
#define NWY_MAX_PHONE_NUMBER  82  // Maximum length for a phone number or SIP URI (81 + NULL).

typedef enum
{
    NWY_RES_OK                      = 0,
    NWY_VOICE_E_UNKNOWN             = -1,
    NWY_VOICE_E_INVALID_PARA        = -2,
}nwy_voice_error_t;

typedef enum
{
    NWY_VOICE_CALL_STATE_INCOMING =0x0000,
    NWY_VOICE_CALL_STATE_DIALING  =0x0001,
    NWY_VOICE_CALL_STATE_ALERTNG  =0x0002,
    NWY_VOICE_CALL_STATE_ACTIVE  =0x0003,
    NWY_VOICE_CALL_STATE_HOLDING  =0x0004,
    NWY_VOICE_CALL_STATE_END =0x0005,
    NWY_VOICE_CALL_STATE_WAITING  =0x0006,
} nwy_vc_call_state_t;

typedef enum {
    NWY_VOICE_CALL_MOBILE_ORIGINATED_V01 = 1, // Mobile-originated.
    NWY_VOICE_CALL_MOBILE_TERMINATED_V01 = 2, // Mobile-terminated.
}nwy_voice_call_direction_type_t;

typedef enum {
  NWY_CALL_MODE_NO_SRV = 0x00, /**<  No service \n  */
  NWY_CALL_MODE_CDMA = 0x01, /**<  CDMA \n  */
  NWY_CALL_MODE_GSM = 0x02, /**<  GSM \n  */
  NWY_CALL_MODE_UMTS = 0x03, /**<  UMTS \n  */
  NWY_CALL_MODE_LTE = 0x04, /**<  LTE \n  */
  NWY_CALL_MODE_TDS = 0x05, /**<  TD-SCDMA \n  */
  NWY_CALL_MODE_UNKNOWN = 0x06, /**<  Unknown \n  */
  NWY_CALL_MODE_WLAN = 0x07, /**<  WLAN  */
} nwy_call_mode_e;

typedef enum {
  NWY_CALL_TYPE_VOICE = 0x00, /**<  Voice \n  */
  NWY_CALL_TYPE_VOICE_FORCED = 0x01, /**<  Avoid modem call classification \n  */
  NWY_CALL_TYPE_VOICE_IP = 0x02, /**<  Voice over IP  \n  */
  NWY_CALL_TYPE_VT = 0x03, /**<  Videotelephony call over IP \n  */
  NWY_CALL_TYPE_VIDEOSHARE = 0x04, /**<  Videoshare \n  */
  NWY_CALL_TYPE_TEST = 0x05, /**<  Test call type \n  */
  NWY_CALL_TYPE_OTAPA = 0x06, /**<  OTAPA \n  */
  NWY_CALL_TYPE_STD_OTASP = 0x07, /**<  Standard OTASP \n  */
  NWY_CALL_TYPE_NON_STD_OTASP = 0x08, /**<  Nonstandard OTASP \n  */
  NWY_CALL_TYPE_EMERGENCY = 0x09, /**<  Emergency \n  */
  NWY_CALL_TYPE_SUPS = 0x0A, /**<  Supplementary service \n  */
  NWY_CALL_TYPE_EMERGENCY_IP = 0x0B, /**<  Emergency VoIP \n  */
  NWY_CALL_TYPE_ECALL = 0x0C, /**<  eCall \n  */
  NWY_CALL_TYPE_EMERGENCY_VT = 0x0D, /**<  Emergency videotelephony call over IP  */
} nwy_call_type_e;

typedef struct {
    uint32_t call_id;                            // Call ID associated with this call.
    nwy_vc_call_state_t state;         // Current call state (mcm_voice_call_state).
    char number[NWY_MAX_PHONE_NUMBER + 1];     // Phone number.
    nwy_voice_call_direction_type_t direction; // Voice call direction.
    nwy_call_mode_e call_mode;
    nwy_call_type_e call_type;
}nwy_voice_call_record_type_t;

typedef void (*nwy_voice_statehandlerfunc)
(
    int                     call_id,
    char*                   phone_num,
    nwy_vc_call_state_t     state,
    void                    *contextPtr
);

typedef struct nwy_voice_handle {
    nwy_voice_statehandlerfunc func;
}nwy_voice_handle_t;

int nwy_voice_call_start(uint8_t sim_id,char *phone_num);
int nwy_voice_call_end(uint8_t sim_id);
int nwy_voice_call_autoanswver();
int nwy_voice_setvolte(uint8_t sim_id, uint8_t setvolte);
int nwy_voice_add_statehandler(nwy_voice_statehandlerfunc state_handle);

#ifdef __cplusplus
   }
#endif

#endif
