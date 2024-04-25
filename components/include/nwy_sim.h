/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====
    Copyright (c) 2018 Neoway Technologies, Inc.
    All rights reserved.
    Confidential and Proprietary - Neoway Technologies, Inc.
    Author: lipengchao
    Date: 2019/12
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef __NWY_SIM_H__
#define __NWY_SIM_H__

#include "nwy_common.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
typedef enum nwy_result
{
  NWY_RES_OK = 0,
  NWY_RES_ERROR = -1
}nwy_result_type;
*/
typedef enum nwy_sim_status
{
  NWY_SIM_STATUS_GET_ERROR = -1,
  NWY_SIM_STATUS_READY,
  NWY_SIM_STATUS_NOT_INSERT,
  NWY_SIM_STATUS_PIN1,
  NWY_SIM_STATUS_PUK1,
  NWY_SIM_STATUS_BUSY,
}nwy_sim_status;

typedef enum
{
  NWY_SIM_PIN_DISABLE,                  //"READY"
  NWY_SIM_PIN_ENABLE,
  NWY_SIM_PIN_MAX,
}nwy_pin_mode_type;

typedef struct nwy_sim_result
{
  unsigned char iccid[23];
  unsigned char imsi[16];
  unsigned char lock_sim_password[12];
  unsigned char unlock_sim_password[12];
  nwy_pin_mode_type pin_mode;
  unsigned char nImei[16];
}nwy_sim_result_type;

typedef enum
{
  NWY_SIM_ID_SLOT_1,
  NWY_SIM_ID_SLOT_2,
  NWY_SIM_ID_MAX
}nwy_sim_id_t;

typedef enum 
{
NWY_SIM_STATUS_URC_NOT_INSERT = 0,
NWY_SIM_STATUS_URC_INSERT,
}nwy_sim_status_urc_e;

typedef enum 
{
    NWY_SIM_PRESENT_URC = 0, //nwy_sim_status_urc_e

}nwy_sim_ind_type_e;


typedef int (* nwy_sim_recv_cb_t)(nwy_sim_ind_type_e type, void *data, size_t size);


nwy_sim_status nwy_sim_get_card_status(nwy_sim_id_t sim_id);

int nwy_sim_get_iccid(nwy_sim_id_t sim_id, char *iccid_buf, size_t buf_len);

int nwy_sim_get_imsi(nwy_sim_id_t sim_id, char *imsi_buf, size_t buf_len);


int nwy_sim_enable_pin(nwy_sim_id_t sim_id, const char *pin);

int nwy_sim_disable_pin(nwy_sim_id_t sim_id, const char *pin);


nwy_pin_mode_type nwy_sim_get_pin_mode(nwy_sim_id_t sim_id);


int nwy_sim_get_msisdn(nwy_sim_id_t sim_id, char* msisdn_buf, size_t buf_len);


int nwy_sim_change_pin(nwy_sim_id_t sim_id, const char *old_pin, const char *new_pin);

int nwy_sim_verify_pin(nwy_sim_id_t sim_id, const char* pin);

int nwy_sim_unblock(nwy_sim_id_t sim_id, const char* puk, const char* new_pin);

/*Start:add by yangmengsha for config sim detect enable or disable 20220714*/
/**
 * @brief set sim detect enable or disable
 *
 * @param mode : 0--disable, 1--enable
 * @return
 *      - NULL
 */
void nwy_set_sim_detect_mode(bool enable);
/*End:add by yangmengsha for config sim detect enable or disable 20220714*/

void nwy_set_sim_detect_trigger_mode(bool mode); //update by gaohe for Eliminate the warning in 2021/11/11

int nwy_sim_set_simid(nwy_sim_id_t sim_id);
int nwy_sim_get_simid();

int nwy_sim_get_imei(nwy_sim_id_t sim_id, char *imei_buf, size_t buf_len);

int nwy_sim_reg_recv_cb(nwy_sim_recv_cb_t recv_cb);

int nwy_sim_module_reset(nwy_sim_id_t sim_id);



#ifdef __cplusplus
   }
#endif


#endif
