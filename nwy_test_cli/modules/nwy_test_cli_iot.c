#include "nwy_test_cli_utils.h"
#if (defined(FEATURE_NWY_PAHO_MQTT_V5)) || (defined(FEATURE_NWY_PAHO_MQTT_V3))
#include "MQTTClient.h"
#include "nwy_mqtt.h"
#include "nwy_osi_api.h"
#endif
#ifdef FEATURE_NEOWAY_PIPECLOUD
#include "nwy_pipecloud.h"
#endif
int test_connect = 0 ;
#ifdef FEATURE_NWY_ALI_MQTT_V4X
#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"


char *product_key ="a1cPP8Xe4Ax";
char *device_name ="TEST_T8";
char *device_secret ="dP1UMS0gKlNsvBrPvEfiChBmpYnz2lNI";
char *mqtt_host = "a1cPP8Xe4Ax.iot-as-mqtt.cn-shanghai.aliyuncs.com";

extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;
extern const char *ali_ca_cert;

nwy_osi_thread_t g_mqtt_process_thread = NULL;
nwy_osi_thread_t g_mqtt_recv_thread = NULL;
static uint8_t g_mqtt_process_thread_running = 0;
static uint8_t g_mqtt_recv_thread_running = 0;
void *mqtt_handle = NULL;

int32_t demo_state_logcb(int32_t code, char *message)
{
    //nwy_test_cli_echo("logcb:%s", message);
    return 0;
}

void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type) {
        case AIOT_MQTTEVT_CONNECT: {
            nwy_test_cli_echo("\r\nAIOT_MQTTEVT_CONNECT\n");
        }
        break;

        case AIOT_MQTTEVT_RECONNECT: {
            nwy_test_cli_echo("\r\nAIOT_MQTTEVT_RECONNECT\n");
        }
        break;

        case AIOT_MQTTEVT_DISCONNECT: {
            char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :
                          ("heartbeat disconnect");
            nwy_test_cli_echo("\r\nAIOT_MQTTEVT_DISCONNECT: %s\n", cause);
        }
        break;

        default: {
        }
    }
}

void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
            //nwy_test_cli_echo("heartbeat response\n");
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
            nwy_test_cli_echo("\r\nsuback, res: -0x%04X, packet id: %d, max qos: %d\n",
                   -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            nwy_test_cli_echo("\r\npub, qos: %d, topic: %.*s\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
            nwy_test_cli_echo("\r\npub, payload: %.*s\n", packet->data.pub.payload_len, packet->data.pub.payload);
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            nwy_test_cli_echo("\r\npuback, packet id: %d\n", packet->data.pub_ack.packet_id);
        }
        break;

        default: {

        }
    }
}
//heart package
void demo_mqtt_process_thread(void *args)
{
    int32_t res = STATE_SUCCESS;
    while(1)
    {
        if(g_mqtt_process_thread_running)
        {
            res = aiot_mqtt_process(mqtt_handle);
            if (res == STATE_USER_INPUT_EXEC_DISABLED) 
            {
                nwy_test_cli_echo("\r\nmqtt handle is NULL\n suspend_thread");
                nwy_suspend_thread(g_mqtt_recv_thread);
            }
        }
        else
            nwy_suspend_thread(g_mqtt_process_thread);
        nwy_sleep(200);
    }
}

void demo_mqtt_recv_thread(void *args)
{
    int32_t res = STATE_SUCCESS;
    while(1)
    {
        if(g_mqtt_recv_thread_running)
        {
            res = aiot_mqtt_recv(mqtt_handle);
            if (res == STATE_USER_INPUT_EXEC_DISABLED)
            {
                nwy_test_cli_echo("\r\nmqtt handle is NULL\n suspend_thread");
                nwy_suspend_thread(g_mqtt_recv_thread);
            }
        }
        else
            nwy_suspend_thread(g_mqtt_recv_thread);
        nwy_sleep(200);
    }
}
void nwy_test_cli_alimqtt_connect()
{
    int32_t     res = STATE_SUCCESS;
    uint16_t    port = 1883;
    aiot_sysdep_network_cred_t cred;

    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);
    aiot_state_set_logcb(demo_state_logcb);

    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;
    cred.max_tls_fragment = 2048;
    cred.sni_enabled = 1;
    cred.x509_server_cert = ali_ca_cert;
    cred.x509_server_cert_len = strlen(ali_ca_cert);
    mqtt_handle = aiot_mqtt_init();
    if (mqtt_handle == NULL) {
        nwy_test_cli_echo("\r\naiot_mqtt_init failed\n");
        return -1;
    }

    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)mqtt_host);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)demo_mqtt_default_recv_handler);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)demo_mqtt_event_handler);

    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        aiot_mqtt_deinit(&mqtt_handle);
        nwy_test_cli_echo("\r\naiot_mqtt_connect failed: -0x%04X", -res);
        nwy_test_cli_echo("\r\nplease check variables like mqtt_host, produt_key, device_name, device_secret in demo\r\n");
        return -1;
    }
    nwy_test_cli_echo("\r\nMQTT construct success\n");
    if(nwy_at_ali_yeild_task_init())
        nwy_test_cli_echo("\r\nnwy_at_ali_yeild_task_init failed\n");
    else
        nwy_test_cli_echo("\r\nnwy_at_ali_yeild_task_init success\n");
}

int nwy_at_ali_yeild_task_init(void)
{
   int32_t res = STATE_SUCCESS;
   g_mqtt_process_thread_running = 1;
    if (g_mqtt_process_thread == NULL)
    {
        res = nwy_create_thread(&g_mqtt_process_thread, 1024 * 2, NWY_OSI_PRIORITY_NORMAL, "neo_ali_process_task", demo_mqtt_process_thread,
                                    NULL , 4);
        if(res)
        {
            nwy_test_cli_echo("\r\ng_mqtt_process_thread create failed");
            return -1;
        }
    }
    else
        nwy_resume_thread(g_mqtt_process_thread);
    nwy_sleep(100);
    g_mqtt_recv_thread_running = 1;
    if (g_mqtt_recv_thread == NULL)
    {
        nwy_create_thread(&g_mqtt_recv_thread, 1024 * 4, NWY_OSI_PRIORITY_NORMAL, "neo_ali_recv_task", demo_mqtt_recv_thread,
                                    NULL , 4);
        if(res)
        {
            nwy_test_cli_echo("\r\g_mqtt_recv_thread create failed");
            return -1;
        }
    }
    else
        nwy_resume_thread(g_mqtt_recv_thread);
    nwy_sleep(100);
    return 0;
}

void nwy_test_cli_alimqtt_pub()
{
    int32_t res = STATE_SUCCESS;
    char topic[128] = {0};
    sprintf(topic, "/sys/%s/%s/thing/status/update", product_key, device_name);
    char *pub_payload = "{\"id\":\"12\",\"params\":{\"temperature\":18,\"data\":\"2020/04/03\"}}";
    nwy_test_cli_echo("\r\naiot_mqtt_pub topic %s\n", topic);
    res = aiot_mqtt_pub(mqtt_handle, topic, (uint8_t *)pub_payload, (uint32_t)strlen(pub_payload), 0);
    if (res < 0)
        nwy_test_cli_echo("\r\naiot_mqtt_pub failed, res: -0x%04X\n", -res);
    else
        nwy_test_cli_echo("\r\naiot_mqtt_pub success");
}

void nwy_test_cli_alimqtt_sub()
{
    int32_t res = STATE_SUCCESS;
    char topic[128] = {0};
    sprintf(topic, "/sys/%s/%s/thing/status/update", product_key, device_name);
    nwy_test_cli_echo("\r\naiot_mqtt_sub topic %s\n", topic);
    res = aiot_mqtt_sub(mqtt_handle, topic, NULL, 1, NULL);
    if (res < 0)
        nwy_test_cli_echo("\r\naiot_mqtt_sub failed, res: -0x%04X\n", -res);
    else
        nwy_test_cli_echo("\r\naiot_mqtt_sub success");
}
void nwy_test_cli_alimqtt_unsub()
{
    int32_t res = STATE_SUCCESS;
    char topic[128] = {0};
    sprintf(topic, "/sys/%s/%s/thing/status/update", product_key, device_name);
    nwy_test_cli_echo("\r\naiot_mqtt_unsub topic %s\n", topic);
    res = aiot_mqtt_unsub(mqtt_handle, topic);
    if (res < 0)
        nwy_test_cli_echo("\r\naiot_mqtt_unsub failed, res: -0x%04X\n", -res);
    else
        nwy_test_cli_echo("\r\naiot_mqtt_unsub success");
}
void nwy_test_cli_alimqtt_state()
{
    int32_t res;
    if( aiot_mqtt_state(mqtt_handle)==STATE_SUCCESS )
        nwy_test_cli_echo("\r\naiot_mqtt_status connected");
    else
        nwy_test_cli_echo("\r\naiot_mqtt_status disconnected");
}
void nwy_test_cli_alimqtt_disconnect()
{
    int32_t res = STATE_SUCCESS;
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        aiot_mqtt_deinit(&mqtt_handle);
        nwy_test_cli_echo("\r\naiot_mqtt_disconnect failed: -0x%04X\n", -res);
        return ;
    }
    g_mqtt_recv_thread_running = 0;
    g_mqtt_process_thread_running = 0;
    res = aiot_mqtt_deinit(&mqtt_handle);
    if (res < STATE_SUCCESS) {
        nwy_test_cli_echo("\r\naiot_mqtt_deinit failed: -0x%04X\n", -res);
        return ;
    }
    nwy_test_cli_echo("\r\naiot_mqtt disconnect ok");
}
void nwy_test_cli_alimqtt_del_kv()
{
    nwy_test_cli_echo("\r\nnot support");
    return ;
}

#endif

#ifdef FEATURE_NWY_ALI_MQTT
#include "mqtt_api.h"
#endif
/**************************ALI MQTT*********************************/
char *sptr;

#ifdef FEATURE_NWY_ALI_MQTT

static void *pclient = NULL;

#define MQTT_PRODUCT_KEY "a1cPP8Xe4Ax"
#define MQTT_DEVICE_NAME "TEST_T8"
#define MQTT_DEVICE_SECRET "dP1UMS0gKlNsvBrPvEfiChBmpYnz2lNI"
#define MSG_LEN_MAX (1024)

#define MQTT_PRODUCT_KEY_AUTH1 "a1TezxwJJ2l"
#define MQTT_DEVICE_NAME_AUTH1 "test_device_sy"
#define MQTT_PRODUCT_SECRET_AUTH1 "gqZAOJ1wsnLln3h4"
#define _KV_FILE_NAME_         "/linkkit_kv.bin"
static char g_empty_string[1] = "";
int auth_mode = 0;

static int mqtt_sub_flag = -1;
static int mqtt_pub_flag = -1;
static int mqtt_unsub_flag = -1;
typedef struct
{
    char topic_name[128];
    iotx_mqtt_event_handle_func_fpt topic_handle_func;
}topic_list_t;
static topic_list_t topic_list[5] = {0};
int check_topic_exit(char *topic_name)
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        if(strlen(topic_list[i].topic_name) && strstr(topic_list[i].topic_name, topic_name))
            return 1;
    }
    return 0;
}
int update_topic_list(char *topic_name, int type)
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        if(type == 1)
        {
            if(strlen(topic_list[i].topic_name) == 0)
            {
                strncpy(topic_list[i].topic_name, topic_name, strlen(topic_name));
                return 1;
            }
        }
        else
        {
            if(strstr(topic_list[i].topic_name, topic_name))
            {
                memset(topic_list[i].topic_name, 0, sizeof(topic_list[i].topic_name));
                return 1;
            }
        }
    }
    nwy_test_cli_echo("update failed\r\n");
    return 0;
}


static void message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt)msg->msg;
    char topic_name[256] = {0};
    strncpy(topic_name, ptopic_info->ptopic, ptopic_info->topic_len);
    nwy_test_cli_echo("\r\ntopic_len:%d, Topic:%s, payload_len:%d, Payload:%s\r\n",
                      ptopic_info->topic_len, topic_name,
                      ptopic_info->payload_len, ptopic_info->payload);
}

static void event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type)
    {
    case IOTX_MQTT_EVENT_UNDEF:
        nwy_test_cli_echo("\r\nundefined event occur.");
        break;

    case IOTX_MQTT_EVENT_DISCONNECT:
        nwy_test_cli_echo("\r\nMQTT disconnect.");
        break;

    case IOTX_MQTT_EVENT_RECONNECT:
        nwy_test_cli_echo("\r\nMQTT reconnect.");
        break;

    case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
        mqtt_sub_flag = 1;
        nwy_test_cli_echo("\r\nsubscribe success, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
        mqtt_sub_flag = 0;
        nwy_test_cli_echo("\r\nsubscribe wait ack timeout, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
        mqtt_sub_flag = 0;
        nwy_test_cli_echo("\r\nsubscribe nack, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
        mqtt_unsub_flag = 1;
        nwy_test_cli_echo("\r\nunsubscribe success, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
        mqtt_unsub_flag = 0;
        nwy_test_cli_echo("\r\nunsubscribe timeout, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
        mqtt_unsub_flag = 0;
        nwy_test_cli_echo("\r\nunsubscribe nack, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
        mqtt_pub_flag = 1;
        nwy_test_cli_echo("\r\npublish success, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
        mqtt_pub_flag = 0;
        nwy_test_cli_echo("\r\npublish timeout, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_NACK:
        mqtt_pub_flag = 0;
        nwy_test_cli_echo("\r\npublish nack, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
        nwy_test_cli_echo("\r\ntopic message arrived but without any related handle: topic=%.*s, topic_msg=%.*s",
                          topic_info->topic_len,
                          topic_info->ptopic,
                          topic_info->payload_len,
                          topic_info->payload);
        break;

    default:
        nwy_test_cli_echo("\r\nShould NOT arrive here.");
        break;
    }
}

nwy_osi_thread_t task_id = NULL;

void nwy_ali_cycle(void *ctx)
{
    int ret = 0;

    for (;;)
    {
        while (IOT_MQTT_CheckStateNormal(pclient))
        {
            ret = IOT_MQTT_Yield(pclient, 300);
            nwy_sleep(100);
            if (ret != 0)
            {
                IOT_MQTT_Destroy(&pclient);
                break;
            }
        }
        nwy_suspend_thread(task_id);
    }
}

nwy_osi_thread_t nwy_at_ali_yeild_task_init(void)
{
    if (task_id == NULL)
    {
        nwy_create_thread(&task_id, 1024 * 15, NWY_OSI_PRIORITY_NORMAL, "neo_ali_yeild_task", nwy_ali_cycle,
                                    NULL , 4);
    }
    else
    {
        nwy_resume_thread(task_id);
    }

    nwy_test_cli_echo(" CreateThread neo_ali_yeild_task\n");
    return task_id;
}

int nwy_Snprintf(char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int rc;
    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);
    return rc;
}

int iotx_midreport_topic(char *topic_name, char *topic_head, char *product_key, char *device_name)
{
    int ret;
    /* reported topic name: "/sys/${productKey}/${deviceName}/thing/status/update" */
    int len = strlen(product_key) + strlen(device_name) + 128;
    ret = nwy_Snprintf(topic_name,
                       len,
                       "%s/sys/%s/%s/thing/status/update",
                       topic_head,
                       product_key,
                       device_name);
    return ret;
}

static int dynreg_device_secret(const char *device_secret)
{
    int rc = -1;
    int lenth = strlen(device_secret);
    nwy_test_cli_echo("callback func device secret: %s,len=%d", device_secret,lenth);
    if(lenth > IOTX_DEVICE_SECRET_LEN || lenth <= 0)
    {
        return -1;
    }
    IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)device_secret);
    rc = HAL_Kv_Set(_KV_FILE_NAME_, (void*)device_secret, lenth, 1);
    if(rc < 0)
    {
        nwy_test_cli_echo("dynreg_device_secret to KV_SET ERROR rc= %d",rc);
        return -1;
    }
    return 0;
}


void nwy_test_cli_alimqtt_connect()
{
    int domain_type = IOTX_CLOUD_DOMAIN_SH;
    char topic[128] = {0};
    static iotx_conn_info_pt pconn_info = NULL;
    static iotx_mqtt_param_t mqtt_params;
    nwy_osi_thread_t id = NULL;
    unsigned keepalive = 120000;
    unsigned int clean = 0;
    unsigned int timeout = 30000;
    int ret = 0;
    int dynamic_register = 0;
    iotx_mqtt_topic_info_t topic_info;
    char msg[MSG_LEN_MAX] = {0};
    int device_secret_len = IOTX_DEVICE_SECRET_LEN;
    char device_secret[IOTX_DEVICE_SECRET_LEN] = {0,};
    nwy_test_cli_echo("\r\nalimqtt test");
 
    sptr = nwy_test_cli_input_gets("\r\nPlease input authmode(0/1): ");
    auth_mode = atoi(sptr);
    if (auth_mode > 1 || auth_mode < 0)
    {
        nwy_test_cli_echo("\r\ninput auth_mode error");
        return;
    }
    nwy_authmode_type_set(auth_mode);
    if(1 == auth_mode)
    {
        if(0 == HAL_Kv_Get(_KV_FILE_NAME_, (void*)device_secret, &device_secret_len))
        {
            nwy_test_cli_echo("\r\nHAL_Kv_Get OK device_secret=%s,len=%d",device_secret,device_secret_len);
            dynamic_register = 0;
            //memset(g_nwy_alimqtt_at_param.devorproSecret, 0, IOTX_DEVICE_SECRET_LEN + 1);
            //memcpy(g_nwy_alimqtt_at_param.devorproSecret, device_secret, device_secret_len);
            IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)device_secret);
            IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_SECRET, (void *)g_empty_string);
        }
        else
        {
            nwy_test_cli_echo("HAL_Kv_Get NULL do ITE_DYNREG_DEVICE_SECRET");
            dynamic_register = 1;
            IOT_RegisterCallback(ITE_DYNREG_DEVICE_SECRET, dynreg_device_secret);
            IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)g_empty_string);
            IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_SECRET, (void*)MQTT_PRODUCT_SECRET_AUTH1);
            strcpy(device_secret, MQTT_PRODUCT_SECRET_AUTH1);
        }
        IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_KEY, (void *)MQTT_PRODUCT_KEY_AUTH1);
        IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_NAME, (void *)MQTT_DEVICE_NAME_AUTH1);
        nwy_test_cli_echo("\r\setup with %s,%s,%s\n", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1, device_secret);
    }
    else
    {
        if(2 == auth_mode)
        {
            //nwy_alicloud_X509_conn_init(info->engine);
            nwy_test_cli_echo("\r\nauthmode 2 not support");
            return ;
        }
        dynamic_register = 0;
        IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)MQTT_DEVICE_SECRET);
        IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_SECRET, (void *)g_empty_string);
        IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_KEY, (void *)MQTT_PRODUCT_KEY);
        IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_NAME, (void *)MQTT_DEVICE_NAME);
        strcpy(device_secret, MQTT_DEVICE_SECRET);
        nwy_test_cli_echo("\r\setup with %s,%s,%s\n", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME, device_secret);
    }
    IOT_Ioctl(IOTX_IOCTL_SET_DYNAMIC_REGISTER, (void *)&dynamic_register);
    IOT_Ioctl(IOTX_IOCTL_SET_REGION, (void *)&domain_type);

    //IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)MQTT_DEVICE_SECRET);
    ret = IOT_SetupConnInfo(MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME, device_secret, (void **)&pconn_info);
    if (ret == 0)
    {
        if ((NULL == pconn_info) || (0 == pconn_info->client_id))
        {
            nwy_test_cli_echo("\r\nplease auth fail\n");
            return;
        }
        nwy_test_cli_echo("\r\nIOT_SetupConnInfo SUCCESS\r\n");
        memset(&mqtt_params, 0x0, sizeof(mqtt_params));
        mqtt_params.port = pconn_info->port;
        mqtt_params.host = pconn_info->host_name;
        mqtt_params.client_id = pconn_info->client_id;
        mqtt_params.username = pconn_info->username;
        mqtt_params.password = pconn_info->password;
        mqtt_params.pub_key = pconn_info->pub_key;

        mqtt_params.request_timeout_ms = timeout;
        mqtt_params.clean_session = clean;
        mqtt_params.keepalive_interval_ms = keepalive;
        //mqtt_params.pread_buf = msg_readbuf;
        mqtt_params.read_buf_size = MSG_LEN_MAX;
        //mqtt_params.pwrite_buf = msg_writebuf;
        mqtt_params.write_buf_size = MSG_LEN_MAX;

        mqtt_params.handle_event.h_fp = event_handle;
        mqtt_params.handle_event.pcontext = NULL;
        ret = IOT_MQTT_CheckStateNormal(pclient);
        if (1 == ret)
        {
            nwy_test_cli_echo("\r\nMQTT is connected");
            return;
        }
        pclient = IOT_MQTT_Construct(&mqtt_params);
        if (NULL == pclient)
        {
            nwy_test_cli_echo("\r\nMQTT construct failed\n");
            return;
        }
        else
        {
            nwy_test_cli_echo("\r\nMQTT construct success\n");
            id = nwy_at_ali_yeild_task_init();
            if (NULL == id)
            {
                nwy_test_cli_echo("\r\nMQTT construct failed\n");
                return;
            }
        }
        if(1 == auth_mode)
            iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
        else
            iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);
        nwy_test_cli_echo("\r\ntopic = %s\r\n", topic);
        memset(topic_list, 0, sizeof(topic_list));
        ret = IOT_MQTT_Subscribe(pclient, topic, IOTX_MQTT_QOS1, message_arrive, NULL);
        if (ret < 0)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error");
            return;
        }
        while (1)
        {
            if (1 == mqtt_sub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe OK\r\n");
                update_topic_list(topic, 1);
                break;
            }
            else if (0 == mqtt_sub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error\r\n");
                break;
            }
            nwy_sleep(100);
        }
        /*
        iotx_midreport_reqid(requestId,
         dev->product_key,
         dev->device_name)
        iotx_midreport_payload(msg, )/*/
        strcpy(msg, "{hello word}");
        topic_info.qos = IOTX_MQTT_QOS1;
        topic_info.payload = (void *)msg;
        topic_info.payload_len = strlen(msg);
        topic_info.retain = 0;
        topic_info.dup = 0;
        ret = IOT_MQTT_Publish(pclient, topic, &topic_info);
        if (ret < 0)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
            return;
        }

        while (1)
        {
            if (1 == mqtt_pub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Publish OK");
                break;
            }
            else if (0 == mqtt_pub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
                break;
            }
            nwy_sleep(100);
        }
    }
    else
        nwy_test_cli_echo("\r\nMQTT auth failed");
}

void nwy_test_cli_alimqtt_pub()
{
    char msg[MSG_LEN_MAX] = {0};
    int ret = 0;
    iotx_mqtt_topic_info_t topic_info;
    char topic[128] = {0};

    if(auth_mode == 1)
    {
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
        strcpy(msg, "{\"LightStatus\":\"true\"}");
    }
    else
    {
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);
        strcpy(msg, "{\"id\":\"12\",\"params\":{\"temperature\":18,\"data\":\"2020/04/03\"}}");
    }
    topic_info.qos = IOTX_MQTT_QOS1;
    topic_info.payload = (void *)msg;
    topic_info.payload_len = strlen(msg);
    topic_info.retain = 0;
    topic_info.dup = 0;
    ret = IOT_MQTT_Publish(pclient, topic, &topic_info);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
        return;
    }

    while (1)
    {
        if (1 == mqtt_pub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Publish OK");
            break;
        }
        else if (0 == mqtt_pub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
            break;
        }
        nwy_sleep(100);
    }
}

void nwy_test_cli_alimqtt_sub()
{
    char topic[128] = {0};
    int ret = 0;
    if(auth_mode == 1)
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
    else
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);

    nwy_test_cli_echo("\r\ntopic = %s", topic);
    if(check_topic_exit(topic))
    {
        nwy_test_cli_echo("topic has been subscibe\r\n");
        return ;
    }
    ret = IOT_MQTT_Subscribe(pclient, topic, IOTX_MQTT_QOS1, message_arrive, NULL);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error");
        return;
    }
    mqtt_sub_flag = -1;

    while (1)
    {
        if (1 == mqtt_sub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe OK");
            update_topic_list(topic, 1);
            break;
        }
        else if (0 == mqtt_sub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error");
            break;
        }
        nwy_sleep(100);
    }
}

void nwy_test_cli_alimqtt_unsub()
{
    int rc = 0;
    char topic[128] = {0};
    if(auth_mode == 1)
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
    else
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);
    rc = IOT_MQTT_Unsubscribe(pclient, topic);
    if (rc < 0)
    {
        nwy_test_cli_echo("\r\nIOT_MQTT_Unsubscribe error");
        return;
    }
    while (1)
    {
        if (1 == mqtt_unsub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Unsubscribe OK");
            update_topic_list(topic, 0);
            break;
        }
        else if (0 == mqtt_unsub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Unsubscribe error");
            break;
        }
        nwy_sleep(100);
    }
}

void nwy_test_cli_alimqtt_state()
{
    int ret = 0;
    ret = IOT_MQTT_CheckStateNormal(pclient);
    nwy_test_cli_echo("\r\nMQTT state is %d", ret);
}

void nwy_test_cli_alimqtt_disconnect()
{
    int ret = 0;
    ret = IOT_MQTT_CheckStateNormal(pclient);
    if (ret == 0)
        nwy_test_cli_echo("\r\nMQTT is disconnected ");
    else
    {
        IOT_MQTT_Destroy(&pclient);
        pclient = NULL;
        nwy_test_cli_echo("\r\nMQTT is Destroy");
    }
}
void nwy_test_cli_alimqtt_del_kv()
{
    HAL_Kv_Del(_KV_FILE_NAME_);
    nwy_test_cli_echo("\r\nALIMQTT dek kv success");
}

#endif
#ifdef FEATURE_NWY_PAHO_MQTT_V3
/**************************MQTT*********************************/
MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTClient paho_mqtt_client;
unsigned char *g_nwy_paho_readbuf = NULL;
unsigned char *g_nwy_paho_writebuf = NULL;
nwy_osi_thread_t nwy_paho_task_id = NULL;
Network *paho_network = NULL;
#define NWY_EXT_SIO_PER_LEN 1024
char echo_buff[NWY_EXT_SIO_PER_LEN + 1] = {0};
nwy_paho_mqtt_at_param_type paho_mqtt_at_param = {0};

void messageArrived(MessageData *md)
{
    char topic_name[PAHO_TOPIC_LEN_MAX] = {0};
    int i = 0;
    unsigned int remain_len = 0;
    strncpy(topic_name, md->topicName->lenstring.data, md->topicName->lenstring.len);
    nwy_test_cli_echo("\r\n===messageArrived======");
    nwy_test_cli_echo("\r\npayloader len is %d", md->message->payloadlen);
    nwy_test_cli_echo("\r\n%s:\r\n", topic_name);
    remain_len = md->message->payloadlen;

    if (md->message->payloadlen > NWY_EXT_SIO_PER_LEN)
    {
        for (i = 0; i < ((md->message->payloadlen / NWY_EXT_SIO_PER_LEN) + 1); i++)
        {
            memset(echo_buff, 0, sizeof(echo_buff));
            strncpy(echo_buff, md->message->payload + i * NWY_EXT_SIO_PER_LEN,
                    remain_len > NWY_EXT_SIO_PER_LEN ? NWY_EXT_SIO_PER_LEN : remain_len);
            remain_len = md->message->payloadlen - (i + 1) * NWY_EXT_SIO_PER_LEN;
            nwy_test_cli_echo(echo_buff);
        }
    }
    else
    {
        memset(echo_buff, 0, sizeof(echo_buff));
        strncpy(echo_buff, md->message->payload, md->message->payloadlen);
        nwy_test_cli_echo(echo_buff);
    }
}

void nwy_paho_cycle(void *ctx)
{
    while (1)
    {
        while (MQTTIsConnected(&paho_mqtt_client))
        {
            MQTTYield(&paho_mqtt_client, 500);
            nwy_sleep(200);
        }
        nwy_test_cli_echo("\r\nMQTT disconnect ,Out paho cycle");
        nwy_suspend_thread(nwy_paho_task_id);
    }
    nwy_sleep(200);
}
nwy_osi_thread_t nwy_paho_yeild_task_init(void)
{

    if (nwy_paho_task_id == NULL)
    {
        nwy_create_thread(&nwy_paho_task_id, 1024 * 2,NWY_OSI_PRIORITY_NORMAL,"neo_paho_yeild_task", nwy_paho_cycle,
                                                     NULL, 4);
    }
    else
        nwy_resume_thread(nwy_paho_task_id);
    return nwy_paho_task_id;
}
void nwy_test_cli_mqtt_connect()
{
    int rc;
    nwy_test_cli_echo("\r\nnwy_test_cli_mqtt_connect\r\n");
    if (MQTTIsConnected(&paho_mqtt_client) == 1)
    {
        nwy_test_cli_echo("\r\npaho mqtt already connect");
        return;
    }
    memset(&paho_mqtt_at_param, 0, sizeof(nwy_paho_mqtt_at_param_type));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input url/ip: ");
    sptr = "mqtt.emotorad.com";
    strncpy(paho_mqtt_at_param.host_name, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input port: ");
    sptr = 8883;
    paho_mqtt_at_param.port = atoi(sptr);

    //sptr = nwy_test_cli_input_gets("\r\nPlease input client_id: ");
    sptr = "test_lat_long";
    strncpy(paho_mqtt_at_param.clientID, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input usrname: ");
    sptr = "test";
    strncpy(paho_mqtt_at_param.username, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input password: ");
    sptr = "3Motorad";
    strncpy(paho_mqtt_at_param.password, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input sslmode(1-ssl,0-no ssl): ");
    sptr = 1;
    if (atoi(sptr) > 1 || atoi(sptr) < 0)
    {
        nwy_test_cli_echo("\r\ninput sslmode error");
        return;
    }
    paho_mqtt_at_param.paho_ssl_tcp_conf.sslmode = atoi(sptr);
    if (g_nwy_paho_writebuf != NULL)
    {
        free(g_nwy_paho_writebuf);
        g_nwy_paho_writebuf = NULL;
    }
    if (NULL == (g_nwy_paho_writebuf = (unsigned char *)malloc(PAHO_PLAYOAD_LEN_MAX)))
    {
        nwy_test_cli_echo("\r\nmalloc buffer g_nwy_paho_writebuf error");
        return;
    }
    if (g_nwy_paho_readbuf != NULL)
    {
        free(g_nwy_paho_readbuf);
        g_nwy_paho_readbuf = NULL;
    }
    if (NULL == (g_nwy_paho_readbuf = (unsigned char *)malloc(PAHO_PLAYOAD_LEN_MAX)))
    {
        nwy_test_cli_echo("\r\nmalloc buffer g_nwy_paho_readbuf error");
        return;
    }
    memset(g_nwy_paho_readbuf, 0, PAHO_PLAYOAD_LEN_MAX);
    memset(g_nwy_paho_writebuf, 0, PAHO_PLAYOAD_LEN_MAX);
    if (paho_network != NULL)
    {
        free(paho_network);
        paho_network = NULL;
    }
    if (NULL == (paho_network = (Network *)malloc(sizeof(Network))))
    {
        nwy_test_cli_echo("\r\nmalloc buffer g_nwy_paho_readbuf error");
        return;
    }
    memset(paho_network, 0, sizeof(Network));
    NetworkInit(paho_network);
    if (paho_mqtt_at_param.paho_ssl_tcp_conf.sslmode == 1)
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease input auth_mode(0/1/2): ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput auth_mode error");
            return;
        }
        paho_mqtt_at_param.paho_ssl_tcp_conf.authmode = atoi(sptr);
        if (paho_mqtt_at_param.paho_ssl_tcp_conf.authmode == 0)
        {
            paho_network->tlsConnectParams.pRootCALocation = NULL;
            paho_network->tlsConnectParams.pDeviceCertLocation = NULL;
            paho_network->tlsConnectParams.pDevicePrivateKeyLocation = NULL;
        }
        else if (paho_mqtt_at_param.paho_ssl_tcp_conf.authmode == 1)
        {
            sptr = nwy_test_cli_input_gets("\r\nPlease input ca: ");
            strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, sptr, strlen(sptr));
            paho_network->tlsConnectParams.pRootCALocation = paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name;
            paho_network->tlsConnectParams.pDeviceCertLocation = NULL;
            paho_network->tlsConnectParams.pDevicePrivateKeyLocation = NULL;
        }
        else
        {
            sptr = nwy_test_cli_input_gets("\r\nPlease input ca: ");
            strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, sptr, strlen(sptr));
            paho_network->tlsConnectParams.pRootCALocation = paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name;
            sptr = nwy_test_cli_input_gets("\r\nPlease input clientcert: ");
            strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.clientcert.cert_name, sptr, strlen(sptr));
            sptr = nwy_test_cli_input_gets("\r\nPlease input clientkey: ");
            strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.clientkey.cert_name, sptr, strlen(sptr));
            paho_network->tlsConnectParams.pDeviceCertLocation = paho_mqtt_at_param.paho_ssl_tcp_conf.clientcert.cert_name;
            paho_network->tlsConnectParams.pDevicePrivateKeyLocation = paho_mqtt_at_param.paho_ssl_tcp_conf.clientkey.cert_name;
        }
        paho_network->tlsConnectParams.ServerVerificationFlag = paho_mqtt_at_param.paho_ssl_tcp_conf.authmode;
        paho_network->is_SSL = 1;
        paho_network->tlsConnectParams.timeout_ms = 5000;
        sptr = nwy_test_cli_input_gets("\r\nPlease input sslversion: ");
        if (atoi(sptr) > 3 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput sslversion error");
            return;
        }
        paho_network->tlsConnectParams.sslversion = (sptr[0] - 0x30);
    }
    else
        nwy_test_cli_echo("\r\nis no-SSL NetworkConnect");
    sptr = nwy_test_cli_input_gets("\r\nPlease input clean_flag(0/1): ");
    if (atoi(sptr) > 1 || atoi(sptr) < 0)
    {
        nwy_test_cli_echo("\r\ninput clean_flag error");
        return;
    }
    paho_mqtt_at_param.cleanflag = (sptr[0] - 0x30);
    sptr = nwy_test_cli_input_gets("\r\nPlease input keep_alive: ");
    paho_mqtt_at_param.keepalive = atoi(sptr);
    nwy_test_cli_echo("\r\nip:%s, port :%d", paho_mqtt_at_param.host_name, paho_mqtt_at_param.port);
    rc = NetworkConnect(paho_network, paho_mqtt_at_param.host_name, paho_mqtt_at_param.port);
    if (rc < 0)
    {
        nwy_test_cli_echo("\r\nNetworkConnect err rc=%d", rc);
        return;
    }
    nwy_test_cli_echo("\r\nNetworkConnect ok");
    MQTTClientInit(&paho_mqtt_client, paho_network, 10000, g_nwy_paho_writebuf, PAHO_PLAYOAD_LEN_MAX,
                   g_nwy_paho_readbuf, PAHO_PLAYOAD_LEN_MAX);
    MQTTClientInit_defaultMessage(&paho_mqtt_client, messageArrived);
    data.clientID.cstring = paho_mqtt_at_param.clientID;
    if (0 != strlen((char *)paho_mqtt_at_param.username) && 0 != strlen((char *)paho_mqtt_at_param.password))
    {
        data.username.cstring = paho_mqtt_at_param.username;
        data.password.cstring = paho_mqtt_at_param.password;
    }
    data.keepAliveInterval = paho_mqtt_at_param.keepalive;
    data.cleansession = paho_mqtt_at_param.cleanflag;
    if (0 != strlen((char *)paho_mqtt_at_param.willtopic))
    {
        memset(&data.will, 0x0, sizeof(data.will));
        data.willFlag = 1;
        data.will.retained = paho_mqtt_at_param.willretained;
        data.will.qos = paho_mqtt_at_param.willqos;
        if (paho_mqtt_at_param.willmessage_len != 0)
        {
            data.will.topicName.lenstring.data = paho_mqtt_at_param.willtopic;
            data.will.topicName.lenstring.len = strlen((char *)paho_mqtt_at_param.willtopic);
            data.will.message.lenstring.data = paho_mqtt_at_param.willmessage;
            data.will.message.lenstring.len = paho_mqtt_at_param.willmessage_len;
        }
        else
        {
            data.will.topicName.cstring = paho_mqtt_at_param.willtopic;
            data.will.message.cstring = paho_mqtt_at_param.willmessage;
        }
        nwy_test_cli_echo("\r\nMQTT will ready");
    }
    nwy_test_cli_echo("\r\nConnecting MQTT");
    if ((rc = nwy_MQTTConnect(&paho_mqtt_client, &data)))
        nwy_test_cli_echo("\r\nFailed to create client, return code %d", rc);
    else
    {
        nwy_test_cli_echo("\r\nMQTT connect ok");
        nwy_osi_thread_t task_id = nwy_paho_yeild_task_init();
        if (task_id == NULL)
            nwy_test_cli_echo("\r\npaho yeid task create failed ");
        else
            nwy_test_cli_echo("\r\npaho yeid task create ok ");
    }
}

void nwy_test_cli_mqtt_pub()
{
    int rc;
    MQTTMessage pubmsg = {0};
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        //sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        sptr = "10_sec_info";
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        //sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        sptr = 0;
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        //sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        sptr = 1;
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput retained error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
        //sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 2048): ");
        sptr = "CAoQChgKIAooCjAKOAFACkgKUa5H4XoUriRAWApgAWgBcAF4AYABAYkBdk8eFmpNJUCSASQwYjkyN2Q5Ny03ODJhLTRjODItYjlkMi1lNGUwNjc3NGVkMzeaAQZFMUFCQ0ShAclw4eWO1ytAqQEDY9i8BWtTQLEBI046Nf+efUC4AeiDh8HaMQ==";
        //sptr = sprintf(info_String,"%.6f,%.6f", loc_info.latitude, loc_info.longitude);
        //nwy_test_cli_echo("\r\nMQTT string sent:%s", sptr);
        sptr = info_string;
        if (strlen(sptr) > NWY_EXT_SIO_RX_MAX)
        {
            nwy_test_cli_echo("\r\nNo more than 2048 bytes at a time ");
            return;
        }
#else
        //sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 512): ");
        //sptr = "CAoQChgKIAooCjAKOAFACkgKUa5H4XoUriRAWApgAWgBcAF4AYABAYkBdk8eFmpNJUCSASQwYjkyN2Q5Ny03ODJhLTRjODItYjlkMi1lNGUwNjc3NGVkMzeaAQZFMUFCQ0ShAclw4eWO1ytAqQEDY9i8BWtTQLEBI046Nf+efUC4AeiDh8HaMQ==";
        //sptr = sprintf(info_String,"%.6f,%.6f", loc_info.latitude, loc_info.longitude);
        //nwy_test_cli_echo("\r\nMQTT string sent:%s", sptr);
        sptr = info_string;
        if (strlen(sptr) > 512)
        {
            nwy_test_cli_echo("\r\nNo more than 512 bytes at a time ");
            return;
        }
#endif
        strncpy(paho_mqtt_at_param.message, sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, msg = %s",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          paho_mqtt_at_param.message);
        memset(&pubmsg, 0, sizeof(pubmsg));
        pubmsg.payload = (void *)paho_mqtt_at_param.message;
        pubmsg.payloadlen = strlen(paho_mqtt_at_param.message);
        pubmsg.qos = paho_mqtt_at_param.qos;
        pubmsg.retained = paho_mqtt_at_param.retained;
        pubmsg.dup = 0;
        rc = nwy_MQTTPublish(&paho_mqtt_client, paho_mqtt_at_param.topic, &pubmsg);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}

void nwy_test_cli_mqtt_sub()
{
    int rc;
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        rc = MQTTSubscribe(&paho_mqtt_client,
                           (char *)paho_mqtt_at_param.topic,
                           paho_mqtt_at_param.qos,
                           messageArrived);
        if (rc == SUCCESS)
            nwy_test_cli_echo("\r\nMQTT Sub ok");
        else
            nwy_test_cli_echo("\r\nMQTT Sub error:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

void nwy_test_cli_mqtt_unsub()
{
    int rc;
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        rc = MQTTUnsubscribe(&paho_mqtt_client, paho_mqtt_at_param.topic);
        if (rc == SUCCESS)
            nwy_test_cli_echo("\r\nMQTT Unsub ok");
        else
            nwy_test_cli_echo("\r\nMQTT Unsub error:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

//int test_connect = 0;
void nwy_test_cli_mqtt_state()
{
    if (MQTTIsConnected(&paho_mqtt_client))
        {
        nwy_test_cli_echo("\r\nMQTTconnect");
        test_connect = 1;
        }
    else
        {
        nwy_test_cli_echo("\r\nMQTT disconnect");
        test_connect = 0;
        }
}

void nwy_test_cli_mqtt_disconnect()
{
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        MQTTDisconnect(&paho_mqtt_client);
        NetworkDisconnect(paho_network);
        if (NULL != g_nwy_paho_writebuf)
        {
            free(g_nwy_paho_writebuf);
            g_nwy_paho_writebuf = NULL;
        }
        if (NULL != g_nwy_paho_readbuf)
        {
            free(g_nwy_paho_readbuf);
            g_nwy_paho_readbuf = NULL;
        }
        if (NULL != paho_network)
        {
            free(paho_network);
            paho_network = NULL;
        }
    }
    nwy_test_cli_echo("\r\nMQTT disconnect ok");
}
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
void nwy_test_cli_mqtt_pub_test(void)
{
    int rc, i;
    MQTTMessage pubmsg = {0};
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput retained error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
        sptr = nwy_test_cli_input_gets("\r\nPlease input 1k str,msg(10k) consists of 10 str");
        if (strlen(sptr) != 1024)
        {
            nwy_test_cli_echo("\r\nmust be 1k message");
            return;
        }
        for (i = 0; i < 10; i++)
            strncpy(paho_mqtt_at_param.message + i * strlen(sptr), sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, strlen is %d",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          strlen(paho_mqtt_at_param.message));
        memset(&pubmsg, 0, sizeof(pubmsg));
        pubmsg.payload = (void *)paho_mqtt_at_param.message;
        pubmsg.payloadlen = strlen(paho_mqtt_at_param.message);
        pubmsg.qos = paho_mqtt_at_param.qos;
        pubmsg.retained = paho_mqtt_at_param.retained;
        pubmsg.dup = 0;
        rc = nwy_MQTTPublish(&paho_mqtt_client, paho_mqtt_at_param.topic, &pubmsg);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}
#endif
#endif
#ifdef FEATURE_NWY_PAHO_MQTT_V5
MQTTClient paho_mqtt_client;
nwy_osi_thread_t nwy_paho_task_id;
#define NWY_EXT_SIO_PER_LEN 1024
char echo_buff[NWY_EXT_SIO_PER_LEN + 1] = {0};
nwy_paho_mqtt_at_param_type paho_mqtt_at_param = {0};
int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
    char topic_name[PAHO_TOPIC_LEN_MAX] = {0};
    int i = 0;
    unsigned int remain_len = 0;
    nwy_test_cli_echo("\r\n===messageArrived======");
    nwy_test_cli_echo("\r\npayloader len is %d", m->payloadlen);
    nwy_test_cli_echo("\r\ntopic name is %s:\r\n", topicName);
    nwy_test_cli_echo("message received is ");
    remain_len = m->payloadlen;
    if (m->payloadlen > NWY_EXT_SIO_PER_LEN)
    {
        for (i = 0; i < ((m->payloadlen / NWY_EXT_SIO_PER_LEN) + 1); i++)
        {
            memset(echo_buff, 0, sizeof(echo_buff));
            strncpy(echo_buff, m->payload + i * NWY_EXT_SIO_PER_LEN,
                    remain_len > NWY_EXT_SIO_PER_LEN ? NWY_EXT_SIO_PER_LEN : remain_len);
            remain_len = m->payloadlen - (i + 1) * NWY_EXT_SIO_PER_LEN;
            nwy_test_cli_echo(echo_buff);
        }
    }
    else
    {
        memset(echo_buff, 0, sizeof(echo_buff));
        strncpy(echo_buff, m->payload, m->payloadlen);
        nwy_test_cli_echo(echo_buff);
    }
    MQTTClient_free(topicName);
    MQTTClient_freeMessage(&m);
    return 1;
}


void nwy_paho_cycle(void *ctx)
{
    while (1)
    {
        while (MQTTClient_isConnected(paho_mqtt_client))
        {
            nwy_sleep(5000);
        }
        nwy_test_cli_echo("\r\nMQTT disconnect ,Out paho cycle");
        nwy_suspend_thread(nwy_paho_task_id);
    }
    nwy_sleep(200);
}


nwy_osi_thread_t nwy_paho_yeild_task_init(void)
{

    if ( nwy_paho_task_id == NULL)
    {
        nwy_create_thread(&nwy_paho_task_id, 1024 * 2,NWY_OSI_PRIORITY_NORMAL,"neo_paho_yeild_task", nwy_paho_cycle,
                                                     NULL, 4);
    }
    else
        nwy_resume_thread(nwy_paho_task_id);
    return nwy_paho_task_id;
}
void nwy_test_cli_mqtt_connect()
{
    int rc;
    size_t file_size;
    int authmode =0;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions sslopts = MQTTClient_SSLOptions_initializer;
    MQTTClient_init();
    char url[128] = {0};
    nwy_test_cli_echo("\r\nnwy_test_cli_mqtt_connect\r\n");
    if (MQTTClient_isConnected(paho_mqtt_client) == 1)
    {
        nwy_test_cli_echo("\r\npaho mqtt already connect");
        return;
    }
    memset(&paho_mqtt_at_param, 0, sizeof(nwy_paho_mqtt_at_param_type));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input url/ip: ");
    sptr = "mqtt.emotorad.com";    
    strncpy(paho_mqtt_at_param.host_name, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input port: ");
    sptr = "8883";
    paho_mqtt_at_param.port = atoi(sptr);
    //sptr = nwy_test_cli_input_gets("\r\nPlease input client_id: ");
    sptr = "test_lat_long";
    strncpy(paho_mqtt_at_param.clientID, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input usrname: ");
    sptr = "test";
    strncpy(paho_mqtt_at_param.username, sptr, strlen(sptr));
    //sptr = nwy_test_cli_input_gets("\r\nPlease input password: ");
    sptr = "3Motorad";
    strncpy(paho_mqtt_at_param.password, sptr, strlen(sptr));
    opts.MQTTVersion = MQTTVERSION_DEFAULT;
    //sptr = nwy_test_cli_input_gets("\r\nPlease input sslmode(1-ssl,0-no ssl): ");
    sptr = "1";
    if (atoi(sptr) > 1 || atoi(sptr) < 0)
    {
        nwy_test_cli_echo("\r\ninput sslmode error");
        return;
    }
    paho_mqtt_at_param.paho_ssl_tcp_conf.sslmode = atoi(sptr);
    if(paho_mqtt_at_param.paho_ssl_tcp_conf.sslmode == 0)
        sprintf(url, "%s:%d", paho_mqtt_at_param.host_name, paho_mqtt_at_param.port);
    else
    {
        sprintf(url, "ssl://%s:%d", paho_mqtt_at_param.host_name, paho_mqtt_at_param.port);
        //sptr = nwy_test_cli_input_gets("\r\nPlease input auth_mode(0/1/2): ");
        sptr = "0";
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput auth_mode error");
            return;
        }
        authmode = atoi(sptr);
        if (authmode == 0)
           sslopts.enableServerCertAuth = 0;
        else if (authmode == 1)
        {
           sslopts.enableServerCertAuth = 1;
           sptr = nwy_test_cli_input_gets("\r\nPlease input ca: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, sptr, strlen(sptr));
           sslopts.trustStore = paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name;
        }
        else
        {
           sslopts.enableServerCertAuth = 1;
           sptr = nwy_test_cli_input_gets("\r\nPlease input ca: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, sptr, strlen(sptr));
           if(nwy_mbedtls_load_file(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, 
                    &sslopts.trustStore, &file_size)< 0)
           {
                nwy_test_cli_echo("\r\nload ca error");
                return;
           }
           sptr = nwy_test_cli_input_gets("\r\nPlease input clientcert: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.clientcert.cert_name, sptr, strlen(sptr));
           if(nwy_mbedtls_load_file(paho_mqtt_at_param.paho_ssl_tcp_conf.clientcert.cert_name, 
                    &sslopts.keyStore, &file_size)< 0)
           {
                nwy_test_cli_echo("\r\niload keyStore error");
                return;
           }
           sptr = nwy_test_cli_input_gets("\r\nPlease input clientkey: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.clientkey.cert_name, sptr, strlen(sptr));
           if(nwy_mbedtls_load_file(paho_mqtt_at_param.paho_ssl_tcp_conf.clientkey.cert_name, 
                    &sslopts.privateKey, &file_size)< 0)
           {
                nwy_test_cli_echo("\r\niload privateKey error");
                return;
           }
        }
        //sptr = nwy_test_cli_input_gets("\r\nPlease input ssl version(0/1/2/3): ");
        sptr = "0";
        if (atoi(sptr) > 3 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput sslverison error");
            return;
        }
        sslopts.sslVersion = atoi(sptr);
        sslopts.verify = 1;
        opts.ssl = &sslopts;
    }
    //sptr = nwy_test_cli_input_gets("\r\nPlease input keep_alive: ");
    sptr = "1";
    opts.keepAliveInterval = atoi(sptr);
    //sptr = nwy_test_cli_input_gets("\r\nPlease input clean_flag(0/1): ");
    sptr = "0";
    if (atoi(sptr) > 1 || atoi(sptr) < 0)
    {
        nwy_test_cli_echo("\r\ninput clean_flag error");
        return;
    }
    opts.cleansession = atoi(sptr);
    opts.username = paho_mqtt_at_param.username;
    opts.password = paho_mqtt_at_param.password;
    opts.connectTimeout = 10000;
    opts.serverURIs = url;
    nwy_test_cli_echo("\r\nconnect url :%s\n", url);
    rc = MQTTClient_create(&paho_mqtt_client, url, paho_mqtt_at_param.clientID, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
    if(rc != MQTTCLIENT_SUCCESS)
    {
        nwy_test_cli_echo("create mqtt client rc : %d\n", rc);
        return ;
    }
    rc = MQTTClient_setCallbacks(paho_mqtt_client, NULL, NULL, messageArrived, NULL);
    if(rc != MQTTCLIENT_SUCCESS)
    {
        nwy_test_cli_echo("set callbacks rc : %d\n", rc);
        return ;
    }
    nwy_test_cli_echo("\r\nConnecting MQTT");
    rc = MQTTClient_connect(paho_mqtt_client, &opts);
    if (rc!=MQTTCLIENT_SUCCESS)
        nwy_test_cli_echo("\r\nFailed to connect client, return code %d", rc);
    else
    {
        nwy_test_cli_echo("\r\nMQTT connect ok");
        nwy_osi_thread_t task_id = nwy_paho_yeild_task_init();
        if (task_id == NULL)
            nwy_test_cli_echo("\r\npaho yeid task create failed ");
        else
            nwy_test_cli_echo("\r\npaho yeid task create ok ");
    }
}

void nwy_test_cli_mqtt_sub()
{
    int rc;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        rc = MQTTClient_subscribe(paho_mqtt_client, (char *)paho_mqtt_at_param.topic, paho_mqtt_at_param.qos);
        if (rc < 0)
            nwy_test_cli_echo("\r\nMQTT Sub error:%d", rc);
        else
            nwy_test_cli_echo("\r\nMQTT Sub ok");
    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

void nwy_test_cli_mqtt_pub()
{
    int rc;
    MQTTClient_deliveryToken last_token;
    MQTTProperties pub_props = MQTTProperties_initializer;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        //sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        sptr = "10_sec_info";
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        //sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        sptr = "0";
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        //sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        sptr = "1";
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
        //sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 2048): ");
        //sptr = "CAoQChgKIAooCjAKOAFACkgKUa5H4XoUriRAWApgAWgBcAF4AYABAYkBdk8eFmpNJUCSASQwYjkyN2Q5Ny03ODJhLTRjODItYjlkMi1lNGUwNjc3NGVkMzeaAQZFMUFCQ0ShAclw4eWO1ytAqQEDY9i8BWtTQLEBI046Nf+efUC4AeiDh8HaMQ==";
        //sptr = sprintf(info_String,"%.6f,%.6f", loc_info.latitude, loc_info.longitude);
        //nwy_test_cli_echo("\r\nMQTT string sent:%s", sptr);
        sptr = info_string;
        if (strlen(sptr) > NWY_EXT_SIO_RX_MAX)
        {
            nwy_test_cli_echo("\r\nNo more than 2048 bytes at a time ");
            return;
        }
#else
        //sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 512): ");
        //sptr = "CAoQChgKIAooCjAKOAFACkgKUa5H4XoUriRAWApgAWgBcAF4AYABAYkBdk8eFmpNJUCSASQwYjkyN2Q5Ny03ODJhLTRjODItYjlkMi1lNGUwNjc3NGVkMzeaAQZFMUFCQ0ShAclw4eWO1ytAqQEDY9i8BWtTQLEBI046Nf+efUC4AeiDh8HaMQ==";
        //sptr = sprintf(info_String,"%.6f,%.6f", loc_info.latitude, loc_info.longitude);
        //nwy_test_cli_echo("\r\nMQTT string sent:%s", sptr);
        sptr = info_string;
        if (strlen(sptr) > 512)
        {
            nwy_test_cli_echo("\r\nNo more than 512 bytes at a time ");
            return;
        }
#endif
        strncpy(paho_mqtt_at_param.message, sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, msg = %s",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          paho_mqtt_at_param.message);
        rc = MQTTClient_publish(paho_mqtt_client, paho_mqtt_at_param.topic, strlen(paho_mqtt_at_param.message),
                paho_mqtt_at_param.message, paho_mqtt_at_param.qos, paho_mqtt_at_param.retained, &last_token);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}

void nwy_test_cli_mqtt_unsub()
{
    int rc;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        rc = MQTTClient_unsubscribe(paho_mqtt_client, paho_mqtt_at_param.topic);
        if (rc < 0)
            nwy_test_cli_echo("\r\nMQTT unsub error:%d", rc);
        else
            nwy_test_cli_echo("\r\nMQTT unsub ok");


    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

//int test_connect = 0;
void nwy_test_cli_mqtt_state()
{
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
        {
        nwy_test_cli_echo("\r\nMQTTconnect");
        test_connect = 1;
        }
    else
        {
        nwy_test_cli_echo("\r\nMQTT disconnect");
        test_connect = 0;
        }
}

void nwy_test_cli_mqtt_disconnect()
{
    int rc;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        MQTTClient_disconnect(paho_mqtt_client, 0);
        MQTTClient_destroy(&paho_mqtt_client);
    }
    nwy_test_cli_echo("\r\nMQTT disconnect ok");
}
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
void nwy_test_cli_mqtt_pub_test(void)
{
    int rc, i;
    MQTTClient_deliveryToken last_token;
    MQTTProperties pub_props = MQTTProperties_initializer;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
        sptr = nwy_test_cli_input_gets("\r\nPlease input 1k str,msg(10k) consists of 10 str");
        if (strlen(sptr) != 1024)
        {
            nwy_test_cli_echo("\r\nmust be 1k message");
            return;
        }
        for (i = 0; i < 10; i++)
            strncpy(paho_mqtt_at_param.message + i * strlen(sptr), sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, strlen is %d",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          strlen(paho_mqtt_at_param.message));
        rc = MQTTClient_publish(paho_mqtt_client, paho_mqtt_at_param.topic, strlen(paho_mqtt_at_param.message),
                        paho_mqtt_at_param.message, paho_mqtt_at_param.qos, paho_mqtt_at_param.retained, &last_token);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}
#endif
#endif

/*AWS MQTT*/
#ifdef NWY_AWS_MQTT
#include "aws_iot_mqtt_client.h"
#include "aws_iot_log.h"
#include "aws_iot_mqtt_client_interface.h"


#define NWY_AWS_MQTT_MAX_CERT_SIZE  (4096)
#define NWY_AWS_MQTT_MAX_CERT_NAME_LEN (64)
#define NWY_AWS_MQTT_MAX_KEY_NAME_LEN (64)
#define NWY_EXT_SIO_RX_MAX          (2 * 1024)


#define NWY_CA_NAME "/nwy_cert/AmazonRootCA1.pem"
#define NWY_CLEINT_NAME "/nwy_cert/certificate.pem.crt"
#define NWY_CLIENT_PRIVATE_NAME "/nwy_cert/private.pem.key"


typedef struct{
    uint8_t ca_cert_name[NWY_AWS_MQTT_MAX_CERT_NAME_LEN + 32];
    uint8_t client_cert_name[NWY_AWS_MQTT_MAX_CERT_NAME_LEN + 32];
    uint8_t client_key_name[NWY_AWS_MQTT_MAX_KEY_NAME_LEN + 32];
    uint32_t ca_cert_len;
    uint32_t client_cert_len;
    uint32_t client_key_len;
}nwy_aws_mqtt_cert_t;


AWS_IoT_Client   g_aws_mqtt_Client = { 0 };
static nwy_osi_thread_t g_nwy_aws_mqtt_task_id = NULL;
static uint8_t                      g_nwy_mqtt_state = 0;

nwy_aws_mqtt_cert_t g_nwy_aws_mqtt_cert;

static nwy_aws_mqtt_menu()
{
    nwy_test_cli_echo("\r\nPlease select an option to data test from the items listed below. \r\n");
    nwy_test_cli_echo("1. aws_mqtt connect\r\n");
    nwy_test_cli_echo("2. aws_mqtt disconnect \r\n");
    nwy_test_cli_echo("3. aws_mqtt publlish\r\n");
    nwy_test_cli_echo("4. aws_mqtt subscribe\r\n");
    nwy_test_cli_echo("5. Exit tcpfunction\r\n");
}

static void nwy_aws_mqtt_disconnect_cb(AWS_IoT_Client *pClient, void *data)
{
    IOT_WARN("MQTT Disconnect");
}

static void nwy_aws_mqtt_cycle(void)
{
    OSI_LOGI(0,"AWS nwy_awsmqtt_cycle");

    for (;;)
    {
        IoT_Error_t rc        = SUCCESS;
        while (g_nwy_mqtt_state && ((rc >= 0) || (rc == MQTT_CLIENT_NOT_IDLE_ERROR)))
        {
            rc = aws_iot_mqtt_yield(&g_aws_mqtt_Client, 100);
            if(NETWORK_ATTEMPTING_RECONNECT == rc) {
               // If the client is attempting to reconnect we will skip the rest of the loop.
                   continue;
              }
            nwy_sleep(300);
        }
        OSI_LOGI(0, "Out AWS cycle");
        g_nwy_mqtt_state = 0;
        rc = 0;
        nwy_suspend_thread(g_nwy_aws_mqtt_task_id);
    }

}

static nwy_osi_thread_t nwy_aws_mqtt_yeild_task(void)
{
  if (g_nwy_aws_mqtt_task_id == NULL)
  {
    nwy_create_thread (&g_nwy_aws_mqtt_task_id, 1024*15,NWY_OSI_PRIORITY_NORMAL,"nwy_aws_yeild_task",nwy_aws_mqtt_cycle,
                   NULL, 4);
  }
  else
  {
    nwy_resume_thread(g_nwy_aws_mqtt_task_id);
  }

  return g_nwy_aws_mqtt_task_id;
}

void nwy_aws_mqtt_connect()
{
    IoT_Error_t ret  = FAILURE;
    char url[128] = {0};
    int autoconnect = 0;
    char* sptr = NULL;
    IoT_Client_Init_Params     nwy_mqttinit_params = IoT_Client_Init_Params_initializer;
    IoT_Client_Connect_Params  nwy_connect_params  = IoT_Client_Connect_Params_initializer;

    ret = aws_iot_mqtt_is_client_connected(&g_aws_mqtt_Client);
    if(ret == 1)
    {
        nwy_test_cli_echo("\r\n aws_mqtt has already connect");
        return;
    }

    nwy_mqttinit_params.pRootCALocation= NWY_CA_NAME;
    nwy_mqttinit_params.pDeviceCertLocation = NWY_CLEINT_NAME;
    nwy_mqttinit_params.pDevicePrivateKeyLocation = NWY_CLIENT_PRIVATE_NAME;


    sptr = nwy_test_cli_input_gets("\r\nPlease host url: ");
    memcpy(url, sptr, strlen(sptr));


    sptr = nwy_test_cli_input_gets("\r\nPlease url port: ");

    nwy_mqttinit_params.pHostURL = url;
    nwy_mqttinit_params.port = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease auto connect(0-1): ");
    autoconnect = atoi(sptr);
    nwy_mqttinit_params.enableAutoReconnect = autoconnect;


    nwy_mqttinit_params.mqttPacketTimeout_ms = 5000;
    nwy_mqttinit_params.mqttCommandTimeout_ms = 20000;
    nwy_mqttinit_params.tlsHandshakeTimeout_ms = 20000;

    nwy_mqttinit_params.disconnectHandler = nwy_aws_mqtt_disconnect_cb;
    nwy_mqttinit_params.disconnectHandlerData = NULL;
    nwy_mqttinit_params.isSSLHostnameVerify = 1;
    nwy_aws_log_print("aws aws_iot_mqtt_init isSSLHostnameVerify = %d", nwy_mqttinit_params.isSSLHostnameVerify);
    ret = aws_iot_mqtt_init(&g_aws_mqtt_Client, &nwy_mqttinit_params);
    if (SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\n aws_mqtt init fail");
        return;
    }

    nwy_connect_params.keepAliveIntervalInSec = 60;
    nwy_connect_params.isCleanSession = 1;
    nwy_connect_params.MQTTVersion = 4;
    nwy_connect_params.isWillMsgPresent = false;

    OSI_LOGI(0, "aws aws_iot_mqtt_connect");

    ret = aws_iot_mqtt_connect(&g_aws_mqtt_Client, &nwy_connect_params);
    if (SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\n aws_mqtt connect fail");
        return;
    }
    else
    {

         if(true == autoconnect)
         {
           ret = aws_iot_mqtt_autoreconnect_set_status(&g_aws_mqtt_Client, true);
           if(SUCCESS != ret)
           {
               nwy_test_cli_echo("Unable to set Auto Reconnect to true - %d", ret);
               return;
           }
         }

         OSI_LOGI(0, "aws nwy_awsmqtt_send_recv_loop");
         g_nwy_mqtt_state = 1;
         nwy_aws_mqtt_yeild_task();
         nwy_test_cli_echo("\r\n aws mqtt connect success");
    }
}
int nwy_aws_mqtt_disconnect(void)
{
    IoT_Error_t rc  = FAILURE;
    bool ret = true;
    ret = aws_iot_mqtt_is_client_connected(&g_aws_mqtt_Client);
    if(!ret)
    {
        return ret;
    }
    rc = aws_iot_mqtt_autoreconnect_set_status(&g_aws_mqtt_Client, false);
    if(SUCCESS != rc)
    {
      IOT_ERROR("Unable to set Auto Reconnect to false - %d", rc);
      return rc;
    }
    IOT_ERROR("AWS AA100");
    rc = aws_iot_mqtt_disconnect(&g_aws_mqtt_Client);
    if (SUCCESS != rc)
    {
       g_aws_mqtt_Client.clientStatus.clientState = CLIENT_STATE_DISCONNECTED_ERROR;
       g_aws_mqtt_Client.networkStack.disconnect(&(g_aws_mqtt_Client.networkStack));
       g_aws_mqtt_Client.networkStack.destroy(&(g_aws_mqtt_Client.networkStack));
       IOT_ERROR("error aws_iot_mqtt_disconnect rc:%d", rc);

       return rc;
    }
    if(NULL != g_aws_mqtt_Client.clientData.disconnectHandler)
    {
       g_aws_mqtt_Client.clientData.disconnectHandler(&g_aws_mqtt_Client,
                         g_aws_mqtt_Client.clientData.disconnectHandlerData);
    }

    g_nwy_mqtt_state = 0;
    if (rc != SUCCESS) {
        nwy_test_cli_echo("\r\n aws mqtt disconnect error %d", rc);
    } else {
        nwy_test_cli_echo("\r\n aws mqtt disconnect success");
    }
    return rc;
}

int nwy_aws_mqtt_publish(void)
{
    char* sptr = NULL;
    char topicname[32] = {0};
    IoT_Error_t rc = FAILURE;
    IoT_Publish_Message_Params paramsQOS0;

    memset(&paramsQOS0, 0, sizeof(paramsQOS0));


    sptr = nwy_test_cli_input_gets("\r\nPlease input topicname: ");
    memcpy(topicname, sptr, strlen(sptr));


    sptr = nwy_test_cli_input_gets("\r\nPlease input payload len: ");
    paramsQOS0.payloadLen = atoi(sptr);

    paramsQOS0.payload = (char *)malloc(paramsQOS0.payloadLen + 1);
    memset(paramsQOS0.payload, 0, (paramsQOS0.payloadLen + 1));

    sptr = nwy_test_cli_input_gets("\r\nPlease input payload info: ");
    memcpy(paramsQOS0.payload, sptr, (paramsQOS0.payloadLen));

    paramsQOS0.qos = QOS0;
    paramsQOS0.isRetained = 0;

    rc = aws_iot_mqtt_publish(&g_aws_mqtt_Client, topicname, strlen(topicname), &paramsQOS0);
    if (rc != SUCCESS) {
        nwy_test_cli_echo("\r\n aws mqtt publish error %d", rc);
    } else {
        nwy_test_cli_echo("\r\n aws mqtt publish success");
    }
    return rc;
}

void nwy_aws_mqtt_subscribe_cb(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData) {
    nwy_test_cli_echo("\r\nSubscribe callback");
    nwy_test_cli_echo("\r\n%.*s\t%.*s", topicNameLen, topicName, (int) params->payloadLen, (char *) params->payload);
}

int nwy_aws_mqtt_subscribe(void)
{
    char* sptr = NULL;
    static char topicname[32] = {0};
    IoT_Error_t rc = FAILURE;

    sptr = nwy_test_cli_input_gets("\r\nPlease input topicname: ");
    memcpy(topicname, sptr, strlen(sptr));

    rc = aws_iot_mqtt_subscribe(&g_aws_mqtt_Client, topicname, strlen(topicname), QOS0, nwy_aws_mqtt_subscribe_cb, NULL);
    if (rc != SUCCESS) {
        nwy_test_cli_echo("\r\n aws mqtt subscribe error %d", rc);
    } else {
        nwy_test_cli_echo("\r\n aws mqtt subscribe success");
    }
    return rc;
}

int nwy_aws_mqtt_unsubcribe(void)
{
    char* sptr = NULL;
    static char topicname[32] = {0};
    IoT_Error_t rc = FAILURE;

    sptr = nwy_test_cli_input_gets("\r\nPlease input topicname: ");
    memcpy(topicname, sptr, strlen(sptr));

    rc = aws_iot_mqtt_unsubscribe(&g_aws_mqtt_Client, topicname, strlen(topicname));
    if (rc != SUCCESS) {
        nwy_test_cli_echo("\r\n aws mqtt unsubscribe error %d", rc);
    } else {
        nwy_test_cli_echo("\r\n aws mqtt unsubscribe success");
    }
    return rc;
}

int nwy_aws_mqtt_status_get(void)
{
    int status = aws_iot_mqtt_is_client_connected(&g_aws_mqtt_Client);
    if(status == true)
        nwy_test_cli_echo("\r\n aws mqtt is connected");
    else
        nwy_test_cli_echo("\r\n aws mqtt is disconnected");
    return status;
}
int nwy_aws_mqtt_test()
{
    char* sptr = NULL;
    while (1)
    {
        nwy_aws_mqtt_menu();
        sptr = nwy_test_cli_input_gets("\r\nPlease input option: ");
        switch(atoi(sptr)) {
            case 1:
                nwy_aws_mqtt_connect();
                break;
            case 2:
                nwy_aws_mqtt_disconnect();
                break;
            case 3:
                nwy_aws_mqtt_publish();
                break;
            case 4:
                nwy_aws_mqtt_subscribe();
                break;
            case 5:
                return;
        }
    }
}
#endif
#ifdef FEATURE_NEOWAY_PIPECLOUD
void nwy_pipecloud_set_info(void)
{
    char* sptr = NULL;
    nwy_pipe_server_para_st server;
    memset(&server, 0, sizeof(server));
    sptr = nwy_test_cli_input_gets("\r\n input server url(len <= 64)");
    memcpy(server.server_url, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\n input server port(1-65535)");
    server.server_port = atoi(sptr);
    if(nwy_pipecloude_svr_cfg_set(&server) == NWY_SUCESS)
        nwy_test_cli_echo("nwy_pipecloude_svr_cfg_set success \r\n");
    else
        nwy_test_cli_echo("nwy_pipecloude_svr_cfg_set failed \r\n");

}
int nwy_pipecloud_start(void)
{
    nwy_pipe_switch_st info;
    char* sptr = NULL;
    memset(&info, 0, sizeof(info));
    sptr = nwy_test_cli_input_gets("\r\n input action(1-start, 0-stop)");
    if(atoi(sptr) == 1)
        info.status = NWY_PIPE_SWITCH_ENABLE;
    else
        info.status = NWY_PIPE_SWITCH_DISABLE;
    info.type = NWY_PIPE_SERVICE_TYPE_NEOWAY;
    if(nwy_pipecloude_switch_set(&info)== NWY_SUCESS)
        nwy_test_cli_echo("nwy_pipecloude_switch_set success \r\n");
    else
        nwy_test_cli_echo("nwy_pipecloude_switch_set failed \r\n");
}
int nwy_pipecloud_get_status(void)
{
    nwy_pipe_switch_st info;
    char* sptr = NULL;
    memset(&info, 0, sizeof(info));

    if(nwy_pipecloude_switch_get(&info)== NWY_SUCESS)
        nwy_test_cli_echo("nwy_pipecloude_switch_get success , status %d \r\n", info.status);
    else
        nwy_test_cli_echo("nwy_pipecloude_switch_get failed \r\n");
}
#endif

