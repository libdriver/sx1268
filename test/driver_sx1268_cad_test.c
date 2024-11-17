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
 * @file      driver_sx1268_cad_test.c
 * @brief     driver sx1268 cad test source file
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

#include "driver_sx1268_cad_test.h"

static sx1268_handle_t gs_handle;        /**< sx1268 handle */

/**
 * @brief  sx1268 cad test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t sx1268_cad_test_irq_handler(void)
{
    if (sx1268_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  send test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t sx1268_cad_test(void)
{
    uint8_t res;
    uint32_t reg;
    uint8_t modulation;
    uint8_t config;
    sx1268_bool_t enable;
    
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
    
    /* start cad test */
    sx1268_interface_debug_print("sx1268: start cad test.\n");
    
    /* init the sx1268 */
    res = sx1268_init(&gs_handle);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: init failed.\n");
       
        return 1;
    }
    
    /* enter standby */
    res = sx1268_set_standby(&gs_handle, SX1268_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set standby failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable stop timer on preamble */
    res = sx1268_set_stop_timer_on_preamble(&gs_handle, SX1268_BOOL_FALSE);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: stop timer on preamble failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dc dc ldo */
    res = sx1268_set_regulator_mode(&gs_handle, SX1268_REGULATOR_MODE_DC_DC_LDO);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set regulator mode failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set +17dBm power */
    res = sx1268_set_pa_config(&gs_handle, 0x02, 0x03);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set pa config failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enter to stdby rc mode */
    res = sx1268_set_rx_tx_fallback_mode(&gs_handle, SX1268_RX_TX_FALLBACK_MODE_STDBY_XOSC);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set rx tx fallback mode failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dio irq */
    res = sx1268_set_dio_irq_params(&gs_handle, 0x03FF, 0x03FF, 0x0000, 0x0000);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set dio irq params failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear irq status */
    res = sx1268_clear_irq_status(&gs_handle, 0x03FF);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: clear irq status failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lora mode */
    res = sx1268_set_packet_type(&gs_handle, SX1268_PACKET_TYPE_LORA);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set packet type failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* +17dBm */
    res = sx1268_set_tx_params(&gs_handle, 17, SX1268_RAMP_TIME_10US);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set tx params failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* sf9, 125khz, cr4/5, disable low data rate optimize */
    res = sx1268_set_lora_modulation_params(&gs_handle, SX1268_LORA_SF_9, SX1268_LORA_BANDWIDTH_125_KHZ, 
                                            SX1268_LORA_CR_4_5, SX1268_BOOL_FALSE);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set lora modulation params failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert the frequency */
    res = sx1268_frequency_convert_to_register(&gs_handle, 480100000, (uint32_t *)&reg);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: convert to register failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the frequency */
    res = sx1268_set_rf_frequency(&gs_handle, reg);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set rf frequency failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set base address */
    res = sx1268_set_buffer_base_address(&gs_handle, 0x00, 0x00);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set buffer base address failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 1 lora symb num */
    res = sx1268_set_lora_symb_num_timeout(&gs_handle, 0);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set lora symb num timeout failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* reset stats */
    res = sx1268_reset_stats(&gs_handle, 0x0000, 0x0000, 0x0000);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: reset stats failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear device errors */
    res = sx1268_clear_device_errors(&gs_handle);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: clear device errors failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the lora sync word */
    res = sx1268_set_lora_sync_word(&gs_handle, 0x1424);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set lora sync word failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get tx modulation */
    res = sx1268_get_tx_modulation(&gs_handle, (uint8_t *)&modulation);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get tx modulation failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    modulation |= 0x04;
    
    /* set the tx modulation */
    res = sx1268_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set tx modulation failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx gain */
    res = sx1268_set_rx_gain(&gs_handle, 0x94);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set rx gain failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the ocp */
    res = sx1268_set_ocp(&gs_handle, 0x38);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set ocp failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get the tx clamp config */
    res = sx1268_get_tx_clamp_config(&gs_handle, (uint8_t *)&config);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: get tx clamp config failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    config |= 0x1E;
    
    /* set the tx clamp config */
    res = sx1268_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set tx clamp config failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set cad params */
    res = sx1268_set_cad_params(&gs_handle, SX1268_LORA_CAD_SYMBOL_NUM_2,
                                24, 10, SX1268_LORA_CAD_EXIT_MODE_ONLY,
                                0);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: set cad params failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* run the cad */
    res = sx1268_lora_cad(&gs_handle, &enable);
    if (res != 0)
    {
        sx1268_interface_debug_print("sx1268: lora cad failed.\n");
        (void)sx1268_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output the result */
    if (enable == SX1268_BOOL_TRUE)
    {
        sx1268_interface_debug_print("sx1268: cad detected.\n");
    }
    else
    {
        sx1268_interface_debug_print("sx1268: cad not detected.\n");
    }
    
    /* finish cad test */
    sx1268_interface_debug_print("sx1268: finish cad test.\n");
    
    /* deinit */
    (void)sx1268_deinit(&gs_handle);
    
    return 0;
}
