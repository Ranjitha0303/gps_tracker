#ifndef _NWY_OCPP_FWK_H_
#define _NWY_OCPP_FWK_H_

#include <stdio.h>
#include <string.h>
#include "nwy_osi_api.h"
#include "nwy_usb_serial.h"
#include "osi_log.h"
#include "nwy_rws_proc.h"
#include "nwy_data.h"
#include <stdlib.h>
typedef char               ws_s8;
typedef char               ws_char;
typedef short              ws_s16;
typedef unsigned char      ws_u8;
typedef unsigned short     ws_u16;
typedef unsigned int       ws_u32;
typedef int                ws_s32;
typedef signed long long   ws_s64;
typedef unsigned long long ws_u64;
typedef void*              ws_ptr;
typedef void*              ws_task;

#ifndef __FILE__
#define __FILE__    ""
#endif
#ifndef __LINE__
#define __LINE__    ""
#endif

typedef enum NWY_OCPP_MSG
{
    NWY_OCPP_FWK_SEND_EVENT= 0x13,
    NWY_OCPP_FWK_STEUP_EVENT= 0x14,
    NWY_OCPP_FWK_CLOSE_EVENT= 0x15,

}NWY_OCPP_EVENT_MSG;

typedef enum NWY_OCPP_DEMO_MSG
{
    NWY_OCPP_DEMO_INIT= 0x10,
    NWY_OCPP_DEMO_INIT_ERROR= 0x11,
    NWY_OCPP_DEMO_REQUEST_EVENT= 0x12,
    NWY_OCPP_DEMO_CLOSE_OK= 0x13,
    NWY_OCPP_DEMO_CLOSE_ERROR= 0x14,
    NWY_OCPP_DEMO_REQUEST_OK= 0x15,
    NWY_OCPP_DEMO_REQUEST_ERROR= 0x16,
    NWY_OCPP_DEMO_RESPONSE_OK= 0x17,
    NWY_OCPP_DEMO_RESPONSE_ERROR= 0x18,
    NWY_OCPP_DEMO_RESPONSE_TIMEOUT= 0x19,
    NWY_OCPP_DEMO_RESPONSE_INVALID= 0x20,
    NWY_OCPP_DEMO_PARAM_ERROR= 0x21,
    NWY_OCPP_DEMO_BUSY= 0x22,
}NWY_OCPP_DEMO_EVENT_MSG;


//OCPP Machine States, as per defined in the documentation
typedef enum{
    Authorize,
    BootNotification,
    CancelReservation,
    ChangeAvailability,
    ChangeConfiguration,
    ClearCache,
    ClearChargingProfile,
    DataTransfer,
    DiagnosticsStatusNotification,
    FirmwareStatusNotification,
    GetConfiguration,
    GetDiagnostics,
    GetLocalListVersion,
    Heartbeat,
    MeterValues,
    RemoteStartTransaction,
    RemoteStopTransaction,
    GetCompositeSchedule,
    ReserveNow,
    Reset,
    StartTransaction,
    StatusNotification,
    StopTransaction,
    SendLocalList,
    SetChargingProfile,
    TriggerMessage,
    UnlockConnector,
    UpdateFirmware
}OCPP_States;

typedef struct _ocpp_frame
{
    char *messageCode;     //messageCode of character type
    char *messageID;       //messageID or UUID for uniquely identifying each message
    char *action;          //What action is to be used
    char *jsonPacket;     //JSON packet associated with that particular action 
}ocpp_frame;

typedef struct{
    char *msg;
    int len;
    OCPP_States ocppstates;
}ocpp_msg_s;

//UUID structure defined as mentioned in RFC4122
struct uuid
{
    ws_u32     time_low;
    ws_u16     time_mid;
    ws_u16     time_hi_and_version;
    ws_u16     clock_seq;
    ws_u8      node[6];
};

void nwy_ocpp_debug(const char *file_path, int line, const char *tag, const char *format, ...);
void nwy_ocpp_fwk_send_event(ocpp_msg_s *param,NWY_OCPP_EVENT_MSG event_msg);
int nwy_ocpp_fwk_init_task(void *param);


#endif

