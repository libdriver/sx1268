/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sx1268_register_test.c
 * @brief     driver sx1268 register test source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-01-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/01/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_sx1268_register_test.h"
#include <stdlib.h>

static sx1268_handle_t gs_handle;        /**< sx1268 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t sx1268_register_test(void)
{
    uint8_t res;
    uint8_t addr, addr_check;
    uint8_t i;
    uint16_t value, value_check;
    uint8_t sync_word[8];
    uint8_t sync_word_check[8];
    uint8_t setup, setup_check;
    uint32_t rand_number;
    uint8_t modulation, modulation_check;
    uint8_t gain, gain_check;
    uint8_t config, config_check;
    uint8_t ocp, ocp_check;
    uint8_t control, control_check;
    uint8_t trim, trim_check;
    uint8_t mask, mask_check;
    uint8_t enable, enable_check;
    double us, us_check;
    uint32_t freq, freq_check;
    uint32_t br, br_check;
    uint32_t reg;
    sx1268_packet_type_t type;
    sx1268_info_t info;
    
    /* link interface function */
    DRIVER_SX1268_LINK_INIT(&gs_handle, sx1268_handle_t);
    DRIVER_SX1268_LINK_SPI_INIT(&gs_handle, sx1268_interface_spi_init);
    DRIVER_SX1268_LINK_SPI_DEINIT(&gs_handle, sx1268_interface_spi_deinit);
    DRIVER_SX1268_LINK_SPI_WRITE_READ(&gs_handle, sx1268_interface_spi_write_read);
    DRIVER_SX1268_LINK_RESET_GPIO_INIT(&gs_handle, sx1268_interface_reset_gpio_init);
    DRIVER_SX1268_LINK_RESET_GPIO_DEINIT(&gs_handle, sx1268_interface_reset_gpio_deinit);
    DRIVER_SX1268_LINK_RESET_GPIO_WRITE(&gs_handle, sx1268_interface_reset_gpio_write);
    DRIVER_SX1268_LINK_BUSY_GPIO_INIT(&gs_handle, sx1268_interface_busy_gpio_init);
    DRIVER_SX1268_LINK_BUSY_GPIO_DEINIT(&gs_handle, sx1268_interface_busy_gpio_deinit);
    DRIVER_SX1268_LINK_BUSY_GPIO_READ(&gs_handle, sx1268_interface_busy_gpio_read);
    DRIVER_SX1268_LINK_DELAY_MS(&gs_handle, sx1268_interface_delay_ms);
    DRIVER_SX1268_LINK_DEBUG_PRINT(&gs_handle, sx1268_interface_debug_print);
    DRIVER_SX1268_LINK_RECEIVE_CALLBACK(&gs_handle, sx1268_interface_receive_callback);
    
    /* get information */
    res = sx1268_info(&info);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        sx1268_interface_debug_print("sx1268: chip is %s.\n", info.chip_name);
        sx1268_interface_debug_print("sx1268: manufacturer is %s.\n", info.manufacturer_name);
        sx1268_interface_debug_print("sx1268: interface is %s.\n", info.interface);
        sx1268_interface_debug_print("sx1268: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sx1268_interface_debug_print("sx1268: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sx1268_interface_debug_print("sx1268: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sx1268_interface_debug_print("sx1268: max current is %0.2fmA.\n", info.max_current_ma);
        sx1268_interface_debug_print("sx1268: max temperature is %0.1fC.\n", info.temperature_max);
        sx1268_interface_debug_print("sx1268: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    sx1268_interface_debug_print("sx1268: start register test.\n");
    
    /* init the sx1268 */
    res = sx1268_init(&gs_handle);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: init failed.\n");
       
        return 1;
    }
    
    /* set standby mode */
    res = sx1268_set_standby(&gs_handle, SX1268_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set standby failed.\n");
        (void)sx1268_deinit(&gs_handle);
       
        return 1;
    }
    
    /* sx1268_write_register/sx1268_read_register test */
    sx1268_interface_debug_print("sx1268: sx1268_write_register/sx1268_read_register test.\n");
    
    control = rand() % 256;
    res = sx1268_write_register(&gs_handle, 0x06BC, (uint8_t *)&control, 1);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: write register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set register 0x%02X.\n", control);
    res = sx1268_read_register(&gs_handle, 0x06BC, (uint8_t *)&control_check, 1);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: read register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check register %s.\n", control_check == control ? "ok" : "error");
    
    /* sx1268_write_buffer/sx1268_read_buffer test */
    sx1268_interface_debug_print("sx1268: sx1268_write_buffer/sx1268_read_buffer test.\n");
    
    control = rand() % 256;
    res = sx1268_write_buffer(&gs_handle , 0, (uint8_t *)&control, 1);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: write buffer failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set buffer 0x%02X.\n", control);
    res = sx1268_read_buffer(&gs_handle, 0, (uint8_t *)&control_check, 1);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: read buffer failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check buffer %s.\n", control_check == control ? "ok" : "error");
    
    /* sx1268_timeout_convert_to_register/sx1268_timeout_convert_to_data test */
    sx1268_interface_debug_print("sx1268: sx1268_timeout_convert_to_register/sx1268_timeout_convert_to_data test.\n");
    
    us = (rand() % 10000) / 10.0;
    res = sx1268_timeout_convert_to_register(&gs_handle, us, (uint32_t *)&reg);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: timeout convert to register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set timeout %f us.\n", us);
    res = sx1268_timeout_convert_to_data(&gs_handle, reg, (double *)&us_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: timeout convert to data failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check timeout %f us.\n", us_check);
    
    /* sx1268_frequency_convert_to_register/sx1268_frequency_convert_to_data test */
    sx1268_interface_debug_print("sx1268: sx1268_frequency_convert_to_register/sx1268_frequency_convert_to_data test.\n");
    
    freq = 47000000;
    res = sx1268_frequency_convert_to_register(&gs_handle, freq, (uint32_t *)&reg);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: frequency convert to register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set frequency %d Hz.\n", freq);
    res = sx1268_frequency_convert_to_data(&gs_handle, reg, (uint32_t *)&freq_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: frequency convert to data failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check frequency %d Hz.\n", freq_check);
    
    /* sx1268_gfsk_bit_rate_convert_to_register/sx1268_gfsk_bit_rate_convert_to_data test */
    sx1268_interface_debug_print("sx1268: sx1268_gfsk_bit_rate_convert_to_register/sx1268_gfsk_bit_rate_convert_to_data test.\n");
    
    br = 470000;
    res = sx1268_gfsk_bit_rate_convert_to_register(&gs_handle, br, (uint32_t *)&reg);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: gfsk bit rate convert to register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set bit rate %d Hz.\n", br);
    res = sx1268_gfsk_bit_rate_convert_to_data(&gs_handle, reg, (uint32_t *)&br_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: gfsk bit rate convert to data failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check bit rate %d Hz.\n", br_check);
    
    /* sx1268_gfsk_frequency_deviation_convert_to_register/sx1268_gfsk_frequency_deviation_convert_to_data test */
    sx1268_interface_debug_print("sx1268: sx1268_gfsk_frequency_deviation_convert_to_register/sx1268_gfsk_frequency_deviation_convert_to_data test.\n");
    
    freq = 10000;
    res = sx1268_gfsk_frequency_deviation_convert_to_register(&gs_handle, freq, (uint32_t *)&reg);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: gfsk frequency deviation convert to register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: gfsk frequency deviation %d Hz.\n", freq);
    res = sx1268_gfsk_frequency_deviation_convert_to_data(&gs_handle, reg, (uint32_t *)&freq_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: gfsk frequency deviation convert to data failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check gfsk frequency deviation %d Hz.\n", freq_check);
    
    /* sx1268_set_packet_type/sx1268_get_packet_type test */
    sx1268_interface_debug_print("sx1268: sx1268_set_packet_type/sx1268_get_packet_type test.\n");
    
    /* set gfsk type */
    res = sx1268_set_packet_type(&gs_handle, SX1268_PACKET_TYPE_GFSK);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set packet type failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set gfsk type.\n");
    res = sx1268_get_packet_type(&gs_handle, &type);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get packet type failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check packet type %s.\n", type == SX1268_PACKET_TYPE_GFSK ? "ok" : "error");
    
    /* set lora type */
    res = sx1268_set_packet_type(&gs_handle, SX1268_PACKET_TYPE_LORA);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set packet type failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set lora type.\n");
    res = sx1268_get_packet_type(&gs_handle, &type);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get packet type failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check packet type %s.\n", type == SX1268_PACKET_TYPE_LORA ? "ok" : "error");
    
    /* sx1268_set_fsk_whitening_initial_value/sx1268_get_fsk_whitening_initial_value test */
    sx1268_interface_debug_print("sx1268: sx1268_set_fsk_whitening_initial_value/sx1268_get_fsk_whitening_initial_value test.\n");
    
    value = 0x0100;
    res = sx1268_set_fsk_whitening_initial_value(&gs_handle, value);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set fsk whitening initial value failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set fsk whitening initial value 0x%04X.\n", value);
    res = sx1268_get_fsk_whitening_initial_value(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get fsk whitening initial value failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check fsk whitening initial value %s.\n", value_check == value ? "ok" : "error");
    
    /* sx1268_set_fsk_crc_initical_value/sx1268_get_fsk_crc_initical_value test */
    sx1268_interface_debug_print("sx1268: sx1268_set_fsk_crc_initical_value/sx1268_get_fsk_crc_initical_value test.\n");
    
    value = rand() % 65536;
    res = sx1268_set_fsk_crc_initical_value(&gs_handle, value);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set fsk crc initical value failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set fsk crc initical value 0x%04X.\n", value);
    res = sx1268_get_fsk_crc_initical_value(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get fsk crc initical value failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check fsk crc initical value %s.\n", value_check == value ? "ok" : "error");
    
    /* sx1268_set_fsk_crc_initical_value/sx1268_get_fsk_crc_initical_value test */
    sx1268_interface_debug_print("sx1268: sx1268_set_fsk_crc_initical_value/sx1268_get_fsk_crc_initical_value test.\n");
    
    value = rand() % 65536;
    res = sx1268_set_fsk_crc_polynomial_value(&gs_handle, value);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set fsk crc polynomial value failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set fsk crc polynomial value 0x%04X.\n", value);
    res = sx1268_get_fsk_crc_polynomial_value(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get fsk crc polynomial value failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check fsk crc polynomial value %s.\n", value_check == value ? "ok" : "error");
    
    /* sx1268_set_fsk_sync_word/sx1268_get_fsk_sync_word test */
    sx1268_interface_debug_print("sx1268: sx1268_set_fsk_sync_word/sx1268_get_fsk_sync_word test.\n");
    
    for (i = 0; i < 8; i++)
    {
        sync_word[i] = rand() %256;
    }
    res = sx1268_set_fsk_sync_word(&gs_handle, (uint8_t *)sync_word);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set fsk sync word failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set fsk sync word 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                 sync_word[0], sync_word[1], sync_word[2], sync_word[3],
                                 sync_word[4], sync_word[5], sync_word[6], sync_word[7]);
    res = sx1268_get_fsk_sync_word(&gs_handle, (uint8_t *)sync_word_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get fsk sync word failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check fsk sync word %s.\n", memcmp((uint8_t *)sync_word, (uint8_t *)sync_word_check, 8) == 0 ? "ok" : "error");
    
    /* sx1268_set_fsk_node_address/sx1268_get_fsk_node_address test */
    sx1268_interface_debug_print("sx1268: sx1268_set_fsk_node_address/sx1268_get_fsk_node_address test.\n");
    
    addr = rand() %256;
    res = sx1268_set_fsk_node_address(&gs_handle, addr);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set fsk node address failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set fsk node address 0x%02X.\n", addr);
    res = sx1268_get_fsk_node_address(&gs_handle, (uint8_t *)&addr_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get fsk node address failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check fsk node address %s.\n", addr == addr_check ? "ok" : "error");
    
    /* sx1268_set_fsk_broadcast_address/sx1268_get_fsk_broadcast_address test */
    sx1268_interface_debug_print("sx1268: sx1268_set_fsk_broadcast_address/sx1268_get_fsk_broadcast_address test.\n");
    
    addr = rand() %256;
    res = sx1268_set_fsk_broadcast_address(&gs_handle, addr);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set fsk broadcast address failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set fsk broadcast address 0x%02X.\n", addr);
    res = sx1268_get_fsk_broadcast_address(&gs_handle, (uint8_t *)&addr_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get fsk broadcast address failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check fsk broadcast address %s.\n", addr == addr_check ? "ok" : "error");
    
    /* sx1268_set_iq_polarity/sx1268_get_iq_polarity test */
    sx1268_interface_debug_print("sx1268: sx1268_set_iq_polarity/sx1268_get_iq_polarity test.\n");
    
    setup = rand() %256;
    res = sx1268_set_iq_polarity(&gs_handle, setup);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set iq polarity failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set iq polarity 0x%02X.\n", setup);
    res = sx1268_get_iq_polarity(&gs_handle, (uint8_t *)&setup_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get iq polarity failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check iq polarity %s.\n", setup == setup_check ? "ok" : "error");
    
    /* sx1268_set_lora_sync_word/sx1268_get_lora_sync_word test */
    sx1268_interface_debug_print("sx1268: sx1268_set_lora_sync_word/sx1268_get_lora_sync_word test.\n");
    
    value = rand() % 65536;
    res = sx1268_set_lora_sync_word(&gs_handle, value);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set lora sync word failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set lora sync word 0x%04X.\n", value);
    res = sx1268_get_lora_sync_word(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get lora sync word failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check lora sync word %s.\n", value == value_check ? "ok" : "error");
    
    /* sx1268_get_random_number test */
    sx1268_interface_debug_print("sx1268: sx1268_get_random_number test.\n");
    
    res = sx1268_get_random_number(&gs_handle, (uint32_t *)&rand_number);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get random number failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: random number is 0x%08X.\n", rand_number);
    
    /* sx1268_set_tx_modulation/sx1268_get_tx_modulation test */
    sx1268_interface_debug_print("sx1268: sx1268_set_tx_modulation/sx1268_get_tx_modulation test.\n");
    
    res = sx1268_get_tx_modulation(&gs_handle, (uint8_t *)&modulation_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get tx modulation failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    modulation = 0;
    modulation |= 0x04;
    res = sx1268_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set tx modulation failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set tx modulation 0x%02X.\n", modulation);
    res = sx1268_get_tx_modulation(&gs_handle, (uint8_t *)&modulation_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get tx modulation failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check tx modulation %s.\n", modulation == modulation_check ? "ok" : "error");
    
    /* sx1268_set_rx_gain/sx1268_get_rx_gain test */
    sx1268_interface_debug_print("sx1268: sx1268_set_rx_gain/sx1268_get_rx_gain test.\n");
    
    gain = rand() % 256;
    res = sx1268_set_rx_gain(&gs_handle, gain);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set rx gain failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set rx gain 0x%02X.\n", gain);
    res = sx1268_get_rx_gain(&gs_handle, (uint8_t *)&gain_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get rx gain failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check rx gain %s.\n", gain == gain_check ? "ok" : "error");
    
    /* sx1268_set_tx_clamp_config/sx1268_get_tx_clamp_config test */
    sx1268_interface_debug_print("sx1268: sx1268_set_tx_clamp_config/sx1268_get_tx_clamp_config test.\n");
    
    config = rand() % 256;
    res = sx1268_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set tx clamp config failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set tx clamp config 0x%02X.\n", config);
    res = sx1268_get_tx_clamp_config(&gs_handle, (uint8_t *)&config_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get tx clamp config failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check tx clamp config %s.\n", config == config_check ? "ok" : "error");
    
    /* sx1268_set_ocp/sx1268_get_ocp test */
    sx1268_interface_debug_print("sx1268: sx1268_set_ocp/sx1268_get_ocp test.\n");
    
    ocp = rand() % 0x20;
    res = sx1268_set_ocp(&gs_handle, ocp);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set ocp failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set ocp 0x%02X.\n", ocp);
    res = sx1268_get_ocp(&gs_handle, (uint8_t *)&ocp_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get ocp failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check ocp config %s.\n", ocp == ocp_check ? "ok" : "error");
    
    /* sx1268_set_rtc_control/sx1268_get_rtc_control test */
    sx1268_interface_debug_print("sx1268: sx1268_set_rtc_control/sx1268_get_rtc_control test.\n");
    
    /* disable */
    control = 0;
    res = sx1268_set_rtc_control(&gs_handle, control);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set rtc control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: disable rtc control.\n");
    res = sx1268_get_rtc_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get rtc control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check rtc control %s.\n", control_check == control ? "ok" : "error");
    
    /* sx1268_set_xta_trim/sx1268_get_xta_trim test */
    sx1268_interface_debug_print("sx1268: sx1268_set_xta_trim/sx1268_get_xta_trim test.\n");
    
    trim = 0x05;
    res = sx1268_set_xta_trim(&gs_handle, trim);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set xta trim failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set xta trim 0x%02X.\n", trim);
    res = sx1268_get_xta_trim(&gs_handle, (uint8_t *)&trim_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get xta trim failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check xta trim %s.\n", trim_check == trim ? "ok" : "error");
    
    /* sx1268_set_xtb_trim/sx1268_get_xtb_trim test */
    sx1268_interface_debug_print("sx1268: sx1268_set_xtb_trim/sx1268_get_xtb_trim test.\n");
    
    trim = 0x05;
    res = sx1268_set_xtb_trim(&gs_handle, trim);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set xtb trim failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set xtb trim 0x%02X.\n", trim);
    res = sx1268_get_xtb_trim(&gs_handle, (uint8_t *)&trim_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get xtb trim failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check xtb trim %s.\n", trim_check == trim ? "ok" : "error");
    
    /* sx1268_set_dio3_output_control/sx1268_get_dio3_output_control test */
    sx1268_interface_debug_print("sx1268: sx1268_set_dio3_output_control/sx1268_get_dio3_output_control test.\n");
    
    control = 0x01;
    res = sx1268_set_dio3_output_control(&gs_handle, control);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set dio3 output control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set dio3 output control 0x%02X.\n", control);
    res = sx1268_get_dio3_output_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get dio3 output control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check dio3 output control %s.\n", control_check == control ? "ok" : "error");
    
    /* sx1268_set_event_mask/sx1268_get_event_mask test */
    sx1268_interface_debug_print("sx1268: sx1268_set_event_mask/sx1268_get_event_mask test.\n");
    
    mask = 0;
    res = sx1268_set_event_mask(&gs_handle, mask);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set event mask failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set event mask 0x%02X.\n", mask);
    res = sx1268_get_event_mask(&gs_handle, (uint8_t *)&mask_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get event mask failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check event mask %s.\n", mask_check == mask ? "ok" : "error");
    
    /* sx1268_set_dio_output_enable/sx1268_get_dio_output_enable test */
    sx1268_interface_debug_print("sx1268: sx1268_set_dio_output_enable/sx1268_get_dio_output_enable test.\n");
    
    enable = rand() % 0xF;
    res = sx1268_set_dio_output_enable(&gs_handle, enable);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set dio output enable failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set dio output enable 0x%02X.\n", enable);
    res = sx1268_get_dio_output_enable(&gs_handle, (uint8_t *)&enable_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get dio output enable failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check dio output enable %s.\n", enable == enable_check ? "ok" : "error");
    
    /* sx1268_set_dio_input_enable/sx1268_get_dio_input_enable test */
    sx1268_interface_debug_print("sx1268: sx1268_set_dio_input_enable/sx1268_get_dio_input_enable test.\n");
    
    res = sx1268_set_dio_output_enable(&gs_handle, 0x00);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set dio output enable failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    enable = rand() % 0xF;
    res = sx1268_set_dio_input_enable(&gs_handle, enable);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set dio input enable failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set dio input enable 0x%02X.\n", enable);
    res = sx1268_get_dio_input_enable(&gs_handle, (uint8_t *)&enable_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get dio input enable failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check dio input enable %s.\n", enable == enable_check ? "ok" : "error");
    
    /* sx1268_set_pull_up_control/sx1268_get_pull_up_control test */
    sx1268_interface_debug_print("sx1268: sx1268_set_pull_up_control/sx1268_get_pull_up_control test.\n");
    
    control = rand() % 0x0F;
    res = sx1268_set_pull_up_control(&gs_handle, control);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set pull up control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set pull up control 0x%02X.\n", control);
    res = sx1268_get_pull_up_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get pull up control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check pull up control %s.\n", control == control_check ? "ok" : "error");
    
    /* sx1268_set_pull_down_control/sx1268_get_pull_down_control test */
    sx1268_interface_debug_print("sx1268: sx1268_set_pull_down_control/sx1268_get_pull_down_control test.\n");
    
    res = sx1268_set_pull_up_control(&gs_handle, 0x00);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set pull up control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    control = rand() % 0x0F;
    res = sx1268_set_pull_down_control(&gs_handle, control);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set pull down control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: set pull down control 0x%02X.\n", control);
    res = sx1268_get_pull_down_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get pull down control failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    sx1268_interface_debug_print("sx1268: check pull down control %s.\n", control == control_check ? "ok" : "error");
    
    /* finish register test */
    sx1268_interface_debug_print("sx1268: finish register test.\n");
    (void)sx1268_deinit(&gs_handle);
    
    return 0;
}
