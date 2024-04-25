#ifndef __NWY_VIR_AT_H__
#define __NWY_VIR_AT_H__

#include "nwy_common.h"

#ifdef __cplusplus
extern "C" {
#endif
#define NWY_AT_TIMEOUT_MAX   (30)  //30S
#define NWY_AT_TIMEOUT_MIN   (5)  //5S
#define NWY_AT_TIMEOUT_DEFAULT   (30)  //5S
#define NWY_AT_UNISOLICITE_REG_MAX (32)
#define NWY_ATFORWARD_MAX  50

/*********************************************************************
handle: uart or usb, or vir at handle
type:   0: AT_CMD_SET, 1: AT_CMD_TEST, 2: AT_CMD_READ; 3: AT_CMD_EXE;
**********************************************************************/
typedef void (*nwy_at_forward_process_cb)(void* handle, char* atcmd,int type, char* para0, char* para1, char* para2);
typedef int (*nwy_at_unsolicite_handle)(uint8 *data_str, int len);
typedef void (*nwy_get_original_at_rsp)(char *rsp, int len);

typedef struct
{
    char at_command[1024];
    int length;
    char ok_fmt[64];
    char err_fmt[64];
    int ok_flag;
}nwy_at_info_t;

typedef struct nwy_at_unsolicite_table
{
    char at_prefix[32];
    nwy_at_unsolicite_handle nwy_at_unsolicite_cb;
}nwy_at_unsolicite_table_t;

typedef struct nwy_report_info
{
    uint8 *report_data;
    int len;
}nwy_report_info;

typedef void (*nwy_at_rsp_report_cb)(char *resp, int len);
/**
 * @init at parameter.
 *
 * @param
 * @return
 */

void nwy_sdk_at_parameter_init();

/**
 * @.send at command
 *
 * @param pInfo:at infor
          resp:at response
          timeout:5-30s
 * @return
        sucess:NWY_SUCESS
        fail:others
 */
    int nwy_sdk_at_cmd_send(nwy_at_info_t *pInfo, char *resp, int resp_len, int timeout);

/**
 * @.register the unsolicited at process
 *
 * @param at_prefix:the unsolicited at prefix
          p_func:the process function
 * @return
        sucess:NWY_SUCESS
        fail:others
 */
int nwy_sdk_at_unsolicited_cb_reg(char *at_prefix, void *p_func);

void nwy_sdk_at_get_original_rsp_cb(nwy_get_original_at_rsp cb);
void nwy_set_at_forward_cb(int index,const char* name, nwy_at_forward_process_cb cb);
void nwy_at_forward_send(void* handle, char* buf, int len);


#ifdef __cplusplus
   }
#endif

#endif
