#ifndef __NWY_PM_H__
#define __NWY_PM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nwy_keypad.h"

/*----------------------------Type Definition-----------------------------*/
typedef enum {
    NWY_POWER_DROP_STATE   = 0,
    NWY_POWER_NORMAL_STATE = 1
}nwy_power_state_t;

typedef enum {
    NWY_QUICK_POWER_OFF  = 0,
    NWY_NORMAL_POWER_OFF = 1
}nwy_power_off_t;

typedef enum {
    NWY_WAKEUP = 0,
    NWY_LP_DISABLE_SLEEP = NWY_WAKEUP,
    NWY_ENTRY_SLEEP = 1,
    NWY_LP_ENABLE_SLEEP = NWY_ENTRY_SLEEP,
    NWY_LP_DISABLE_PSM,
    NWY_LP_ENABLE_PSM,

    NWY_LP_NONE
}nwy_pm_set_t;

typedef enum
{
    NWY_POWER_LCD,
    NWY_POWER_KEYLED,
    NWY_POWER_BACK_LIGHT,
    NWY_POWER_SD,
    NWY_POWER_CAMA,
    NWY_POWER_CAMD,
    NWY_POWER_SIM1,
    NWY_POWER_RGB_IB0,
    NWY_POWER_RGB_IB1,
    NWY_POWER_RGB_IB2
} nwy_sub_power_t;

/*---------------------------Function Definition--------------------------*/

/**
 * \brief set PSM sleep time from now
 *
 * \param ms        PSM sleep time from now
 *                  unit second
 * @return
 *      - SUCCESS    0
 *      - ERROR     -1
 */
int nwy_pm_psm_time_set(unsigned int secs);

/**
 * @brief set the pm state
 *
 * @param mode : 0-NWY_WAKEUP,1-NWY_ENTRY_SLEEP,2-NWY_LP_DISABLE_PSM,3-NWY_LP_ENABLE_PSM
 * @return
 *      - SUCCESS    0
 *      - ERROR     -1
 */
int nwy_pm_state_set(int mode);

/**
 * @brief view the pm state
 *
 * @param 
 * @return
 *      - NWY_POWER_DROP_STATE    0
 *      - NWY_POWER_NORMAL_STATE  1
 */
int nwy_power_state(void);

/**
 * @brief set the pm power off state
 *
 * @param option : 0-quickly power off,1-normal power off,2-reset
 * @return
 *      - SUCCESS    1
 *      - ERROR     -1
 */
int nwy_power_off(int option);

/**
 * @brief normal shutdown, not psm shutdown
 *
 * @return
 *      - SUCCESS    1
 *      - ERROR      0
 */
bool nwyShutdown(void);

/**
 * @brief set powerkey control shutdown
 *
 * @param enable : 0-close,1-open
 * @return
 */
void nwy_powerkey_poweroff_ctrl(bool enable);

/**
 * @brief switch sub power
 *
 * @param id : sub power id
 * @param enabled : 0-close,1-open at normal run
 * @param lp_enabled : 0-close,1-open at lowpower mode
 
 * @return
 */
bool nwy_subpower_switch(unsigned int id, bool enabled, bool lp_enabled);
typedef enum
{
    // Charge message.
    NWY_CAP_IND = 0x1, // Notify the battery's capacity

    NWY_CHARGE_START_IND, // start the charge process.
    NWY_CHARGE_END_IND,   // the charge ended.

    NWY_WARNING_IND,  // the capacity is low, should charge.
    NWY_SHUTDOWN_IND, // the capacity is very low and must shutdown.

    NWY_CHARGE_FINISH,     // the charge has been completed.
    NWY_CHARGE_DISCONNECT, // the charge be disconnect
    NWY_CHARGE_FAULT,      // the charge fault, maybe the voltage of charge is too low.

    NWY_MSG_MAX_NUM
} NWY_SVR_MSG_SERVICE_E;

typedef void (*ChargerNoticeCB_t)(NWY_SVR_MSG_SERVICE_E msg, uint32_t param);

/**
 * @brief set powerkey control shutdown
 *
 * @param shut_vol : shutdown_vol mv
 * @param dead_vol : deadline_vol mv
 * @param count : warning_count,warning interval
 * @return
 *      - SUCCESS    1
 *      - ERROR     0
 */
bool nwy_set_auto_poweroff(uint16_t shut_vol, uint16_t dead_vol, int count, ChargerNoticeCB_t cb);

int nwy_set_pmu_power_level(uint32_t id, uint32_t mv);

#define NWY_BOOTCAUSE_UNKNOWN     0            ///< placeholder for unknown reason
#define NWY_BOOTCAUSE_PWRKEY      (1 << 0)     ///< boot by power key
#define NWY_BOOTCAUSE_PIN_RESET   (1 << 1)     ///< boot by pin reset
#define NWY_BOOTCAUSE_ALARM       (1 << 2)     ///< boot by alarm
#define NWY_BOOTCAUSE_CHARGE      (1 << 3)     ///< boot by charge in
#define NWY_BOOTCAUSE_WDG         (1 << 4)     ///< boot by watchdog
#define NWY_BOOTCAUSE_PIN_WAKEUP  (1 << 5)     ///< boot by wakeup
#define NWY_BOOTCAUSE_SMPL_WAKEUP (1 << 6) ///< boot from SMPL
#define NWY_BOOTCAUSE_GPT_WAKEUP  (1 << 7)  ///< boot from GPT
#define NWY_BOOTCAUSE_WDG_IN_PSM  (1 << 8)  ///< boot by watchdog rst
#define NWY_BOOTCAUSE_PANIC       (1 << 9)       ///< boot by panic reset
#define NWY_BOOTCAUSE_PM2BOOT     (1 << 10)    ///< boot from PM2 cold boot

/**
 * \brief get boot causes
 *
 * It is possible there are multiple boot causes. The returned value is
 * bit or of all boot causes.
 *
 * \return  boot causes
 */
unsigned int nwy_get_boot_causes(void);

/**
 * \brief : set bootup alarm
 * @param sec_in_day : alarm time second in every day
 *
 * \return:
 *      - SUCCESS   0
 *      - ERROR     -1
 */
int nwy_bootup_alarm_set(uint32_t sec_in_day);

/**
 * \brief : delete bootup alarm
 *
 * \return:
 *      - SUCCESS   0
 *      - ERROR     -1
 */
int nwy_bootup_alarm_del(void);

/**
 * \brief : get if support power off charge mode.
 * @param 
 *
 * \return:
 *      - true: support
 *      - false: not support
 */
bool nwy_get_poweroff_charge_mode(void);

/**
 * \brief : set support power off charge mode,this api mast restart,
 *          enable in poweron init
 * @param enable: true:enable; false: disable;
 *
 * \return:
 */
void nwy_set_poweroff_charge_mode(bool enable);

/**
 * @brief return the charger and battery info 
 
 * @param nBcs  set the channel to measue
 * 0 No charging adapter is connected
 * 1 Charging adapter is connected
 * 2 Charging adapter is connected, charging in progress
 * 3 Charging adapter is connected, charging has finished
 * 4 Charging error, charging is interrupted
 * 5 False charging temperature, charging is interrupted while temperature is beyond allowed range
 * @param nBcl   percent of remaining capacity.
 */
void nwy_get_charge_info(uint8_t *nBcs, uint8_t *nBcl);

/**
 * @brief get usb state
 *
 * @return
 *      - 1: connected
 *      - 0: disconnected
 */
bool nwy_get_usb_status();

/**
 * @brief set auto power on config when vbat voltage rise
 
 * @param    enable: true:enable; false: disable;
 * power_on_voltage: power on thredhold, unit mV
 * monitor_period  : monitor period after voltage lost, unit second
 *
 * @return:
 *      - SUCCESS   0
 *      - ERROR     -1
 */
int nwy_pm_set_auto_power_on_cfg(int enable, uint32_t power_on_voltage, uint32_t monitor_period);

/**
 * @brief get auto power on config when vbat voltage rise
 
 * @param    enable: true:enable; false: disable;
 * power_on_voltage: power on thredhold, unit mV
 * monitor_period  : monitor period after voltage lost, unit second
 *
 * @return:
 *      - SUCCESS   0
 *      - ERROR     -1
 */
int nwy_pm_get_auto_power_on_cfg(int *enable, uint32_t *power_on_voltage, uint32_t *monitor_period);


/**
 * @brief set enable or disable log when insleep
 
 * @param    enable: true:enable; false: disable;
 * power_on_voltage: power on thredhold, unit mV
 *
 * @return:
 *      - 1
 */
int nwy_set_sleeplog_flag(int enable);

int nwy_get_chip_id( uint8_t *uid);

void nwy_set_back_light_level(uint32_t level);

/**
 * @brief set backlight level separately
 
 * @param    enable: true:enable; false: disable;
 * power_on_voltage: power on thredhold, unit mV
 *
 * @return:
 *      - SUCCESS   1
 *      - ERROR     0
 *back_light   NWY_POWER_RGB_IB0/NWY_POWER_RGB_IB1/NWY_POWER_RGB_IB2
 *level   (0~63)
 */
int nwy_set_back_light_level2(uint32_t back_light, uint32_t level);

#ifdef __cplusplus
   }
#endif

#endif
 
