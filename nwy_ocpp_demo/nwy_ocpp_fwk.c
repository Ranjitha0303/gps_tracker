
/**

* File name: nwy_ocpp_fwk.c
* Author:gz  Date:2023/3/23 
* Description:  Please read the following code before using the OCPP message framework 
                processing file: https://www.oasis-open.org/committees/download.php/58944/ocpp-1.6.pdf After that, 
                learn about the implementation mechanism of the OCPP client
* Others: 
* Function List:  
   1. int nwy_ocpp_response_message(const char *data, const unsigned int length)
   2.int nwy_ocpp_Request_msg( OCPP_States ocppstates,char *request_body)
* History: 
   1. Date: 2023/3/23
      Author:gz
      version:V1.0
  2. ...
*/

#include "nwy_ocpp_fwk.h"

//Declaring GUID 16 octects according to RFC4122 https://tools.ietf.org/html/rfc4122
typedef unsigned char uuid_t[16];
//UUID comes in lower and upper cases, to convert the GUID returned and changing it to lower format
static const char *fmt_lower ="%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";
/**
 * This Frame is most crucial part for sending OCPP packet frame from CP to CMS, the CMS does not understand packet format apart from it
 * OCPP Payload --> [
 *                     <message Code>  <data type --> int>,
 *                     <message UUID>  <data type --> string>,
 *                     <action>        <data type --> string>,
 *                     <action data>   <data type --> JSON>
 *                  ]
*/

char *ocppFrameArray = "[%s,\"%s\",\"%s\",%s]";

/**
 *  These are the message codes when any RPC call is made from CMS or CP 
*/
char *ocpp_message_call_code = "2";          //This message code is used when any RPC call is to be made
char *ocpp_message_response_code = "3";      //This message code is used when response of any RPC call is to be given
char *ocpp_message_error_code = "4";        //This message code is used when there is some error in sending packet or Transport layer

ocpp_frame *ocppSendFrame = NULL;          //OCPP Send Frame defined as per ocpp_frame structure 
ocpp_frame *ocppReceiveFrame = NULL;       //OCPP Receive Frame defined as per ocpp_frame structure 

//Message length defined for OCPP Payload
#define MESSAGE_LEN 1000

//Declaring buffer for OCPP Payload
char ocppMessagePayload[MESSAGE_LEN] = {0};

#define NWY_LOG_CFUN_NAME_MAX 15
typedef struct {
    char* flag; //%s
    int   size; //size in stack
}nwy_ocpp_fmt_flag_info_t;
//OCPP buffer used with index matching to ENUM of OCPPStates such that OCPPStates name can be used instead of its ENUM values
char *ocpp_states_Buffer[] = {
     "Authorize",
     "BootNotification",
     "CancelReservation",
     "ChangeAvailability",
     "ChangeConfiguration",
     "ClearCache",
     "ClearChargingProfile",
     "DataTransfer",
     "DiagnosticsStatusNotification",
     "FirmwareStatusNotification",
     "GetConfiguration",
     "GetDiagnostics",
     "GetLocalListVersion",
     "Heartbeat",
     "MeterValues",
     "RemoteStartTransaction",
     "RemoteStopTransaction",
     "GetCompositeSchedule",
     "ReserveNow",
     "Reset",
     "StartTransaction",
     "StatusNotification",
     "StopTransaction",
     "SendLocalList",
     "SetChargingProfile",
     "TriggerMessage",
     "UnlockConnector",
     "UpdateFirmware"

};

static nwy_osi_thread_t nwy_ocpp_fwk_thread;
static void *nwy_ocpp_socket_hdl = NULL;
static nwy_osi_timer_t g_ocpp_resp_timer_hdl;

#define NWY_OCPP_RESP_TIMEOUT  50*1000
extern void nwy_ocpp_demo_send_event(ocpp_msg_s *param,NWY_OCPP_DEMO_EVENT_MSG event_msg);
static void nwy_uuid_unpack(const uuid_t in, struct uuid *uu);
static void nwy_random_get_bytes(void *buf, ws_s32 nbytes);


static nwy_ocpp_fmt_flag_info_t __format_ocpp_flag[] = {
    { "%a", sizeof(float)},
    { "%A", sizeof(float)},
    { "%c", sizeof(char)},
    { "%C", sizeof(wchar_t)},
    { "%d", sizeof(int)},
    { "%ld", sizeof(long)},
    { "%Ld", sizeof(long)},
    { "%hd", sizeof(short)},
    { "%e", sizeof(float)},
    { "%E", sizeof(float)},
    { "%f", sizeof(float)},
    { "%.nf", sizeof(float)},
    { "%g", sizeof(float)},
    { "%G", sizeof(float)},
    { "%i", sizeof(int)},
    { "%o", sizeof(int)},
    { "%p", sizeof(void*)},
    { "%s", sizeof(char*)},
    { "%S", sizeof(wchar_t*)},
    { "%u", sizeof(unsigned int)},
    { "%x", sizeof(int)},
    { "%#x", sizeof(int)},
    { "%lld", sizeof(long long)},
    { "%llu", sizeof(unsigned long long)},
    { "%llx", sizeof(long long)}
};

//返回在栈中占用的大小, 0 表示未匹配到 flag
static int __fmt_start_with_output_flag(char* fmt){
    int i = 0;
    for(i = 0; i < sizeof(__format_ocpp_flag)/sizeof(nwy_ocpp_fmt_flag_info_t); i++){
        if(memcmp(fmt,__format_ocpp_flag[i].flag,strlen(__format_ocpp_flag[i].flag)) == 0){
            return __format_ocpp_flag[i].size;
        }
    }
    return 0;
}

static void nwy_ocpp_log(const char *file, int line,
                    const char *tag, const char *fmt, va_list ap)
{
    char *str = NULL;
    va_list va_list_copy = {0};
    int size = -1;
    char print_buf[255] = {0};
    int len = 0;
    int nwy_file_len = 0;
    char nwy_file_name[NWY_LOG_CFUN_NAME_MAX+10] = {0};

    if(fmt == NULL || strlen(fmt) == 0) return;

    if(file !=NULL)
    {
        nwy_file_len= strlen(file);

        if(nwy_file_len-NWY_LOG_CFUN_NAME_MAX>0)
        {
            snprintf(nwy_file_name,sizeof(nwy_file_name)-1,"...%s",file+(nwy_file_len-NWY_LOG_CFUN_NAME_MAX));
        }
        else
        {
            snprintf(nwy_file_name,sizeof(nwy_file_name)-1,"...%s",file);
        }
    }
    else
    {
        snprintf(nwy_file_name,sizeof(nwy_file_name)-1,"...%s","NULL");
    }
    char fmt_new[100] = {0};
    if(strlen(fmt) < sizeof(fmt_new)){
        strcpy(fmt_new,fmt);
        str = fmt_new;
        memcpy(&va_list_copy,&ap,sizeof(va_list));

        // 循环找到%s,若对应的参数为 NULL ，则替换%s为 "%d"
        do{
            str = strstr(str,"%");
            if(str == NULL){
                break;
            }
            size = __fmt_start_with_output_flag(str);

            if(size == 0){
                break;
            }

            switch(size){
                case 1:
                    va_arg(va_list_copy, int);//warning: 'char' is promoted to 'int' when passed through '...'
                break;
                case 2:
                    va_arg(va_list_copy, int);//'short int' is promoted to 'int' when passed through '...'
                break;
                case 4:
                {
                    len  = strlen(print_buf);
                    if(memcmp(str,"%s",strlen("%s")) == 0){
                        char* ptr = va_arg(va_list_copy, char*);
                        if(ptr == 0){

                            // null output, change %s to %d
                            memcpy(str,"%d",strlen("%d"));

                            snprintf(print_buf+len, sizeof(print_buf)-len, "NULL->0,");
                        } else {

                        }
                    } else {
                        va_arg(va_list_copy, int);
                    }
                }
                break;
                case 8:
                    va_arg(va_list_copy, long long);
                break;
                default :

                break;
            }
            str++;
        }while(size > 0 && str != NULL && *str != 0);

        len  = strlen(print_buf);

        if(sizeof(print_buf)-len-1 <=0) return;

        snprintf(print_buf+len,sizeof(print_buf)-len-1,"|%s|%s|%d|",tag,nwy_file_name,line);

        len  = strlen(print_buf);

        if(sizeof(print_buf)-len-1 <=0) return;

        vsnprintf(print_buf+len, sizeof(print_buf)-len-1,fmt_new, ap);
    }
    else
    {
        return;
    }

    OSI_LOGXI(OSI_LOGPAR_S, 0, "%s", print_buf);

}

void nwy_ocpp_debug(const char *file_path, int line, const char *tag, const char *format, ...)
{
    va_list valist;
    va_start(valist, format);
    nwy_ocpp_log(file_path, line, tag, format, valist);
    va_end(valist);
}

#define NWY_OCPP_FWK_LOG(format,...)        nwy_ocpp_debug(__FILE__, __LINE__, "nwy_ocpp_fwk", format, ##__VA_ARGS__)

void nwy_ocpp_fwk_send_event(ocpp_msg_s *param,NWY_OCPP_EVENT_MSG event_msg)
{
    nwy_event_msg_t event;
    ocpp_msg_s *ocpp_param = NULL;
    ocpp_param = (ocpp_msg_s *)malloc(sizeof(ocpp_msg_s));
    memset(ocpp_param,0,sizeof(ocpp_msg_s));
    memset(&event,0,sizeof(nwy_event_msg_t));
    event.id = event_msg;
    if(param != NULL)
    {
        ocpp_param->msg = malloc(param->len+1);
        memset(ocpp_param->msg,0,param->len+1);
        memcpy(ocpp_param->msg,param->msg,param->len);
        ocpp_param->len = param->len;
        ocpp_param->ocppstates = param->ocppstates;
        event.param1 = (ws_u32)ocpp_param;
    }
    nwy_send_thread_event(nwy_ocpp_fwk_thread, &event, 1);
}
/**
 *  @brief   --> This packet packs the struct uuid into GUID octets
 *  @param  --> const struct uuid **uu : uuid structure passed for generating GUID
 *               uuid_t ptr             : GUID pointer to be passed
*/

static void nwy_uuid_pack(const struct uuid *uu, uuid_t ptr)
{
    ws_u32   tmp;
    unsigned char   *out = ptr;

    tmp = uu->time_low;
    out[3] = (unsigned char)tmp;
    tmp >>= 8;
    out[2] = (unsigned char)tmp;
    tmp >>= 8;
    out[1] = (unsigned char)tmp;
    tmp >>= 8;
    out[0] = (unsigned char)tmp;

    tmp = uu->time_mid;
    out[5] = (unsigned char)tmp;
    tmp >>= 8;
    out[4] = (unsigned char)tmp;

    tmp = uu->time_hi_and_version;
    out[7] = (unsigned char) tmp;
    tmp >>= 8;
    out[6] = (unsigned char) tmp;

    tmp = uu->clock_seq;
    out[9] = (unsigned char) tmp;
    tmp >>= 8;
    out[8] = (unsigned char) tmp;

    memcpy(out+10, uu->node, 6);

}

/**
 *  @brief   --> This function generates UUID of 16 octects according to RFC4122
 *  @param  --> uuid_t out : uuid_t type of GUID octets
 *               int *num   : this is static value taken for OR and AND operations in the function
*/

static void __uuid_generate_random(uuid_t out, int *num)
{
    uuid_t buf;
    struct uuid uu;
    int i, n;

    if (!num || !*num)
        n = 1;
    else
        n = *num;

    for(i = 0; i < n; i++)
    {
        nwy_random_get_bytes(buf, sizeof(buf));
        nwy_uuid_unpack(buf, &uu);

        uu.clock_seq = (uu.clock_seq & 0x3FFF) | 0x8000;
        uu.time_hi_and_version = (uu.time_hi_and_version & 0x0FFF) | 0x4000;
        
        nwy_uuid_pack(&uu, out);
        out += sizeof(uuid_t);
    }
}

/**
 *  @brief   --> This function unpacks/parse the GUID generated by sending uuid_t to UUID struct mentioned in RFC4122
 *  @param  --> const uuid_t in : Used for sending GUID of uuid_t type
 *               struct uuid *uu : uuid structure passed for parsing the values from GUID octet to readable UUID struct
*/

static void nwy_uuid_unpack(const uuid_t in, struct uuid *uu)
{
    const ws_u8   *ptr = in;
    ws_u32        tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_low = tmp;
    
    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_mid = tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_hi_and_version = tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->clock_seq = tmp;

    memcpy(uu->node, ptr, 6);
}

/**
 *  @brief   -->  Generate a stream of random nbytes into buf.
 *               Use /dev/urandom if possible, and if not,
 *               use glibc pseudo-random functions.
 *  @param  --> void *buf      : buffer pointer of no data type where random data generated from /dev/urandom is to be copied
 *              ws_s32 nbytes  : size of the buffer or the no of bytes to be written 
*/
static void nwy_random_get_bytes(void *buf, ws_s32 nbytes)
{
    ws_s32 i =0;

    unsigned char *cp = (unsigned char *)buf;

    struct timeval tv;

    gettimeofday(&tv, 0);

    srand(tv.tv_sec ^ tv.tv_usec);

    for(i=0; i<nbytes; i++) 
    {
        cp[i] = (unsigned char)(rand() & 0xff);
    }

    return;
}
/**
 *  @brief   --> this is sub functionality of above function which first unpack the GUID octect and then prints into lower format
 *  @param  --> const uuid_t uu : uuid_t type data to be passed once it has 16 Octets generated
 *               char *out       : char pointer to be passed which will point to the UUID generated
 *               const char *fmt : lower character format defined in ocpp_helper.h file
*/

static void nwy_uuid_unparse_x(const uuid_t uu, char *out, const char *fmt)
{
    struct uuid uuid;

    nwy_uuid_unpack(uu, &uuid);
    sprintf(out, fmt,uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
    uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
    uuid.node[0], uuid.node[1], uuid.node[2],
    uuid.node[3], uuid.node[4], uuid.node[5]);
}

/**
 *  @brief   --> to parse GUID generated into readable UUID of 36 bit lower format
 *  @param  --> const uuid_t uu : uuid_t type data to be passed once it has 16 Octets generated
 *               char *out       : char pointer to be passed which will point to the UUID generated
*/

static  void nwy_uuid_unparse_lower(const uuid_t uu, char *out)
{
    nwy_uuid_unparse_x(uu, out, fmt_lower);
}




/**
 *  @brief   --> to make a structure frame for consolidated and neccessary data required for sending OCPPFrame.
 *               it returns ocpp_frame struct once parsed
 *  @param  --> char *messagecode : According to OCPP, messageCode is the code for RPC call/response/error
 *               char *actiontocms : The action that is needed to be taken by CMS, requested by CP
 *               cJSON *jsonpacket : The JSON packet to be send according to the action schema defined in OCPP1.6J schemas
*/
/*[<2>, "<UniqueId>", "<Action>", {<Payload>}]*/
ocpp_frame *nwy_formOCPPFrame(char *messagecode, char *actiontocms, char *jsonpacket)
{
    NWY_OCPP_FWK_LOG("Inside formOCPP packet\n");
    uuid_t binuuid;
    int num = 1;
    char *uuid = malloc(37);     //UUID of 36 length hence allocation 37 for 36 + '\0' 
    if (uuid == NULL)
    {
        NWY_OCPP_FWK_LOG( "Failed to provide memory for UUID\n");
    }
    ocpp_frame *frame = NULL;
    frame = (ocpp_frame *)malloc(sizeof(ocpp_frame));  //allocating memory for ocppframe
    if (frame == NULL)
    {
        NWY_OCPP_FWK_LOG( "Unable to allocate memory to ocpp frame\n");
    }
    memset(frame, 0, sizeof(ocpp_frame));                  //initializing frame with zero
    frame->messageCode = (char *)malloc(sizeof(messagecode) +1);     //allocating memory for messageCode of ocpp frame
    if(frame->messageCode == NULL)
    {
        NWY_OCPP_FWK_LOG("Unable to locate memory for messagecode");
    }
    memset(frame->messageCode, 0, strlen(messagecode) +1);     //initializing frame with zero
    __uuid_generate_random(binuuid, &num);                    //calling function to generate GUID
    nwy_uuid_unparse_lower(binuuid, uuid);                        //Parsing GUID in 36 bits char uuid lower format
    frame->messageID = (char *)malloc(strlen(uuid) +1);      //allocating memory equivalent to UUID + '\0'
    if (frame->messageID == NULL)
    {
        NWY_OCPP_FWK_LOG("Not able to assign message ID\n");
    }
    memset(frame->messageID, 0, strlen(uuid) +1);             //initializing frame with zero
    frame->action = (char *)malloc(strlen(actiontocms) +1);   //allocating memory equivalent to actiontocms
    if (frame->action == NULL)
    {
        NWY_OCPP_FWK_LOG("Unable to allocate memory of action frame\n");
    }
    memset(frame->action, 0, strlen(actiontocms) +1);       
    frame->jsonPacket = (char *)malloc(strlen(jsonpacket) +1);   //allocating memory equivalent to actiontocms
    if (frame->jsonPacket == NULL)
    {
        NWY_OCPP_FWK_LOG("Unable to allocate memory of action frame\n");
    }
    memset(frame->jsonPacket, 0, strlen(jsonpacket) +1);       
    strncpy(frame->messageCode, messagecode, strlen(messagecode));     //copying messagecode in ocpp frame
    strncpy(frame->messageID, uuid, strlen(uuid));                     //copying messageID in ocpp frame
    strncpy(frame->action, actiontocms, strlen(actiontocms));         //copying action in ocpp frame
    //frame->jsonPacket = jsonpacket;                               //frame->jsonPacket frame pointing towards jsonpacket passed through argument
    strncpy(frame->jsonPacket, jsonpacket, strlen(jsonpacket)); 
    free(uuid);                                                  //freeing memory allocated for uuid
    return frame;
}



/**
 *  @brief   --> to make ocpp payload and it to CMS and if the payload is sent successfully then return 0
 *  @param  --> wsclient *c        : websocket client, from which the payload will be sent
 *               ocpp_frame *packet : the ocpp packet frame that is to be sent
*/

//For success this message sends 0 otherwise -1
int nwy_sendFrameToCMS(ocpp_frame *packet){
    int responseLength = 0;
    int ret = 0;
    memset(ocppMessagePayload, 0, sizeof(ocppMessagePayload));
    //using snprintf printing values of OCPPFrame in ocppMessagePayload taking reference of ocppFrameArray
    snprintf(ocppMessagePayload ,MESSAGE_LEN, ocppFrameArray, packet->messageCode, packet->messageID, packet->action,packet->jsonPacket);

    responseLength = nwy_rws_send_text(nwy_ocpp_socket_hdl,ocppMessagePayload);//calling function which accepts wsClient and char *payload 
    if(responseLength == 0)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    NWY_OCPP_FWK_LOG("%s",ocppMessagePayload);
    memset(ocppMessagePayload, 0, sizeof(ocppMessagePayload));   //reinitializing the ocppMessagePayload for other messages
    return ret;
}

int nwy_ocpp_Request_msg( OCPP_States ocppstates,char *request_body)
{

    if(request_body == NULL)
    {
        return -1;
    }
    switch (ocppstates)
         {
             /**
              * BootNotification is the first state that comes in OCPP which shares the information of the CP to CMS
              */
             case BootNotification:
                NWY_OCPP_FWK_LOG( "Sending BootNotification Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case Heartbeat:
                 NWY_OCPP_FWK_LOG( "Sending Heartbeat Packet");
                 ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                 //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                 return nwy_sendFrameToCMS(ocppSendFrame);
             case Authorize:
                NWY_OCPP_FWK_LOG( "Sending Authorize Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case CancelReservation:
                NWY_OCPP_FWK_LOG( "Sending CancelReservation Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case ChangeAvailability:
                NWY_OCPP_FWK_LOG( "Sending ChangeAvailability Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case ChangeConfiguration:
                NWY_OCPP_FWK_LOG( "Sending ChangeConfiguration Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case ClearCache:
                NWY_OCPP_FWK_LOG( "Sending ClearCache Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case ClearChargingProfile:
                NWY_OCPP_FWK_LOG( "Sending ClearChargingProfile Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case DataTransfer:
                NWY_OCPP_FWK_LOG( "Sending DataTransfer Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case DiagnosticsStatusNotification:
                NWY_OCPP_FWK_LOG( "Sending DiagnosticsStatusNotification Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case FirmwareStatusNotification:
                NWY_OCPP_FWK_LOG( "Sending FirmwareStatusNotification Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case GetConfiguration:
                NWY_OCPP_FWK_LOG( "Sending GetConfiguration Packet");
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case GetDiagnostics:
                NWY_OCPP_FWK_LOG( "Sending GetDiagnostics Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case GetLocalListVersion:
                NWY_OCPP_FWK_LOG( "Sending GetLocalListVersion Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case MeterValues:
                NWY_OCPP_FWK_LOG( "Sending MeterValues Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case RemoteStartTransaction:
                NWY_OCPP_FWK_LOG( "Sending RemoteStartTransaction Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case RemoteStopTransaction:
                NWY_OCPP_FWK_LOG( "Sending RemoteStopTransaction Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case GetCompositeSchedule:
                NWY_OCPP_FWK_LOG( "Sending GetCompositeSchedule Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case ReserveNow:
                NWY_OCPP_FWK_LOG( "Sending ReserveNow Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case Reset:
                NWY_OCPP_FWK_LOG( "Sending Reset Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             case StartTransaction:
                NWY_OCPP_FWK_LOG( "Sending StartTransaction Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case StatusNotification:
                NWY_OCPP_FWK_LOG( "Sending StatusNotification Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case StopTransaction:
                NWY_OCPP_FWK_LOG( "Sending StopTransaction Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case SendLocalList:
                NWY_OCPP_FWK_LOG( "Sending SendLocalList Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case SetChargingProfile:
                NWY_OCPP_FWK_LOG( "Sending SetChargingProfile Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case TriggerMessage:
                NWY_OCPP_FWK_LOG( "Sending TriggerMessage Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case UnlockConnector:
                NWY_OCPP_FWK_LOG( "Sending UnlockConnector Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);

             case UpdateFirmware:
                NWY_OCPP_FWK_LOG( "Sending UpdateFirmware Packet");
                //calling function to form OCPP Packet for bootNotification
                ocppSendFrame = nwy_formOCPPFrame(ocpp_message_call_code, ocpp_states_Buffer[ocppstates], request_body);
                //Once OCPP packet is made, it is passed to nwy_sendFrameToCMS function, which takecare of OCPP payload to be made and then send it to server
                return nwy_sendFrameToCMS(ocppSendFrame);
             default:
                 NWY_OCPP_FWK_LOG("Inside default switch case");
                 return -1;
         }

}
static int ocpp_isMessageSend = 0;// 1 sended,0:not send
static OCPP_States ocppstates = 0; 

void nwy_ocpp_fwk_set_send_flag(int flag)
{
    ocpp_isMessageSend = flag;
}
int nwy_ocpp_fwk_get_send_flag(void)
{
    return ocpp_isMessageSend;
}
void nwy_ocpp_fwk_set_state_flag(OCPP_States state)
{
    ocppstates = state;
}
int nwy_ocpp_fwk_get_state_flag(void)
{
    return ocppstates;
}
/**
 *  @brief   --> This function sends the sub-string of the packet received and used for parsing it
 *  @param  --> char *source : source string pointer
 *               char *target : target string that is to be changed
 *               int from     : index value of source string from where values are to be parsed
 *               int to       : index value till the value needs to be parsed
*/

static int  nwy_getSubString(const char     *source, char *target,int from, int to)
{
        int length=0;
        int i=0,j=0;
        //get length
        while(source[i++]!='\0')
                length++;
        if(from<0 || from>length){
                NWY_OCPP_FWK_LOG("Invalid \'from\' index\n");
                return 1;
        }
        if(to>length){
                NWY_OCPP_FWK_LOG("Invalid \'to\' index\n");
                return 1;
        }
        for(i=from,j=0;i<=to;i++,j++){
                target[j]=source[i];
        }
        //assign NULL at the end of string
        target[j]='\0';
        return 0;
}
/**
 *  @brief   --> This function parses messages that come from CMS to CP into ocppFrame
 *  @param  --> char *payload : The response payload that comes from server to client
*/
/*[<4>, "<UniqueId>", "<errorCode>", "<errorDescription>", {<errorDetails>}]*/
/*[<3>, "<UniqueId>", {<Payload>}]*/
ocpp_frame *nwy_parseOCPPFrame(const char  *payload,int len)
{   
    int count = 0;
    ocpp_frame *frame = NULL;
    frame = (ocpp_frame *)malloc(sizeof(ocpp_frame));     //allocating memory for ocppFrame
    if (frame == NULL)
    {
        NWY_OCPP_FWK_LOG( "Unable to allocate memory to ocpp frame\n");
    }
    memset(frame, 0, sizeof(ocpp_frame));          //initializing ocppFrame with zero
    char *jsonStr = NULL;
    char *str = NULL;
    str = (char *)malloc(len+1);     //allocating memory for the payload from response to remove '[]' from the payload
    if(str == NULL){
        NWY_OCPP_FWK_LOG( "Unable to allocate memory to string");
    }
    memset(str,0,len+1);
    nwy_getSubString(payload, str, 1, len-2);  //function called to parse response and leaving both exterme end indexes 
    char delim[] = ",";                              //assigning delimiter to seperate the values
    char *ptr = strtok(str, delim);                  //strtok function used from string.h library for segregating values

    
    while(ptr != NULL)
    {
        //switch condition to address each messages seperately
        switch (count++)
        {
        //case for parsing messageCode and saving into ocppFrame->messageCode
        case 0:
            frame->messageCode = (char *)malloc(strlen(ptr)+1);
            if(frame->messageCode == NULL)
            {
                NWY_OCPP_FWK_LOG("Unable to locate memory for messagecode");
            }
            memset(frame->messageCode, 0, strlen(ptr));
            strncpy(frame->messageCode, ptr, strlen(ptr)+1);
            break;
        
        //case for parsing messageID and saving into ocppFrame->messageID
        case 1:
            goto Cleanup;
            Cleanup: ;
            char *uuid = malloc(37);
            nwy_getSubString(ptr, uuid, 1, strlen(ptr)-2); //used to remove '""' commas from the message
            frame->messageID = (char *)malloc(strlen(ptr)+1);
            if (frame->messageID == NULL)
            {
                NWY_OCPP_FWK_LOG("Not able to assign message ID\n");
            }
            memset(frame->messageID, 0, strlen(ptr)+1);
            //strncpy(frame->messageID, ptr, strlen(ptr) +1);
            strncpy(frame->messageID, uuid, strlen(uuid) +1);
            free(uuid);
            break;

        //once all the cases are addressed, this default case used in parsing the complete JSON packet since strtok will break json into different words
        default:
            if(jsonStr == NULL)
            {
                //For the firsttime allocating memory usign malloc for the first word
                jsonStr = (char *)malloc(strlen(ptr)+1);
                if(jsonStr == NULL)
                {
                    NWY_OCPP_FWK_LOG( "Error in allocating memory to jsonString\n");
                }
                strcpy(jsonStr, ptr);
            }
            else
            {
               //Reallocating memory according to the proceeding string coming 
               jsonStr = realloc(jsonStr, strlen(jsonStr)+strlen(ptr)+1);
               if(jsonStr == NULL)
               {
                   NWY_OCPP_FWK_LOG( "Error in re-allocating memory to jsonString\n");
               }
               strcat(jsonStr, ",");    
               strcat(jsonStr, ptr); 
            }

            //NWY_OCPP_FWK_LOG("jsonPacket:%s",jsonStr);
            //assigning JSON packet in the frame
            //frame->jsonPacket = cJSON_Parse(jsonStr);
            frame->jsonPacket = (char *)malloc(strlen(jsonStr)+1);
            if(frame->jsonPacket == NULL)
            {
                NWY_OCPP_FWK_LOG("Unable to locate memory for messagecode");
            }

            memset(frame->jsonPacket, 0, strlen(jsonStr)+1);
            strncpy(frame->jsonPacket, jsonStr, strlen(jsonStr));
            break;
        }
        ptr = strtok(NULL, delim);
    }


    //Freeing memory
    if(str != NULL)
        free(str);
    if(jsonStr != NULL)
        free(jsonStr);


    return frame;
}

static char nwy_ocpp_Response_message[1024] = {0};
void nwy_ocpp_free_send_msg(void)
{
    if(ocppSendFrame != NULL)
    {
        if(ocppSendFrame->messageCode != NULL)
        {
            free(ocppSendFrame->messageCode);
            ocppSendFrame->messageCode= NULL;
        }
        if(ocppSendFrame->messageID != NULL)
        {
            free(ocppSendFrame->messageID);
            ocppSendFrame->messageID= NULL;
        }
        if(ocppSendFrame->action != NULL)
        {
            free(ocppSendFrame->action);
            ocppSendFrame->action = NULL;
        }
        if(ocppSendFrame->jsonPacket != NULL)
        {
            free(ocppSendFrame->jsonPacket);
            ocppSendFrame->jsonPacket = NULL;
        }
        free(ocppSendFrame);
        ocppSendFrame = NULL;
    }

}
void nwy_ocpp_free_recv_msg(void)
{
    if(ocppReceiveFrame != NULL)
    {
        if(ocppReceiveFrame->messageCode != NULL)
        {
            free(ocppReceiveFrame->messageCode);
            ocppReceiveFrame->messageCode= NULL;
        }
        if(ocppReceiveFrame->messageID != NULL)
        {
            free(ocppReceiveFrame->messageID);
            ocppReceiveFrame->messageID= NULL;
        }
        if(ocppReceiveFrame->action != NULL)
        {
            free(ocppReceiveFrame->action);
            ocppReceiveFrame->action = NULL;
        }
        if(ocppReceiveFrame->jsonPacket != NULL)
        {
            free(ocppReceiveFrame->jsonPacket);
            ocppReceiveFrame->jsonPacket = NULL;
        }
        free(ocppReceiveFrame);
        ocppReceiveFrame = NULL;
    }

}

void nwy_ocpp_Message_processing(const char *data, const unsigned int length, OCPP_States ocppstates)
{
    ocpp_msg_s ocpp_resp_param;

    ocppReceiveFrame = nwy_parseOCPPFrame(data,length);   //calling the function which parses the char *payload to ocpp_frame
    NWY_OCPP_FWK_LOG( "---------Response code messageCode:%s",ocppReceiveFrame->messageCode);
    NWY_OCPP_FWK_LOG( "---------Response code matched:%s,",ocppReceiveFrame->messageID);
    NWY_OCPP_FWK_LOG( "---------Response code jsonPacket:%s",ocppReceiveFrame->jsonPacket);

   if(strcmp(ocppReceiveFrame->messageCode, (char *)ocpp_message_response_code) == 0) //As mentioned by OCPP, respose code should be checked for RPC call
   {
       if(ocppSendFrame != NULL)
       {
           NWY_OCPP_FWK_LOG( "Response code matched:%s,%s",ocppReceiveFrame->messageID,ocppSendFrame->messageID);
           //As per OCPP1.6J if the request is made by CP then CMS should send same messageID which is GUID for us
           if(strcmp(ocppReceiveFrame->messageID, ocppSendFrame->messageID) == 0)
           {
               //Once it is confirmed that it is response from CMS, then checking conditions according to BootNotification.response.req
               NWY_OCPP_FWK_LOG( "MessageID matched with the messageID sent:%s",ocppReceiveFrame->jsonPacket);
               memset(nwy_ocpp_Response_message,0,sizeof(nwy_ocpp_Response_message));
               snprintf(nwy_ocpp_Response_message,sizeof(nwy_ocpp_Response_message)-1,"%s",ocppReceiveFrame->jsonPacket);
               memset(&ocpp_resp_param,0,sizeof(ocpp_resp_param));
               ocpp_resp_param.len = strlen(nwy_ocpp_Response_message);
               ocpp_resp_param.ocppstates = ocppstates;
               ocpp_resp_param.msg = nwy_ocpp_Response_message;
               nwy_ocpp_demo_send_event(&ocpp_resp_param,NWY_OCPP_DEMO_RESPONSE_OK);
           }
           else
           {
               NWY_OCPP_FWK_LOG( "Message mismatch:%s",data);
           }
       }
       else
       {
           memset(nwy_ocpp_Response_message,0,sizeof(nwy_ocpp_Response_message));
           snprintf(nwy_ocpp_Response_message,sizeof(nwy_ocpp_Response_message)-1,"%s",ocppReceiveFrame->jsonPacket);
           memset(&ocpp_resp_param,0,sizeof(ocpp_resp_param));
           ocpp_resp_param.len = strlen(nwy_ocpp_Response_message);
           ocpp_resp_param.ocppstates = ocppstates;
           ocpp_resp_param.msg = nwy_ocpp_Response_message;
           nwy_ocpp_demo_send_event(&ocpp_resp_param,NWY_OCPP_DEMO_RESPONSE_INVALID);
       }
   }
   else
   {
       memset(nwy_ocpp_Response_message,0,sizeof(nwy_ocpp_Response_message));
       snprintf(nwy_ocpp_Response_message,sizeof(nwy_ocpp_Response_message)-1,"%s",ocppReceiveFrame->jsonPacket);
       memset(&ocpp_resp_param,0,sizeof(ocpp_resp_param));
       ocpp_resp_param.len = strlen(nwy_ocpp_Response_message);
       ocpp_resp_param.ocppstates = ocppstates;
       ocpp_resp_param.msg = nwy_ocpp_Response_message;
       nwy_ocpp_demo_send_event(&ocpp_resp_param,NWY_OCPP_DEMO_RESPONSE_ERROR);
       NWY_OCPP_FWK_LOG( "Received error while getting the data");
   }

   nwy_ocpp_free_send_msg();
   nwy_ocpp_free_recv_msg();
   nwy_ocpp_fwk_set_send_flag(0);
   nwy_sdk_timer_stop(g_ocpp_resp_timer_hdl);
}
int nwy_ocpp_response_message(const char *data, const unsigned int length)
{

    //This condition checks if message was send by CP or CMS
    if(nwy_ocpp_fwk_get_send_flag()==1)//Confirm that the request has been sent and wait for data to be received
    {
        //Switch condition to check which state the current machine is
        switch (nwy_ocpp_fwk_get_state_flag())
        {
            //BootNotfication case
            case BootNotification:
                NWY_OCPP_FWK_LOG( "BootNotification resp");
                nwy_ocpp_Message_processing(data,length,BootNotification);
                break;
            case Heartbeat:
                NWY_OCPP_FWK_LOG( "Heartbeat resp");
                nwy_ocpp_Message_processing(data,length,Heartbeat);
                break;
            case Authorize:
                NWY_OCPP_FWK_LOG( "Authorize resp");
                nwy_ocpp_Message_processing(data,length,Heartbeat);
               break;
            case CancelReservation:
                NWY_OCPP_FWK_LOG( "CancelReservation resp");
                nwy_ocpp_Message_processing(data,length,Heartbeat);
               break;
            case ChangeAvailability:
                NWY_OCPP_FWK_LOG( "ChangeAvailability resp");
                nwy_ocpp_Message_processing(data,length,ChangeAvailability);
               break;
            case ChangeConfiguration:
                NWY_OCPP_FWK_LOG( "ChangeConfiguration resp");
                nwy_ocpp_Message_processing(data,length,ChangeConfiguration);
               break;
            case ClearCache:
                NWY_OCPP_FWK_LOG( "ClearCache resp");
                nwy_ocpp_Message_processing(data,length,ClearCache);
               break;
            case ClearChargingProfile:
                NWY_OCPP_FWK_LOG( "ClearChargingProfile resp");
                nwy_ocpp_Message_processing(data,length,ClearChargingProfile);
               break;
            case DataTransfer:
                NWY_OCPP_FWK_LOG( "DataTransfer resp");
                nwy_ocpp_Message_processing(data,length,DataTransfer);
               break;
            case DiagnosticsStatusNotification:
                 NWY_OCPP_FWK_LOG( "DiagnosticsStatusNotification resp");
                nwy_ocpp_Message_processing(data,length,DiagnosticsStatusNotification);
               break;
            case FirmwareStatusNotification:
                NWY_OCPP_FWK_LOG( "FirmwareStatusNotification resp");
                nwy_ocpp_Message_processing(data,length,FirmwareStatusNotification);
               break;
            case GetConfiguration:
                NWY_OCPP_FWK_LOG( "GetConfiguration resp");
                nwy_ocpp_Message_processing(data,length,GetConfiguration);
               break;
            case GetDiagnostics:
                NWY_OCPP_FWK_LOG( "GetDiagnostics resp");
                nwy_ocpp_Message_processing(data,length,GetDiagnostics);
               break;
            case GetLocalListVersion:
                NWY_OCPP_FWK_LOG( "GetLocalListVersion resp");
                nwy_ocpp_Message_processing(data,length,GetLocalListVersion);
               break;
            case MeterValues:
                NWY_OCPP_FWK_LOG( "MeterValues resp");
                nwy_ocpp_Message_processing(data,length,MeterValues);
               break;
            case RemoteStartTransaction:
                NWY_OCPP_FWK_LOG( "RemoteStartTransaction resp");
                nwy_ocpp_Message_processing(data,length,RemoteStartTransaction);
               break;
            case RemoteStopTransaction:
                NWY_OCPP_FWK_LOG( "RemoteStopTransaction resp");
                nwy_ocpp_Message_processing(data,length,RemoteStopTransaction);
               break;
            case GetCompositeSchedule:
                NWY_OCPP_FWK_LOG( "GetCompositeSchedule resp");
                nwy_ocpp_Message_processing(data,length,GetCompositeSchedule);
               break;
            case ReserveNow:
                NWY_OCPP_FWK_LOG( "ReserveNow resp");
                nwy_ocpp_Message_processing(data,length,ReserveNow);
               break;
            case Reset:
                NWY_OCPP_FWK_LOG( "Reset resp");
                nwy_ocpp_Message_processing(data,length,Reset);
               break;
            case StartTransaction:
                NWY_OCPP_FWK_LOG( "StartTransaction resp");
                nwy_ocpp_Message_processing(data,length,StartTransaction);
               break;
            case StatusNotification:
                NWY_OCPP_FWK_LOG( "StatusNotification resp");
                nwy_ocpp_Message_processing(data,length,StatusNotification);
               break;
            case StopTransaction:
                NWY_OCPP_FWK_LOG( "StopTransaction resp");
                nwy_ocpp_Message_processing(data,length,StopTransaction);
               break;
            case SendLocalList:
                NWY_OCPP_FWK_LOG( "SendLocalList resp");
                nwy_ocpp_Message_processing(data,length,SendLocalList);
               break;
            case SetChargingProfile:
                NWY_OCPP_FWK_LOG( "SetChargingProfile resp");
                nwy_ocpp_Message_processing(data,length,SetChargingProfile);
               break;
            case TriggerMessage:
                NWY_OCPP_FWK_LOG( "TriggerMessage resp");
                nwy_ocpp_Message_processing(data,length,TriggerMessage);
               break;
            case UnlockConnector:
                NWY_OCPP_FWK_LOG( "UnlockConnector resp");
                nwy_ocpp_Message_processing(data,length,UnlockConnector);
               break;
            case UpdateFirmware:
                NWY_OCPP_FWK_LOG( "UpdateFirmware resp");
                nwy_ocpp_Message_processing(data,length,UpdateFirmware);
               break;

                default:
                    NWY_OCPP_FWK_LOG("Inside default switch case");
                    break;
            }
        }
    else
    {
        if(data != NULL)
        {
            NWY_OCPP_FWK_LOG("No messages have been sent:%s",data);
        }
    }
    return 0;
}

static void nwy_onocpp_socket_received_cb(void * socket, const void *data, const unsigned int length,void *arg) 
{

    char *ocpp_resp  = NULL;
    if(data != NULL && length > 0)
    {
        ocpp_resp = malloc(length+1);
        memset(ocpp_resp,0,length+1);
        memcpy(ocpp_resp,data,length);
        NWY_OCPP_FWK_LOG("nwy_onocpp_socket_received_cb: data =%d %s",length, ocpp_resp);
        nwy_ocpp_response_message((const char *)data,length);
        free(ocpp_resp);
    }
    else
    {
         NWY_OCPP_FWK_LOG("nwy_onocpp_socket_received_cb error");
         nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_RESPONSE_ERROR);
    }
}

static void nwy_onocpp_socket_disconnected_cb(void *socket,int reason, void *arg) 
{
    NWY_OCPP_FWK_LOG("nwy_ocpp:nwy_onocpp_socket_disconnected_cb reason =%d\n",reason);

    if(reason == NWY_WS_CONNECTED)
    {
        nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_REQUEST_EVENT);
    }

}


int nwy_ocpp_start(void) 
{
    nwy_rws_opt_t opt;
    nwy_rws_ssl_conf_type ssl_conf;
    char *url="ws://123.139.59.166:10121/v1";//Websocket test server 
    char *websocket_protocol="ocpp1.6";//Websocket sub protocol, using ocpp1.6

    memset(&opt,0,sizeof(nwy_rws_opt_t));
    memset(&ssl_conf, 0, sizeof(nwy_rws_ssl_conf_type));

    NWY_OCPP_FWK_LOG("nwy_ocpp_start start\n");
    opt.ping_interval = 60;
    opt.ping_timout = 10;
    opt.should_handshake = 1;
    opt.websocket_protocol = websocket_protocol;
    opt.ssl_conf = NULL;
   // nwy_sleep(2*1000);
    nwy_ocpp_socket_hdl = nwy_ws_open(url,&opt,nwy_onocpp_socket_received_cb,nwy_onocpp_socket_disconnected_cb,NULL);
    if(nwy_ocpp_socket_hdl == NULL)
    {
        NWY_OCPP_FWK_LOG("nwy_ocpp_start failed\n");
        return -1;
    }
    NWY_OCPP_FWK_LOG("nwy_ocpp_start OK\n");


    return 0;
}
void nwy_ocpp_resp_timer_cb(void *ctx)
{
    NWY_OCPP_FWK_LOG(  "Response message timeout");

    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_RESPONSE_TIMEOUT);
    nwy_ocpp_free_send_msg();
    nwy_ocpp_free_recv_msg();
    nwy_ocpp_fwk_set_send_flag(0);
    nwy_sdk_timer_stop(g_ocpp_resp_timer_hdl);
}

static void nwy_ocpp_message_task(void *param)
{

    nwy_event_msg_t event;
    ocpp_msg_s *ocpp_msg_param = NULL;

    nwy_osi_thread_t hdl;
     NWY_OCPP_FWK_LOG("nwy_ocpp_message_task entry ");

     /*"After sending a request message, a timer waits for the server to respond. The timeout indicates that the server did not respond, and the response timed out."*/
     if(nwy_get_current_thread(&hdl)<0)
     {
         NWY_OCPP_FWK_LOG("nwy_get_current_thread error");
     }
     if(g_ocpp_resp_timer_hdl == NULL)
    {
        if(nwy_sdk_timer_create(&g_ocpp_resp_timer_hdl, hdl, nwy_ocpp_resp_timer_cb, NULL)<0)
        {
            NWY_OCPP_FWK_LOG("nwy_sdk_timer_create error");
        }
    }
    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_INIT);

    while (1)
    {
        memset(&event, 0, sizeof(event));
        nwy_wait_thread_event(nwy_ocpp_fwk_thread, &event, 0);
        NWY_OCPP_FWK_LOG("wait ocpp msg:%x ",event.id);

        switch (event.id)
        {
           case NWY_OCPP_FWK_STEUP_EVENT:
                if(nwy_ocpp_start() < 0)
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_INIT_ERROR);
                }
                else
                {
                    NWY_OCPP_FWK_LOG("nwy_ocpp_message_task sleep ");
                }
                break;
            case NWY_OCPP_FWK_SEND_EVENT:
                if(nwy_ocpp_fwk_get_send_flag()==1)//Waiting for a response message
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_BUSY);
                    break;
                }
                if(event.param1 != NULL)
                {
                    ocpp_msg_param = (ocpp_msg_s *)event.param1;
                }
                else
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_PARAM_ERROR);
                    break;
                }

                if(nwy_ocpp_Request_msg(ocpp_msg_param->ocppstates,ocpp_msg_param->msg)<0)
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_REQUEST_ERROR);
                }
                else
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_REQUEST_OK);
                    nwy_ocpp_fwk_set_send_flag(1);
                    nwy_ocpp_fwk_set_state_flag(ocpp_msg_param->ocppstates);
                    nwy_sdk_timer_start(g_ocpp_resp_timer_hdl,NWY_OCPP_RESP_TIMEOUT, nwy_ocpp_resp_timer_cb,NULL, 0);
                }
                break;
            case NWY_OCPP_FWK_CLOSE_EVENT:
                if(nwy_rws_close(nwy_ocpp_socket_hdl)<0)
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_CLOSE_ERROR);
                }
                else
                {
                    nwy_ocpp_demo_send_event(NULL,NWY_OCPP_DEMO_CLOSE_OK);
                }
            default:
                break;
        }

        if(ocpp_msg_param != NULL)
        {
            if(ocpp_msg_param->msg != NULL)
            {
                free(ocpp_msg_param->msg);
                ocpp_msg_param->msg = NULL;

            }
            free(ocpp_msg_param);
            ocpp_msg_param = NULL;
        }

    }
    nwy_rws_close(nwy_ocpp_socket_hdl);
    nwy_exit_thread_self();
}

int nwy_ocpp_fwk_init_task(void *param)
{

    return nwy_create_thread(&nwy_ocpp_fwk_thread, 1024*10,
                 NWY_OSI_PRIORITY_NORMAL,"nwy_ocpp_message_task", nwy_ocpp_message_task, NULL, 10);
}

