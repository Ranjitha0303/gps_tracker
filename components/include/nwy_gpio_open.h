#ifndef __NWY_GPIO_OPEN_H__
#define __NWY_GPIO_OPEN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NWY_GPIO_MAX_COUNT 48

typedef enum nwy_dir_mode_t{
    nwy_input =0,
    nwy_output,

    nwy_dir_null
}nwy_dir_mode_t;

typedef enum nwy_value_t{
    nwy_low=0,
    nwy_high=1,
  
  nwy_value_null
}nwy_value_t;
typedef enum nwy_dir_t{
  NWY_INPUT =0,
  NWY_OUTPUT,
  NWY_IRQ_RISING,
  NWY_IRQ_FALLING,
  NWY_IRQ_RISING_FALLING,
  NWY_IRQ_HIGH,
  NWY_IRQ_LOW,
  NWY_SLEEP_WAKE,
  
  NWY_DIR_NULL
}nwy_dir_t;

typedef enum nwy_val_t{
  NWY_LOW=0,
  NWY_HIGH=1,
  
  NWY_VAL_NULL
}nwy_val_t;


typedef enum nwy_irq_mode_t{
    nwy_irq_rising = 0,
    nwy_irq_falling,
    nwy_irq_rising_falling,
    nwy_irq_high,
    nwy_irq_low
}nwy_irq_mode_t;

typedef void(*nwy_irq_callback)(void * param);

/*---------------------------Function Definition--------------------------*/
/**
 * @brief config the irq mode
 *
 * @param gpio id : gpio id
 * @param irq_mode: nwy_irq_mode_t
 * @param cb : nwy_irq_callback
 * @return
 *      - SUCCESS    1
 *      - ERROR      0
 */
int nwy_open_gpio_irq_config(uint32_t gpio_id, nwy_irq_mode_t irq_mode, nwy_irq_callback cb);

/**
 * @brief set the gpio value
 *
 * @param gpio id : gpio id
 * @param value: nwy_value_t
 * @return
 *      - SUCCESS    1
 *      - ERROR      0
 */
int nwy_gpio_set_value(uint32_t gpio_id, nwy_value_t value);

/**
 * @brief get the gpio value
 *
 * @param gpio id : gpio id
 * @return
 *      - nwy_value_t         2
 *      - High                1
 *      - Low                 0
 */
int nwy_gpio_get_value(uint32_t gpio_id);

/**
 * @brief set the gpio direction
 *
 * @param gpio id : gpio id
 * @param dirct: nwy_dir_mode_t
 * @return
 *      - SUCCESS    1
 *      - ERROR      0
 */
int nwy_gpio_set_direction(uint32_t gpio_id, nwy_dir_mode_t dirct);

/**
 * @brief get the gpio direction
 *
 * @param gpio id : gpio id
 * @return
 *      - nwy_dir_null        2
 *      - Output              1
 *      - Input               0
 */
int nwy_gpio_get_direction(uint32_t gpio_id);

/**
 * @brief enable the gpio irq
 *
 * @param gpio id : gpio id
 * @return
 *      - SUCCESS         1
 *      - ERROR           0
 */
int nwy_gpio_open_irq_enable(uint32_t gpio_id);

/**
 * @brief disable the gpio irq
 *
 * @param gpio id : gpio id
 * @return
 *      - SUCCESS         1
 *      - ERROR           0
 */
int nwy_gpio_open_irq_disable(uint32_t gpio_id);

/**
 * @brief colse the gpio
 *
 * @param gpio id : gpio id
 * @return
 *      - SUCCESS         1
 *      - ERROR           0
 */
int nwy_close_gpio(uint32_t gpio_id);

/**
 * @brief pull the gpio
 *
 * @param gpio id : gpio id
 * @param pull:0-dn    1-up    2-null
 * @return
 *      - SUCCESS         1
 *      - ERROR           -1
 */
int nwy_gpio_pullup_or_pulldown(uint32_t gpio_id, int pull);

/**
 * @brief set pad driving 
 *
 * @param gpio id : gpio id
 * @param driving :2~15
 * @return
 */
void nwy_set_pad_driving(uint32_t id, unsigned driving);

/**
 * @brief set the netlight&ring for open 
 *
 * @param net_light: net_light gpio id
 * @param ring: ring gpio id
 * @return
 */
void nwy_set_netlight_ring_cfg(uint8_t net_light, uint8_t ring);

/*Begin : add by pengduting for DL master_slave_mode 20240115*/
#ifdef FEATURE_NWY_AT_MASTER_SLAVE_DL
int nwy_get_master_slave_mode();
#endif
/*End : add by pengduting for DL master_slave_mode 20240115*/

#ifdef __cplusplus
}
#endif

#endif/*__NWY_GPIO_OPEN_H__*/


