/**

* File name: nwy_ocpp_demo.c
* Author:gz  Date:2023/3/23 
* Description:  Implemented a demo program for bootNotification requests and responses,
                which needs to be developed based on the actual business of the server;
* Others: 
* Function List:  
* History: 
   1. Date: 2023/3/23
      Author:gz
      version:V1.0
  2. ...
*/

#include "nwy_ocpp_fwk.h"
static nwy_osi_thread_t nwy_test_ocpp_demo_thread = NULL;

#define NWY_OCPP_DEMO_LOG(format,...)        nwy_ocpp_debug(__FILE__, __LINE__, "nwy_ocpp_demo", format, ##__VA_ARGS__)

static int dataCallFlag = 0;
static int handle = 0;

#if 0
void nwy_test_ocpp_dbg( char* fmt, ... )
{
  static char buf[1024];
  va_list args;
  int len =0;

  memset(buf, 0,sizeof(buf));
  
  sprintf(buf, "NWY_OCPP_DEMO:");
  len = strlen(buf);
  va_start(args, fmt);
  
  vsnprintf(&buf[len], sizeof(buf)-len-1,fmt, args);
  va_end(args);

  OSI_LOGXI(OSI_LOGPAR_S, 0, "%s\n", buf);
  nwy_usb_serial_send((char *)buf, strlen(buf));
}
#endif

void data_call_cb(   int hndl,nwy_data_call_state_t ind_state)
{
    if(ind_state == NWY_DATA_CALL_CONNECTED)
    {
        NWY_OCPP_DEMO_LOG(" data call success\n ");
        dataCallFlag  = 1;
    }
}
 static void start_data_call()
{
    nwy_data_start_call_v02_t dataCallPara;

    memset(&dataCallPara,0,sizeof(nwy_data_start_call_v02_t));
    nwy_sleep(10*1000);
    handle = nwy_data_get_srv_handle(data_call_cb);
    dataCallPara.is_auto_recon = 0;
    dataCallPara.profile_idx = 1;
    nwy_data_start_call(handle,&dataCallPara);
    while(dataCallFlag == 0)
    {
        nwy_sleep(2*1000);
    }

    return ;
}

void nwy_ocpp_demo_send_event(ocpp_msg_s *param,NWY_OCPP_DEMO_EVENT_MSG event_msg)
{
    nwy_event_msg_t event;
    ocpp_msg_s *ocpp_param = NULL;
    ocpp_param = (ocpp_msg_s *)malloc(sizeof(ocpp_msg_s));
    memset(&event,0,sizeof(nwy_event_msg_t));
    event.id = event_msg;
    if(param != NULL)
    {
        ocpp_param->msg = malloc(param->len+1);
        memset(ocpp_param->msg,0,param->len+1);
        memcpy(ocpp_param->msg,param->msg,param->len);
        ocpp_param->len = param->len;
        ocpp_param->ocppstates = param->ocppstates;
        event.param1 = (uint32)ocpp_param;
    }
    nwy_send_thread_event(nwy_test_ocpp_demo_thread, &event, 1);
}

static void nwy_ocpp_main_func(void *param)
{
    nwy_event_msg_t event;
    ocpp_msg_s ocpp_req_param;
    char *BootNotification_req_demo = "{\"chargePointVendor\": \"VendorX\", \"chargePointModel\": \"SingleSocketCharger\"}";
    ocpp_msg_s *ocpp_rep_param = NULL;
    NWY_OCPP_DEMO_LOG("nwy_ocpp_main_func entry ");

    start_data_call();

    while (1)
    {
        nwy_wait_thread_event(nwy_test_ocpp_demo_thread, &event, 0);
        
        NWY_OCPP_DEMO_LOG("wait ocpp demo event msg:%x",event.id);
        switch (event.id)
        {
            case NWY_OCPP_DEMO_INIT:
                nwy_ocpp_fwk_send_event(NULL,NWY_OCPP_FWK_STEUP_EVENT);
                break;
            case NWY_OCPP_DEMO_INIT_ERROR:
                break;
            case NWY_OCPP_DEMO_REQUEST_EVENT:
                /*
                For example, a Bootnotification request might look like this:

                [2,

                "19223201",

                "BootNotification",

                {"chargePointVendor": "VendorX", "chargePointModel": "SingleSocketCharger"}

                ]
                */
                memset(&ocpp_req_param,0,sizeof(ocpp_req_param));
                ocpp_req_param.len = strlen(BootNotification_req_demo);
                ocpp_req_param.msg = BootNotification_req_demo;
                ocpp_req_param.ocppstates = BootNotification;
                NWY_OCPP_DEMO_LOG("BootNotification.req1:%d,%s\r\n",ocpp_req_param.len,ocpp_req_param.msg);
                nwy_ocpp_fwk_send_event(&ocpp_req_param,NWY_OCPP_FWK_SEND_EVENT);
                break;
            case NWY_OCPP_DEMO_REQUEST_OK:
                NWY_OCPP_DEMO_LOG("BootNotification.req:OK");
                break;
            case NWY_OCPP_DEMO_REQUEST_ERROR:
                break;
            case NWY_OCPP_DEMO_CLOSE_OK:
                NWY_OCPP_DEMO_LOG("NWY_OCPP_DEMO_CLOSE_OK");
                break;
            case NWY_OCPP_DEMO_CLOSE_ERROR:
                NWY_OCPP_DEMO_LOG("NWY_OCPP_DEMO_CLOSE_ERROR");
                break;
            case NWY_OCPP_DEMO_RESPONSE_OK:
                 if(event.param1 != NULL)
                {
                    ocpp_rep_param = (ocpp_msg_s *)event.param1;
                    NWY_OCPP_DEMO_LOG("BootNotification.resp:%d,%s",ocpp_rep_param->len,ocpp_rep_param->msg);
                    /*Request Again*/
                    memset(&ocpp_req_param,0,sizeof(ocpp_req_param));
                    ocpp_req_param.len = strlen(BootNotification_req_demo);
                    ocpp_req_param.msg = BootNotification_req_demo;
                    ocpp_req_param.ocppstates = BootNotification;
                    NWY_OCPP_DEMO_LOG("BootNotification.req2:%d,%s\r\n",ocpp_req_param.len,ocpp_req_param.msg);
                    nwy_ocpp_fwk_send_event(&ocpp_req_param,NWY_OCPP_FWK_SEND_EVENT);
                }
                else
                {
                   NWY_OCPP_DEMO_LOG("NWY_OCPP_DEMO_RESPONSE_OK error ");
                }
                break;
            case NWY_OCPP_DEMO_RESPONSE_ERROR:
                 if(event.param1 != NULL)
                {
                    ocpp_rep_param = (ocpp_msg_s *)event.param1;
                    NWY_OCPP_DEMO_LOG("BootNotification.resp error:%d,%s ",ocpp_rep_param->len,ocpp_rep_param->msg);
                }
                else
                {
                   NWY_OCPP_DEMO_LOG("NWY_OCPP_DEMO_RESPONSE_ERROR error ");
                }
                break;
            case NWY_OCPP_DEMO_BUSY:
                NWY_OCPP_DEMO_LOG("Waiting for the server to respond, please try again later ");
                break;
            case NWY_OCPP_DEMO_RESPONSE_TIMEOUT:
                 NWY_OCPP_DEMO_LOG("Waiting for the server to respond, timeout ");
                break;
            default:
                break;
        }

        if(ocpp_rep_param != NULL)
        {
            if(ocpp_rep_param->msg != NULL)
            {
                free(ocpp_rep_param->msg);
                ocpp_rep_param->msg = NULL;

            }
            free(ocpp_rep_param);
            ocpp_rep_param = NULL;
        }
    }

    nwy_data_stop_call(handle);
    nwy_exit_thread_self();
}
int nwy_ocpp_demo_init_task(void *param)
{

   return nwy_create_thread(&nwy_test_ocpp_demo_thread, 1024*10,
                NWY_OSI_PRIORITY_NORMAL,"nwy_ocpp_main_func", nwy_ocpp_main_func, NULL, 10);


}

int appimg_enter(void *param)
{
    nwy_sleep(10*1000);
    NWY_OCPP_DEMO_LOG("nwy_ocpp_demo_init_task start ");
    nwy_ocpp_demo_init_task(NULL);
    NWY_OCPP_DEMO_LOG("nwy_ocpp_fwk_init_task start ");
    nwy_ocpp_fwk_init_task(NULL);
    return 0;
}

void appimg_exit(void)
{
}

































