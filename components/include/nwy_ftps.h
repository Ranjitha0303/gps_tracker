#ifndef __NWY_FTPS_H__
#define __NWY_FTPS_H__

#include "nwy_common.h"

#define FTPS_STRING_SIZE 256
#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    NWY_FTPS_EVENT_DNS_ERR,
    NWY_FTPS_EVENT_OPEN_FAIL,
    NWY_FTPS_EVENT_CLOSED,
    NWY_FTPS_EVENT_LOGOUT,
    NWY_FTPS_EVENT_DATA_RECVED,
    NWY_FTPS_EVENT_DATA_GET,  
    NWY_FTPS_EVENT_DATA_OPEND,
    NWY_FTPS_EVENT_DATA_CLOSED,
    NWY_FTPS_EVENT_SSL_INIT_ERR,
    NWY_FTPS_EVENT_SSL_HANDSHAKE_ERR,
    NWY_FTPS_EVENT_SSL_CONNECTED,
    NWY_FTPS_EVENT_SSL_AUTH_FAIL,
    NWY_FTPS_EVENT_DATA_SETUP_ERR,
    NWY_FTPS_EVENT_LOGIN,
    NWY_FTPS_EVENT_SIZE,
    NWY_FTPS_EVENT_SIZE_ZERO,
    NWY_FTPS_EVENT_PASS_ERR,
    NWY_FTPS_EVENT_FILE_NOT_FOUND,
    NWY_FTPS_EVENT_FILE_SIZE_ERR,
    NWY_FTPS_EVENT_FILE_DELE_SUCCESS,
    NWY_FTPS_EVENT_DATA_RECV_ERR,
    NWY_FTPS_EVENT_DATA_GET_ERR,
    NWY_FTPS_EVENT_DATA_PUT_FINISHED,
    NWY_FTPS_EVENT_ACTION_TOUT,             /* unused */
    NWY_FTPS_EVENT_RENAME_OK,               /* unused */
    NWY_FTPS_EVENT_RENAME_ERR,              /* unused */
    NWY_FTPS_EVENT_MKDIR_OK,                /* unused */
    NWY_FTPS_EVENT_MKDIR_ERR,               /* unused */
    NWY_FTPS_EVENT_RMD_OK,                  /* unused */
    NWY_FTPS_EVENT_RMD_ERR,                 /* unused */
    NWY_FTPS_EVENT_CLOSED_SSL_FAIL,
    NWY_FTPS_EVENT_UNKOWN,
}nwy_ftps_event_e;

typedef struct nwy_ftps_login_para_t
{
    uint16_t channel;                   /* channle (1-7) */
    uint8_t host[FTPS_STRING_SIZE];      /* remote server hostname */
    uint16_t port;                      /* remote server port */
    uint8_t username[FTPS_STRING_SIZE];  /* login username */
    uint8_t passwd[FTPS_STRING_SIZE];    /* login passwd */
    uint8_t mode;                       /* 1 is active 0 is passv */
    uint16 timeout;                     /* timeout */
}nwy_ftps_login_para_t;

typedef struct nwy_ftps_result
{
  uint32 event;                     /* event id */
  uint32 data_len;                  /* data length */
  const byte* data;                 /* data */
}nwy_ftps_result_t;

typedef struct nwy_ftps_get_param_t
{
    uint16_t channel;
    char* filename;
    uint8_t type;
    int offset;
    int len;
    int tout;
}nwy_ftps_get_param_t;

typedef struct nwy_ftps_put_param_t
{
    uint16_t channel;
    char* filename;
    int type;
    int mode;
    uint8_t* data_ptr;
    int len;
    bool eof;
    uint16_t tout;
}nwy_ftps_put_param_t;

typedef enum
{
  NWY_VERSION_FTP_SSL_V3,
  NWY_VERSION_FTP_TLS_V1_0,
  NWY_VERSION_FTP_TLS_V1_1,
  NWY_VERSION_FTP_TLS_V1_2,
  NWY_VERSION_FTP_MAX
}nwy_ftps_version_t;

typedef enum
{
  NWY_FTP_SSL_AUTH_NONE = 0,
  NWY_FTP_SSL_AUTH_ONE_WAY,
  NWY_FTP_SSL_AUTH_MUTUAL,
  NWY_FTP_SSL_AUTH_MAX
}nwy_ftp_ssl_auth_mode;

typedef struct
{
  char cert_name[51];
  char *cert_data;
}nwy_ftp_ssl_cert_type;


typedef struct
{
  nwy_ftps_version_t ssl_version;
  nwy_ftp_ssl_auth_mode authmode;
  nwy_ftp_ssl_cert_type cacert;
  nwy_ftp_ssl_cert_type clientcert;
  nwy_ftp_ssl_cert_type clientkey;
  byte hostname[100];
  uint8 ciphersuites_num;
  int* ciphersuites_list;
  uint8 setflag;
}nwy_ftp_ssl_conf_t;


typedef struct
{
  uint8_t ftps_type;
  nwy_ftp_ssl_conf_t ssl_conf;
}nwy_ftp_ssl_ext_paras_t;

typedef void (*nwy_ftps_cb)(uint16_t cid, uint16_t sid, int16_t status, const void* msg_ptr);  //notify events

typedef void (*result_cb)(nwy_ftps_result_t *event);  //notify events


/*
@func
    nwy_ftps_login
@desc
    ftps login
@param
    ftps_param [in]: Login parameter struct
    ftps_ext_param [in]: ssl configs and type
    returncb cb [in]: Callback notification function
@return
    0: on success
    -1: on failed
@other
  Once ftp login is completed successfully, the callback function cb will be triggered.
  NWY_FTPS_EVENT_DATA_OPEND event will report
*/

int nwy_ftps_login(nwy_ftps_login_para_t *ftps_param, nwy_ftp_ssl_ext_paras_t *ftps_ext_param, result_cb cb);

/*
@func
    nwy_ftps_get
@desc
    get file from remote server use ftp ssl
@param
    channel [in]: channel (1-7)
    filename [in]: The file name to get
    type [in]: File transfer mode 1-ASCII 2-Binary
    offset [in]: the offset of the file content to be fetched
    len [in]: the len of the file content to be fetched
    ftps_ext_param [in]: ssl configs and type
@return
     0: on success;
    -1: on failed
@other
  Once action is completed successfully, the callback function cb will be triggered. some event will be report
  NWY_FTPS_EVENT_DATA_RECVED :recv data
  NWY_FTPS_EVENT_DATA_GET :get file
  NWY_FTSP_EVENT_DATA_CLOSED : data path disconnected
*/

int nwy_ftps_get(uint16_t channel, char* filename, uint8_t type, int offset, int len,  int tout, nwy_ftp_ssl_ext_paras_t *ftps_ext_param);

/*
@func
    nwy_ftps_put
@desc
    put file to remote server use ftp ssl
@param
    channel,[in]: channel (1-7)
    filename [in]: The file name to get
    type [in]: File transfer mode 1-ASCII 2-Binary
    mode [in]: Operating mode
           1: STOR mode Create a file on the server to write data, if the file exists, overwrite the source file
           2: APPE mode Create a file on the server to write data, if the file exists, append to the end of the file
           3: DELE delete a file
    data_ptr [in]:Data content to be uploaded
    len [in]: the length of the data
    eof [out]: whether is the end of files or not
    tout [in]: longest time to report put fail
    ftps_ext_param [in]: ssl configs and type
@return
    0: on success;
    -1: on failed
@other
  Once action is completed successfully, the callback function cb will be triggered. some event will be report
  NWY_FTPS_EVENT_DATA_PUT_FINISHED: the data have be uploaded success.

*/

int nwy_ftps_put(nwy_ftps_put_param_t *ftps_put_para, nwy_ftp_ssl_ext_paras_t *ftps_ext_param);

/*
@func
    nwy_ftps_filesize
@desc
    get file size use ftp ssl
@param
    channel,[in]: channel (1-7)
    filename [in]: The file name to getsize
    tout [in]: timeout
    ftps_ext_param [in]: ssl configs and type
@return
    0: on success;
    -1: on failed
@other
  Once action is completed successfully, the callback function cb will be triggered. some event will be report
  NWY_FTP_EVENT_SIZE notify the file length

*/

int nwy_ftps_filesize(uint16_t channel,const char* filename, uint16_t tout, nwy_ftp_ssl_ext_paras_t *ftps_ext_param);

/*
@func
    nwy_ftps_logout
@desc
    ftp logout use ftp ssl
@param
    channel [in]: channel (1-7)
    tout [in]: timeout
    ftps_ext_param [in]: ssl configs and type
@return
    0: on success;
    -1: on failed
@other
  Once action is completed successfully, the callback function cb will be triggered. some event will be report
  NWY_FTP_EVENT_LOGOUT notify success logout

*/

int nwy_ftps_logout(uint16_t channel, uint16_t tout, nwy_ftp_ssl_ext_paras_t *ftps_ext_param);


#ifdef __cplusplus
}
#endif
#endif //__NWY_FTPS_H__ endif


