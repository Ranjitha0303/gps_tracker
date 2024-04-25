#ifndef __NWY_FOTA_H__
#define __NWY_FOTA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nwy_common.h"

typedef struct _ota_package {
    unsigned int ota_size;
    unsigned int offset;
    unsigned int len;
    unsigned char *data;
} ota_package_t;

/*----------------------------Type Definition-----------------------------*/

/*---------------------------Function Definition--------------------------*/

/**
 * write fota data to update file
 *
 *
 * \param [in] ota_package_t   

 * \return
 *      0-- success
 *     -1-- failed
 */
int nwy_fota_download_core(ota_package_t *ota_pack);

/**
 * \brief set firmware update to ready status
 *
 * Inside this function, the update pack file will be thoroughly checked.
 *
 * When the update pack file is valid:
 *    set firmware update to ready status
 *
 * It must be called after the update pack file is written and closed. If
 * firmware update to ready status, bootloader will try to update. 
 *
 * \param [in] bRst     if true, system will be reboot for update
 *                      else , system will be update at next reboot
 * \return
 *      -  0 success
 *      - -1 fail
 */
int nwy_version_core_update(bool bRst);

/**
 * \brief get firmware update result after update reboot 
 *
 * \return
 *      -  0 success
 *      - -1 fail
 */
 int nwy_get_update_result(void);

 /*N706 not support*/
 int nwy_fota_dm(ota_package_t *ota_pack);
 int nwy_package_checksum(void);
 int nwy_fota_ua(void);
 /*N706 not support*/

#ifdef __cplusplus
   }
#endif

#endif // __NWY_FOTA_H__


