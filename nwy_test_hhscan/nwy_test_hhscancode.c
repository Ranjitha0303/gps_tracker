/*
 *   Copyright (c) 2014 - 2019 Oleh Kulykov <info@resident.name>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <nwy_camera.h>
#include <nwy_hhscancode.h>
#include <nwy_osi_api.h>
#include "nwy_usb_serial.h"
#include <nwy_camera.h>

#define DISABLE 0
#define ENABLE  1
#define NWY_CLI_ECHO_LEN (4*1024)
#define QR_WIDTH      (640)
#define QR_HEIGHT     (480)
#define QR_YUV_SIZE   (QR_WIDTH*QR_HEIGHT)

uint8_t nwy_camscan_status = DISABLE;
nwy_osi_thread_t g_camscan_thread = NULL;
nwy_osi_thread_t nwy_test_hhscancode_thread = NULL;
unsigned char *code_yuv = NULL;
uint16_t *pCamPreviewDataBuffer = NULL;
typedef uint8_t nvmImei_t[15];

int nvmReadImei(int index, nvmImei_t *imei)
{
    nwy_dm_get_imei((char *)imei, 15);
    return 15;
}

void nwy_test_cli_echo(char* fmt, ...)
{
  static char echo_str[NWY_CLI_ECHO_LEN];
  static nwy_osi_mutex_t echo_mutex = NULL;
  va_list a;
  int i, size;

  if(NULL == echo_mutex)
    nwy_create_mutex(&echo_mutex);
  if(NULL == echo_mutex)
    return;
  nwy_lock_mutex(echo_mutex, 0);
  va_start(a, fmt);
  vsnprintf(echo_str, NWY_CLI_ECHO_LEN, fmt, a);
  va_end(a);
  size = strlen((char *)echo_str);
  i = 0;
  while(1)
  {
    int tx_size;
    tx_size = nwy_usb_serial_send((char *)echo_str + i, size - i);
    if(tx_size <= 0)
      break;
    i += tx_size;
    if((i < size))
      nwy_sleep(10);
    else
      break;
  }
  nwy_unlock_mutex(echo_mutex);
}

static void yuyv_to_y800(const unsigned char *in_buf, unsigned char *out_buf, unsigned int width, unsigned int height)
{
  nwy_test_cli_echo("enter %s\r\n", __func__);
  unsigned char y1, y2, y3, y4;
  unsigned char *Y;
  int width_2 = width >> 2;

  Y = out_buf;

  if(in_buf == NULL)
  {
    nwy_test_cli_echo("in_buf NULL Ptr Err\r\n");
    return;
  }
  if(out_buf == NULL)
  {
    nwy_test_cli_echo("out_buf NULL Ptr Err \r\n");
    return;
  }

  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width_2; j++)
    {
      y1 = *(in_buf + ((i*width_2+j)<<3) + 0);
      y2 = *(in_buf + ((i*width_2+j)<<3) + 2);
      y3 = *(in_buf + ((i*width_2+j)<<3) + 4);
      y4 = *(in_buf + ((i*width_2+j)<<3) + 6);

      *Y++ = y1;
      *Y++ = y2;
      *Y++ = y3;
      *Y++ = y4;
    }
  }
  nwy_test_cli_echo("%s exit\r\n", __func__);
}


static void nwy_cam_scancode(void)
{
  nwy_test_cli_echo("enter %s\r\n", __func__);
  char scan_result[3096];
  memset(scan_result,0,sizeof(scan_result));

  //init 
  nwy_hh_init();
  pCamPreviewDataBuffer = NULL;
  nwy_camera_get_preview(&pCamPreviewDataBuffer);
  if(pCamPreviewDataBuffer == NULL)
  {
    nwy_test_cli_echo("get perviewdata failed\r\n");
    return ;
  }
  memset(code_yuv, 0, QR_YUV_SIZE);
  yuyv_to_y800(pCamPreviewDataBuffer, code_yuv, QR_WIDTH, QR_HEIGHT);
  int len = Decoding_Image_HH(code_yuv, QR_WIDTH, QR_HEIGHT);
  if(0 < len)
  {
    if(0 < GetResultLength_HH())
    {
      GetDecoderResult_HH(scan_result);
      nwy_test_cli_echo("scan_result len = %d, scan_result = %s\r\n",strlen(scan_result), scan_result);
   }
   else
     nwy_test_cli_echo("GetResultLength failed\r\n");
  }
  else
    nwy_test_cli_echo("Decoding_Image failed\r\n");
}

static void nwy_cam_scan_task_entry_hh(void)
{
  nwy_test_cli_echo("enter %s\r\n", __func__);

  while(1)
  {
    if(nwy_camscan_status)
        //nwy_test_cli_echo("scancode Start\r\n");
        nwy_cam_scancode();
    else
    {
      nwy_test_cli_echo("scancode Stop\r\n");
      break;
    }
    nwy_sleep(20);
  }
  g_camscan_thread = NULL;
  nwy_exit_thread_self();
}

int nwy_create_camscan_task(void)
{
  if(g_camscan_thread == NULL)
  {
    int ret = nwy_create_thread(&g_camscan_thread, 1024*30, 24, "g_camscan_thread", nwy_cam_scan_task_entry_hh, NULL, 0);
    if(ret != 0)
    {
      nwy_test_cli_echo("nwy_create_thread g_camscan_thread fail\r\n");
      return -1;
    }
  }
  nwy_sleep(2*1000);
  return 0;
}

static void nwy_test_main_func(void *param)
{
  while(nwy_camera_open() < 0)
  {
    nwy_test_cli_echo("nwy_camera_open failed!\r\n");
    nwy_sleep(1*1000);
  }
  nwy_test_cli_echo("nwy_camera_open success!\r\n");
  while(nwy_camera_start_preview() < 0)
  {
    nwy_test_cli_echo("nwy_camera_start_preview failed!\r\n");
    nwy_sleep(1*1000);
  }
  nwy_test_cli_echo("nwy_camera_start_preview start suc\r\n");
//malloc code_yuv
  if(code_yuv == NULL)
  {
    code_yuv = (unsigned char *)malloc(QR_YUV_SIZE);
    if(code_yuv == NULL)
    {
      nwy_test_cli_echo("code_yuv malloc failed\r\n");
      nwy_camera_close_preview();
      nwy_camera_close();
      nwy_camscan_status = DISABLE;
      return;
    }
  }
  nwy_camscan_status = ENABLE;

  if(nwy_create_camscan_task() < 0)
  {
    nwy_camera_close_preview();
    nwy_camera_close();
    nwy_camscan_status = DISABLE;
    if(code_yuv != NULL)
    {
      nwy_test_cli_echo(" free code_yuv\r\n");
      free(code_yuv);
      code_yuv = NULL;
    }
    return;
  }
  while(1)
    nwy_sleep(1*1000);
}

int appimg_enter(void *param)
{
  nwy_sleep(10*1000);
  nwy_create_thread(&nwy_test_hhscancode_thread, 1024*35, NWY_OSI_PRIORITY_NORMAL, "test_hhscancode", nwy_test_main_func, NULL,  0);
  nwy_sleep(2*1000);
  return 0;
}

void appimg_exit(void)
{
}
