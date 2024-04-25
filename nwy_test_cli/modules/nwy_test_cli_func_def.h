#ifndef __NWY_TEST_CLI_FUNC_DEF_H__
#define __NWY_TEST_CLI_FUNC_DEF_H__

/**************************SIM*********************************/
void nwy_test_cli_get_sim_status(void);
void nwy_test_cli_verify_pin(void);
void nwy_test_cli_get_pin_mode(void);
void nwy_test_cli_set_pin_mode(void);
void nwy_test_cli_change_pin(void);
void nwy_test_cli_verify_puk(void);
void nwy_test_cli_get_imsi(void);
void nwy_test_cli_get_iccid(void);
void nwy_test_cli_get_msisdn(void);
void nwy_test_cli_set_msisdn(void);
void nwy_test_cli_get_pin_puk_times(void);
void nwy_test_cli_get_sim_slot(void);
void nwy_test_cli_set_sim_slot(void);
void nwy_test_cli_set_csim(void);
void nwy_poc_pocds_test();

void nwy_recv_sat_info_cb(void);
void nwy_test_cli_set_switch_stk(void);
void nwy_test_cli_get_switch_stk(void);
void nwy_test_cli_terminal_response_stk(void);
void nwy_test_cli_envelope_command_stk(void);
void nwy_test_cli_set_profile_stk(void);
void nwy_test_cli_get_profile_stk(void);

#ifdef NWY_OPEN_CLI_UIS8850
void nwy_test_cli_sim_urc();
void nwy_test_cli_sim_module_reset(void);
void nwy_test_cli_set_sim_detect_mode(void);
void nwy_test_cli_set_sim_detect_trigger_mode(void);
#endif
/**************************DATA*********************************/
void nwy_test_cli_data_create(void);
void nwy_test_cli_get_profile(void);
void nwy_test_cli_set_profile(void);
void nwy_test_cli_data_start(void);
void nwy_test_cli_data_info(void);
void nwy_test_cli_data_stop(void);
void nwy_test_cli_data_release(void);
void nwy_test_cli_data_get_flowcalc(void);
void nwy_test_cli_set_dns_server(void);
void nwy_test_cli_get_dns_server(void);
void nwy_test_cli_nwy_set_netsharemode(void);
void nwy_test_cli_nwy_set_netshareact(void);


/**************************NW*********************************/
void nwy_test_cli_nw_get_mode(void);
void nwy_test_cli_nw_set_mode(void);
void nwy_test_cli_nw_get_fplmn(void);
void nwy_test_cli_nw_manual_scan(void);
void nwy_test_cli_nw_band_lock(void);
void nwy_test_cli_nw_freq_lock(void);
void nwy_test_cli_nw_get_ims_st(void);
void nwy_test_cli_nw_set_ims_st(void);
void nwy_test_cli_nw_get_def_pdn(void);
void nwy_test_cli_nw_set_def_pdn(void);
void nwy_test_cli_nw_get_radio_st(void);
void nwy_test_cli_nw_set_radio_st(void);
void nwy_test_cli_nw_get_radio_sign(void);
void nwy_test_cli_nw_cs_st(void);
void nwy_test_cli_nw_ps_st(void);
void nwy_test_cli_nw_lte_st(void);
void nwy_test_cli_nw_operator_info(void);
void nwy_test_cli_nw_get_ehplmn(void);
void nwy_test_cli_nw_get_signal_rssi(void);
void nwy_test_cli_nw_get_netmsg(void);
void nwy_test_cli_nw_get_cfgdftpdn_info(void);
void nwy_test_cli_nw_set_cfgdftpdn_info(void);
void nwy_test_cli_nw_get_cellmsg(void);


/**************************VOICE*********************************/
void nwy_test_cli_voice_call_start(void);
void nwy_test_cli_voice_call_end(void);
void nwy_test_cli_voice_auto_answ(void);
void nwy_test_cli_voice_volte_set(void);
void nwy_test_cli_voice_state_change(void);
void nwy_test_cli_voice_caller_id(void);
void nwy_test_cli_voice_call_hold(void);
void nwy_test_cli_voice_call_unhold(void);

/**************************SMS*********************************/
void nwy_test_cli_sms_send(void);
void nwy_test_cli_sms_del(void);
void nwy_test_cli_sms_get_sca(void);
void nwy_test_cli_sms_set_sca(void);
void nwy_test_cli_sms_set_storage(void);
void nwy_test_cli_sms_get_storage(void);
void nwy_test_cli_sms_set_report_md(void);
void nwy_test_cli_sms_read(void);
void nwy_test_cli_sms_list(void);
void nwy_test_cli_sms_del_type(void);

/**************************UART*********************************/
void nwy_test_cli_uart_init(void);
void nwy_test_cli_uart_set_baud(void);
void nwy_test_cli_uart_get_baud(void);
void nwy_test_cli_uart_set_para(void);
void nwy_test_cli_uart_get_para(void);
void nwy_test_cli_uart_set_tout(void);
void nwy_test_cli_uart_send(void);
void nwy_test_cli_uart_reg_rx_cb(void);
void nwy_test_cli_uart_reg_tx_cb(void);
void nwy_test_cli_uart_deinit(void);

/**************************I2C*********************************/
void nwy_test_cli_i2c_init(void);
void nwy_test_cli_i2c_read(void);
void nwy_test_cli_i2c_write(void);
void nwy_test_cli_i2c_put_raw(void);
void nwy_test_cli_i2c_get_raw(void);
void nwy_test_cli_i2c_deinit(void);

/**************************SPI*********************************/
void nwy_test_cli_spi_init(void);
void nwy_test_cli_spi_trans(void);
void nwy_test_cli_spi_deinit(void);
void nwy_test_cli_spi_flash_mount(void);
void nwy_test_cli_spi_flash_data(void);

/**************************GPIO*********************************/
void nwy_test_cli_gpio_set_val(void);
void nwy_test_cli_gpio_get_val(void);
void nwy_test_cli_gpio_set_dirt(void);
void nwy_test_cli_gpio_get_dirt(void);
void nwy_test_cli_gpio_config_irq(void);
void nwy_test_cli_gpio_enable_irq(void);
void nwy_test_cli_gpio_disable_irq(void);
void nwy_test_cli_gpio_close(void);
void nwy_test_cli_gpio_pull(void);

/**************************ADC*********************************/
void nwy_test_cli_adc_read(void);

/**************************PM*********************************/
void nwy_test_cli_pm_save_md(void);
void nwy_test_cli_pm_get_pwr_st(void);
void nwy_test_cli_pm_pwr_off(void);
void nwy_test_cli_pm_set_dtr(void);
void nwy_test_cli_pm_pwr_key(void);
void nwy_test_cli_pm_switch_sub_pwr(void);
void nwy_test_cli_pm_set_sub_pwr(void);
void nwy_test_cli_pm_set_auto_off(void);
void nwy_test_cli_pm_reg_charger_cb(void);

/**************************KEYPAD*********************************/
void nwy_test_cli_keypad_reg_cb(void);
void nwy_test_cli_keypad_set_debouce(void);

/**************************PWM*********************************/
void nwy_test_cli_pwm_init(void);
void nwy_test_cli_pwm_start(void);
void nwy_test_cli_pwm_stop(void);
void nwy_test_cli_pwm_deinit(void);

/**************************BK4819*********************************/
#ifdef NWY_OPEN_TEST_BK4819
void nwy_test_bk4819_spi_gpio_init(void);
void nwy_test_bk4819_spi_gpio_deinit(void);
void nwy_test_bk4819_spi_read(void);
void nwy_test_bk4819_spi_write(void);
void nwy_test_rf_initial(void);
void nwy_test_rf_set_txau(void);
void nwy_test_rf_set_agc(void);
void nwy_test_rf_set_chn_space(void);
void nwy_test_rf_set_freq(void);
void nwy_test_rf_set_ctcss(void);
void nwy_test_rf_set_ctc2(void);
void nwy_test_rf_set_cdcss(void);
void nwy_test_rf_exit_subau(void);
void nwy_test_rf_set_af(void);
void nwy_test_rf_enter_txmute(void);
void nwy_test_rf_exit_txmute(void);
void nwy_test_rf_enter_scramble(void);
void nwy_test_rf_exit_scramble(void);
void nwy_test_rf_enter_vox(void);
void nwy_test_rf_exit_vox(void);
void nwy_test_rf_enter_compander(void);
void nwy_test_rf_exit_compander(void);
void nwy_test_rf_enter_bypass(void);
void nwy_test_rf_exit_bypass(void);
void nwy_test_rf_gen_tail(void);
void nwy_test_rf_key(void);
void nwy_test_rf_call(void);
void nwy_test_rf_txon_beep(void);
void nwy_test_rf_tx_on(void);
void nwy_test_rf_rx_on(void);
void nwy_test_rf_idle(void);
void nwy_test_rf_sleep(void);
void nwy_test_rf_wakeup(void);
void nwy_test_rf_set_af_response(void);
void nwy_test_rf_set_xtal(void);
void nwy_test_rf_get_sq(void);
void nwy_test_rf_get_ctcss(void);
void nwy_test_rf_get_tail(void);
void nwy_test_rf_get_cdcss(void);
void nwy_test_rf_get_vox(void);
void nwy_test_rf_get_vox_amp(void);
void nwy_test_rf_get_rssi(void);
void nwy_test_rf_get_noise(void);
void nwy_test_rf_get_glitch(void);
void nwy_test_set_gpio_out(void);
void nwy_test_rf_get_gpio_in(void);
void nwy_test_rf_enter_dtmf(void);
void nwy_test_rf_exit_dtmf(void);
void nwy_test_rf_dtmf_transmit(void);
void nwy_test_rf_dtmf_receive(void);
void nwy_test_rf_enter_5tone(void);
void nwy_test_rf_exit_5tone(void);
void nwy_test_rf_5tone_transmit(void);
void nwy_test_rf_5tone_receive(void);
void nwy_test_rf_enter_fsk(void);
void nwy_test_rf_exit_fsk(void);
void nwy_test_rf_fsk_idle(void);
void nwy_test_rf_fsk_transmit(void);
void nwy_test_rf_fsk_receive(void);
void nwy_test_rf_enter_mdc(void);
void nwy_test_rf_exit_mdc(void);
void nwy_test_rf_mdc_transmit(void);
void nwy_test_rf_mdc_receive(void);
void nwy_test_rf_ctc_dcs_scan(void);
void nwy_test_rf_freq_scan(void);
#endif

/**************************LCD*********************************/
void nwy_test_cli_lcd_open(void);
void nwy_test_cli_lcd_draw_line(void);
void nwy_test_cli_lcd_draw_chinese(void);
void nwy_test_cli_lcd_close(void);
void nwy_test_cli_lcd_open_bl(void);
void nwy_test_cli_lcd_close_bl(void);
void nwy_test_cli_lcd_set_bl_level(void);

/**************************CAMERA*********************************/
void nwy_test_cli_camera_open(void);
void nwy_test_cli_camera_close(void);
void nwy_test_cli_camera_get_preview(void);
void nwy_test_cli_camera_capture(void);
/**************************SD*********************************/
void nwy_test_cli_sd_get_st(void);
void nwy_test_cli_sd_mnt(void);
void nwy_test_cli_sd_unmnt(void);
void nwy_test_cli_sd_format(void);
/**************************FLASH*********************************/
void nwy_test_cli_flash_open(void);
void nwy_test_cli_flash_erase(void);
void nwy_test_cli_flash_write(void);
void nwy_test_cli_flash_read(void);

/**************************TTS*********************************/
void nwy_test_cli_tts_input(void);
void nwy_test_cli_tts_play_start(void);
void nwy_test_cli_tts_play_stop(void);

/**************************FOTA*********************************/
void nwy_test_cli_fota_base_ver(void);
void nwy_test_cli_fota_app_ver(void);

/**************************AUDIO*********************************/
void nwy_test_cli_audio_play(void);
void nwy_test_cli_audio_rec(void);
#ifdef NWY_OPEN_AUDIO_DTMF_TEST
void nwy_test_cli_audio_dtmf(void);
void nwy_test_cli_audio_dtmf_enable();
void nwy_test_cli_audio_dtmf_disable();
#endif
#ifdef NWY_OPEN_AUDIO_SD_PLAY
void nwy_test_cli_audio_sd_play();
#endif
/**************************FS*********************************/
void nwy_test_cli_fs_open(void);
void nwy_test_cli_fs_write(void);
void nwy_test_cli_fs_read(void);
void nwy_test_cli_fs_fsize(void);
void nwy_test_cli_fs_seek(void);
void nwy_test_cli_fs_sync(void);
void nwy_test_cli_fs_fstate(void);
void nwy_test_cli_fs_trunc(void);
void nwy_test_cli_fs_close(void);
void nwy_test_cli_fs_remove(void);
void nwy_test_cli_fs_rename(void);
void nwy_test_cli_dir_open(void);
void nwy_test_cli_dir_read(void);
void nwy_test_cli_dir_tell(void);
void nwy_test_cli_dir_seek(void);
void nwy_test_cli_dir_rewind(void);
void nwy_test_cli_dir_close(void);
void nwy_test_cli_dir_mk(void);
void nwy_test_cli_dir_remove(void);
void nwy_test_cli_fs_free_size(void);
void nwy_test_cli_safe_fs_init(void);
void nwy_test_cli_safe_fs_read(void);
void nwy_test_cli_safe_fs_write(void);
void nwy_test_cli_safe_fs_fszie(void);

void nwy_test_cli_fs_ftell(void);


/**************************BLE*********************************/
void nwy_test_cli_ble_open(void);
void nwy_test_cli_ble_set_adv(void);
void nwy_test_cli_ble_send(void);
void nwy_test_cli_ble_recv(void);
void nwy_test_cli_ble_updata_connt(void);
void nwy_test_cli_ble_get_st(void);
void nwy_test_cli_ble_get_ver(void);
void nwy_test_cli_ble_set_dev_name(void);
void nwy_test_cli_ble_close(void);
void nwy_test_cli_ble_set_beacon(void);
void nwy_test_cli_ble_set_manufacture(void);
void nwy_test_cli_ble_set_srv(void);
void nwy_test_cli_ble_set_char(void);
void nwy_test_cli_ble_conn_status_report(void);
void nwy_test_cli_ble_conn_status(void);
void nwy_test_cli_ble_mac_addr(void);
void nwy_test_cli_ble_add_server(void);
void nwy_test_cli_ble_add_char(void);
void nwy_test_cli_ble_add_send_data(void);
void nwy_test_cli_ble_add_recv_data(void);
void nwy_test_cli_ble_disconnect(void);
void nwy_test_cli_ble_read_req(void);
void nwy_test_cli_ble_set_adv_server_uuid(void);
void nwy_test_cli_ble_read_rsp(void);

/**************************BLE Client*********************************/
void nwy_test_cli_ble_client_set_enable(void);
void nwy_test_cli_ble_client_scan(void);
void nwy_test_cli_ble_client_connect(void);
void nwy_test_cli_ble_client_disconnect(void);
void nwy_test_cli_ble_client_discover_srv(void);
void nwy_test_cli_ble_client_discover_char(void);
void nwy_test_cli_ble_client_send_data(void);
void nwy_test_cli_ble_client_recv_data(void);

/**************************WIFI*********************************/
void nwy_cli_test_wifi_get_st(void);
void nwy_cli_test_wifi_enable(void);
void nwy_cli_test_wifi_set_work_md(void);
void nwy_cli_test_wifi_set_ap_para(void);
void nwy_cli_test_wifi_set_ap_para_adv(void);
void nwy_cli_test_wifi_get_clit_info(void);
void nwy_cli_test_wifi_sta_scan(void);
void nwy_cli_test_wifi_sta_scan_ret(void);
void nwy_cli_test_wifi_sta_connt(void);
void nwy_cli_test_wifi_sta_disconnt(void);
void nwy_cli_test_wifi_sta_get_hostpot_info(void);
void nwy_cli_test_wifi_disable(void);

/**************************GNSS*********************************/
#ifdef NWY_OPEN_TEST_GNSS
void nwy_test_cli_gnss_open(void);
void nwy_test_cli_gnss_set_position_md(void);
void nwy_test_cli_gnss_output_format(void);
void nwy_test_cli_gnss_set_output_fmt(void);
void nwy_test_cli_gnss_delete_aiding_data(void);
void nwy_test_cli_gnss_nmea_info_parse(void);
void nwy_test_cli_gnss_nmea_data(void);
void nwy_test_cli_gnss_set_priority(void);
void nwy_test_cli_gnss_open_resume(void);
void nwy_test_cli_gnss_set_server(void);
void nwy_test_cli_gnss_open_assisted(void);
void nwy_test_cli_gnss_close(void);
#endif
#ifdef NWY_OPEN_TEST_LOCATION
void nwy_test_cli_gnss_open_base(void);
void nwy_test_cli_wifi_open_base(void);
#endif
#ifdef NWY_OPEN_TEST_GNSS_EXTERNAL_CHIP
void nwy_test_cli_gnss_open(void);
void nwy_test_cli_gnss_close(void);
void nwy_test_cli_gnss_nmea_data(void);
void nwy_test_cli_gnss_nmea_specific_data(void);
void nwy_test_cli_gnss_set_server(void);
void nwy_test_cli_gnss_open_assisted(void);
void nwy_test_cli_gnss_set_position_md(void);
void nwy_test_cli_gnss_set_updata_rate(void);
void nwy_test_cli_gnss_set_output_fmt(void);
void nwy_test_cli_gnss_set_startup_fmt(void);
#endif

#ifdef NWY_OPEN_TEST_GNSS_LOC
void nwy_test_cli_gnss_recv_cb_reg(void);
int  nwy_test_cli_gnss_open(void);
int  nwy_test_cli_gnss_close(void);
int nwy_test_cli_gnss_start_mode(void);
int nwy_test_cli_gnss_mode_set(void);
void nwy_test_cli_gnss_nmea_get(void);
void nwy_test_cli_loop_output_stop(void);

int nwy_test_cli_agps_start(void);
#endif

/**************************TCP*********************************/
void nwy_test_cli_tcp_setup(void);
void nwy_test_cli_tcp_send(void);
void nwy_test_cli_tcp_close(void);

/**************************UDP*********************************/
void nwy_test_cli_udp_setup(void);
void nwy_test_cli_udp_send(void);
void nwy_test_cli_udp_close(void);

/**************************FTP*********************************/
void nwy_test_cli_ftp_login(void);
void nwy_test_cli_ftp_get(void);
void nwy_test_cli_ftp_put(void);
void nwy_test_cli_ftp_fsize(void);
void nwy_test_cli_ftp_list(void);
void nwy_test_cli_ftp_delet(void);
void nwy_test_cli_ftp_logout(void);

/**************************HTTP*********************************/
void nwy_test_cli_http_setup(void);
void nwy_test_cli_http_get(void);
void nwy_test_cli_http_head(void);
void nwy_test_cli_http_post(void);
#ifdef NWY_OPEN_TEST_HTTPPUT
void nwy_test_cli_http_put(void);
#endif
void nwy_test_cli_http_close(void);
void nwy_test_cli_https_add_cert(void);
void nwy_test_cli_https_check_cert(void);
void nwy_test_cli_https_delet_cert(void);
void nwy_test_cli_ntp_time(void);
void nwy_test_cli_ping(void);

#ifdef NWY_OPEN_TEST_LITE_RWS

/**************************websocket*********************************/
void nwy_test_cli_websocket_setup(void);
void nwy_test_cli_websocket_send(void);
void nwy_test_cli_websocket_close(void);
void nwy_test_cli_websocket_memckeck(void);
#endif

/**************************ALI MQTT*********************************/
void nwy_test_cli_alimqtt_connect(void);
void nwy_test_cli_alimqtt_pub(void);
void nwy_test_cli_alimqtt_sub(void);
void nwy_test_cli_alimqtt_unsub(void);
void nwy_test_cli_alimqtt_state(void);
void nwy_test_cli_alimqtt_disconnect(void);
void nwy_test_cli_alimqtt_del_kv(void);

/**************************MQTT*********************************/
void nwy_test_cli_mqtt_connect(void);
void nwy_test_cli_mqtt_pub(void);
void nwy_test_cli_mqtt_sub(void);
void nwy_test_cli_mqtt_unsub(void);
void nwy_test_cli_mqtt_state(void);
void nwy_test_cli_mqtt_disconnect(void);
void nwy_test_cli_mqtt_pub_test(void);

/**************************OS & Dev API************************/
void nwy_test_cli_get_model(void);
void nwy_test_cli_get_imei(void);
void nwy_test_cli_get_chipid(void);
void nwy_test_cli_get_boot_cause(void);
void nwy_test_cli_get_sw_ver(void);
void nwy_test_cli_get_hw_ver(void);
void nwy_test_cli_get_heap_info(void);
void nwy_test_cli_get_cpu_temp(void);
void nwy_test_cli_set_app_version(void);
void nwy_test_cli_get_secureboot_state(void);
void nwy_test_cli_get_curthread_freestack(void);


void nwy_test_cli_start_timer(void);
void nwy_test_cli_stop_timer(void);
void nwy_test_cli_get_time(void);
void nwy_test_cli_set_time(void);
void nwy_test_cli_set_semp(void);
void nwy_test_cli_get_thread_list(void);
void nwy_test_cli_get_thread_runtime(void);

void nwy_test_cli_send_virt_at(void);
void nwy_test_cli_reg_at_fwd(void);

#ifdef NWY_AWS_MQTT
void nwy_aws_mqtt_connect(void);
int nwy_aws_mqtt_publish(void);
int nwy_aws_mqtt_subscribe(void);
int nwy_aws_mqtt_disconnect(void);
int nwy_aws_mqtt_unsubcribe(void);
int nwy_aws_mqtt_status_get(void);
#endif
#ifdef FEATURE_NEOWAY_PIPECLOUD
void nwy_pipecloud_set_info(void);
int nwy_pipecloud_start(void);
int nwy_pipecloud_get_status(void);
#endif
#endif
