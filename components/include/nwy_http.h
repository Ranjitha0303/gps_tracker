#ifndef __NWY_HTTP_H__
#define __NWY_HTTP_H__

#include "nwy_common.h"
#ifdef __cplusplus
extern "C" {
#endif

#define NWY_HTTP_URL_MAX_LEN   512          //url max len
#define NWY_HTTP_POST_MAX_DATA_LEN  4096    //post body max len
#define NWY_PING_REPLY_IP_ADDR_LEN  64
#define NWY_PING_HOST_MAX_LEN      512
#define NWY_MAX_HOST_NAME_NAME 100

typedef enum
{
    NWY_HTTP_DNS_ERR,       //Dns error
    NWY_HTTP_OPEN_FAIL,     //open fail
    NWY_HTTP_OPENED,        //setup success
    NWY_HTTPS_SSL_CONNECTED, //https setup success
    NWY_HTTP_CLOSED_PASV,   //server closed
    NWY_HTTP_CLOSED,        //client closed
    NWY_HTTP_DATA_RECVED,   //recv data
    NWY_HTTP_DATA_SEND_ERR, //data send error
    NWY_HTTPS_SSL_AUTH_FAIL,    //https auth fail
    NWY_HTTPS_SSL_INIT_ERROR,   //https ssl init fail
    NWY_HTTPS_SSL_HANDSHAKE_ERROR,//https ssl handshake fail
    NWY_HTTP_DATA_SEND_FINISHED, //data send finished
    NWY_HTTP_EVENT_UNKOWN,  //unkown error
}nwy_http_event_e;


typedef struct nwy_http_result
{
  uint32 event;                     /* event id */
  uint32 data_len;                  /* data length */
  byte* data;                 /* data */
}nwy_http_result_t;


typedef enum
{
  NWY_VERSION_SSL_V3_E,         //SSL3.0
  NWY_VERSION_TLS_V1_0_E,       //SSL1.0
  NWY_VERSION_TLS_V1_1_E,       //SSL1.1
  NWY_VERSION_TLS_V1_2_E,       //SSL1.2
  NWY_VERSION_MAX_E
}nwy_ssl_version_type_e;

typedef enum
{
  NWY_SSL_AUTH_NONE_E = 0,      //No authentication
  NWY_SSL_AUTH_ONE_WAY_E,       //Manage server authentication
  NWY_SSL_AUTH_MUTUAL_E,        //Manage server and client authentication if requested by the remote server
  NWY_SSL_AUTH_MAX_E
}nwy_ssl_auth_mode_e;

typedef struct
{
  char cert_name[51];
  uint32 *cert_data;
}nwy_ssl_cert_t;



typedef struct
{
  nwy_ssl_version_type_e ssl_version;
  nwy_ssl_auth_mode_e authmode;
  nwy_ssl_cert_t cacert;
  nwy_ssl_cert_t clientcert;
  nwy_ssl_cert_t clientkey;
  byte hostname[100];
  uint8 ciphersuites_num;
  int* ciphersuites_list;
}nwy_app_ssl_conf_t;

//ntp
typedef enum
{
  NWY_UPDATETIME_SDK_SUCCESS,
  NWY_UPDATETIME_IP_SDK_ERR,
  NWY_UPDATETIME_SDK_UPDATING,
  NWY_UPDATETIME_SEND_SDK_ERR,
  NWY_UPDATETIME_SELECT_SDK_ERR,
  NWY_UPDATETIME_RECV_SDK_TIMEOUT,
  NWY_UPDATETIME_RECV_SDK_ERR,
}nwy_updatetime_errid;
typedef struct
{
  char remote_addr[NWY_MAX_HOST_NAME_NAME + 1];
  int payload;
  int count;
  int timeout;
  int ip_type;  //4-ipv4,6-ipv6
  int interval;
  int debug_switch;
}nwy_ping_param_type;

typedef enum
{
    NWY_PING_ST_RUN_START,
    NWY_PING_ST_DNS_ERR,
    NWY_PING_ST_RUNNING,
    NWY_PING_ST_REPLY_RECV,
    NWY_PING_ST_ALL_FINISH,
    NWY_PING_ST_REPLY_TIMEOUT,
    NWY_PING_ST_SOCKET_ERR,
    NWY_PING_ST_SEND_ERR,
    NWY_PING_ST_OTHER_ERR
}nwy_ping_st_e_type;

typedef struct
{
    uint32_t  min_rtt;  /* Minimum RTT so far, in millisecs           */
    uint32_t  max_rtt;  /* Maximum RTT so far, in millisecs           */
    uint32_t  avg_rtt;  /* Average RTT so far, in millisecs           */
    uint32_t  num_pkts_sent;  /* Number of pings sent so far          */
    uint32_t  num_pkts_recvd; /* Number of responses recieved so far  */
    uint32_t  num_pkts_lost;  /* Number of responses not received     */
}nwy_ping_fin_stats_type;

typedef struct
{
    nwy_ping_st_e_type status;
    char reply_addr[NWY_PING_REPLY_IP_ADDR_LEN+1];
    nwy_ping_fin_stats_type fin_stats_info;   //ping statistics info, only for PING_ST_ALL_FINISH
}nwy_ping_cb_msg_t;
typedef void (*nwy_ping_cb_t)(void* arg);

typedef void(*nwy_updatetime_cb)(void* handle, nwy_updatetime_errid err_code, nwy_time_t *time);

typedef void (*httpresultcb)(nwy_http_result_t *event);  //notify events

/*
@func
    nwy_cert_add
@desc
    add cert
@param
    const char *file_name   cert filename
    const char *data        cert data
    int len                 cert data length
@return
    0: on success;
    -1: on failed
@other
   The system directiry where the certificate is located is /nwy/
*/
int nwy_cert_add(const char *file_name, const char *data, int length);

/*
@func
    nwy_cert_check
@desc
    check cert
@param
    const char *file_name   cert filename
@return
    0: on success
    -1: on failed
@other
     NA
*/
int nwy_cert_check(const char *file_name);

/*
@func
    nwy_cert_del
@desc
    delete cert
@param
    const char *file_name   cert filename
@return
    0: on success
    -1: on failed
@other
    NA
*/
int nwy_cert_del(const char *file_name);

/*
@func
    nwy_http_setup
@desc
    http setup
@param
    uint16_t channel: multiplex index (1-7)
    const char *url     ip or domain
    int port            port
    httpresultch  cb    Callback notification function
@return
    0: on success;
    -1: on failed
@other
  Once ftp login is completed successfully, the callback function cb will be triggered.
  NWY_HTTP_OPENED event will report
*/
int nwy_http_setup(uint16_t channel, const char *url, int port, httpresultcb cb);

/*
@func
    nwy_https_setup
@desc
    http setup
@param
    uint16_t channel: multiplex index (1-7)
    const char *url     ip or domain
    int port            port
    httpresultch  cb    Callback notification function
    nwy_app_ssl_conf_t *ssl_cfg
@return
    0: on success;
    -1: on failed
@other
  Once ftp login is completed successfully, the callback function cb will be triggered.
  NWY_HTTPS_SSL_CONNECTED event will report
  The certificate is added by nwy_cert_add
*/
int nwy_https_setup(uint16_t channel, const char *url, int port, httpresultcb cb, nwy_app_ssl_conf_t *ssl_cfg);


/*
@func
    nwy_http_get
@desc
    HTTP get action
@param
    uint16_t channel: multiplex index (1-7)
    uint8_t keeplive whetier to set up a long connection
    value            0 is not
                     1 is a long connection
    int offset       get data offset
    int size         get data size
    boolean is_https http is false, https is true
@return
    0: on success;
    -1: on failed
@other
  Once get action success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_DATA_RECVED will report
*/
int nwy_http_get(uint16_t channel, uint8_t keepalive, int offset, int size, boolean is_https);

/*
@func
    nwy_http_head
@desc
    HTTP head action
@param
    uint16_t channel: multiplex index (1-7)
    uint8_t keeplive whetier to set up a long connection
    value            0 is not
                     1 is a long connection
    boolean is_https http is false, https is true
@return
    0: on success;
    -1: on failed
@other
  Once head action success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_DATA_RECVED will report
*/
int nwy_http_head(uint16_t channel, uint8_t keepalive, boolean is_https);

/*
@func
    nwy_http_post
@desc
    HTTP post action
@param
    uint16_t channel: multiplex index (1-7)
    uint8_t keeplive whetier to set up a long connection
    value            0 is not
                     1 is a long connection
    uint8_t type     request message data type
    value            0:x-www-form-urlencoded
                     1:text
                     2:json
                     3:xml
                     4:html
                     99:User-defined mode
    const char *data User data
    int len          User data length
    boolean is_https http is false, https is true
@return
    0: on success;
    -1: on failed
@other
  Once post action success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_DATA_RECVED will report
*/
int nwy_http_post(uint16_t channel, uint8_t keepalive, uint8_t type, const char* data, int len, boolean is_https);
/*
@func
    nwy_http_put
@desc
    HTTP post action
@param
    uint16_t channel: multiplex index (1-7)
    uint8_t keeplive whetier to set up a long connection
    value            0 is not
                     1 is a long connection
    uint8_t type     request message data type
    value            0:x-www-form-urlencoded
                     1:text
                     2:json
                     3:xml
                     4:html
                     99:User-defined mode
    const char *data User data
    int len          User data length
    boolean is_https http is false, https is true
@return
    0: on success;
    -1: on failed
@other
  Once put action success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_DATA_RECVED will report
*/
int nwy_http_put(uint16_t channel, uint8_t keepalive, uint8_t type, const char* data, int len, boolean is_https);


/*
@func
    nwy_http_close
@desc
    User can send custom message mode
@param
    uint16_t channel: multiplex index (1-7)
    boolean is_https http is false, https is true
@return
    0: on success;
    -1: on failed
@other
  Once close success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_CLOSED will report
*/
int nwy_http_close(uint16_t channel, boolean is_https);

/*
@func
    nwy_updatetime_ntp
@param
    char* url:host name
    unsigned long timeout:time out (1-30 s)
    char* tz: time zone (E[0-13],W[0-12])
    unsigned char dst: daylight_saving_time (0-1)
    nwy_update_time_cb cb_unc: result , format   void(*nwy_update_time_cb)(void* handle, nwy_updatetime_errid err_code, nwy_time_t *time);
@return
    void
@other
*/
int nwy_updatetime_ntp(char* url, unsigned long timeout, char* tz, unsigned char dst, nwy_updatetime_cb cb_unc);


/* setup timeout  timeout report NWY_HTTP_OPEN_FAIL*/
void nwy_http_set_tcp_connect_timeout(uint32 timeout);

/* if recv NWY_HTTP_OPEN_FAIL EVENT api return 1 is timeout  */
int nwy_http_get_open_fail_if_tout();
/*
@func
    nwy_ping_start
@desc
    User can send ping url
@param
    nwy_ping_param_t ping_param: contain url,payload,count etc.
    nwy_ping_cb nwy_ping_cb:http result ind
@return
    0: on success;
    -1: on failed
@other

*/

int nwy_ping_start(nwy_ping_param_type *ping_param, nwy_ping_cb_t nwy_ping_cb);


#ifdef __cplusplus
}
#endif
#endif //__NWY_HTTP_H__ endif
