#ifndef __NWY_TEST_CLI_UTILS_H__
#define __NWY_TEST_CLI_UTILS_H__

#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "nwy_test_cli_adpt.h"

#define NWY_UART_RECV_SINGLE_MAX   512
#define NWY_EXT_SIO_RX_MAX         (2*1024)
#define CERT_EFS_CHECKSUM_STR_SIZE 4


#define NWY_EXT_INPUT_RECV_MSG          (NWY_APP_EVENT_ID_BASE + 1)
#define NWY_EXT_DATA_REC_END_MSG        (NWY_APP_EVENT_ID_BASE + 2)

typedef void (*nwy_sio_trans_cb) (const char *data, int length);

void nwy_test_cli_echo(char* fmt, ...);
void nwy_test_cli_output(void *data, uint32 length);
char* nwy_test_cli_input_gets(char* msg, ...);
void nwy_test_cli_sio_data_proc(const char *data, uint32 length);
int nwy_test_cli_input_len_gets(void);
int nwy_test_cli_sio_enter_trans_mode(nwy_sio_trans_cb recv_cb);
void nwy_test_cli_sio_quit_trans_mode(void);
void nwy_sms_test_recv_cb(void *data, size_t size);
int nwy_cli_str_to_int(const char* str);
//my changes 
typedef enum
{
	SNA_en = 0,
	send_10sec_en ,
	send_1min_en ,
} mcu_cutoff_handler_en_t ;
extern mcu_cutoff_handler_en_t mcu_cutoff_handler_en; 
extern int test_connect ;
typedef struct
{
    double latitude;
    double longitude;
    double altitude;
}loc_info_t;
extern loc_info_t loc_info ;
//extern char *info_String ;
extern char info_string[100] ;
#ifdef NWY_OPEN_CLI_UIS8850
void nwy_test_cli_nwy_reset_gprs_service(void);
void nwy_test_cli_data_urc(void);
#endif
#endif
