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
 * @file      main.c
 * @brief     main source file
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
#include "driver_sx1268_sent_receive_test.h"
#include "driver_sx1268_cad_test.h"
#include "driver_sx1268_lora.h"
#include "gpio.h"
#include <stdlib.h>

uint8_t g_flag;                            /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */
static uint8_t gs_rx_done;                 /**< rx done */

/**
 * @brief     interface receive callback
 * @param[in] type is the receive callback type
 * @param[in] *buf points to a buffer address
 * @param[in] len is the buffer length
 * @note      none
 */
static void a_callback(uint16_t type, uint8_t *buf, uint16_t len)
{
    switch (type)
    {
        case SX1268_IRQ_TX_DONE :
        {
            sx1268_interface_debug_print("sx1268: irq tx done.\n");
            
            break;
        }
        case SX1268_IRQ_RX_DONE :
        {
            uint16_t i;
            sx1268_bool_t enable;
            float rssi;
            float snr;
            
            sx1268_interface_debug_print("sx1268: irq rx done.\n");
            
            /* get the status */
            if (sx1268_lora_get_status((float *)&rssi, (float *)&snr) != 0)
            {
                return;
            }
            sx1268_interface_debug_print("sx1268: rssi is %0.1f.\n", rssi);
            sx1268_interface_debug_print("sx1268: snr is %0.2f.\n", snr);
            
            /* check the error */
            if (sx1268_lora_check_packet_error(&enable) != 0)
            {
                return;
            }
            if ((enable == SX1268_BOOL_FALSE) && len)
            {
                for (i = 0; i < len; i++)
                {
                    sx1268_interface_debug_print("%c", buf[i]);
                }
                sx1268_interface_debug_print("\n");
                gs_rx_done = 1;
            }
            
            break;
        }
        case SX1268_IRQ_PREAMBLE_DETECTED :
        {
            sx1268_interface_debug_print("sx1268: irq preamble detected.\n");
            
            break;
        }
        case SX1268_IRQ_SYNC_WORD_VALID :
        {
            sx1268_interface_debug_print("sx1268: irq valid sync word detected.\n");
            
            break;
        }
        case SX1268_IRQ_HEADER_VALID :
        {
            sx1268_interface_debug_print("sx1268: irq valid header.\n");
            
            break;
        }
        case SX1268_IRQ_HEADER_ERR :
        {
            sx1268_interface_debug_print("sx1268: irq header error.\n");
            
            break;
        }
        case SX1268_IRQ_CRC_ERR :
        {
            sx1268_interface_debug_print("sx1268: irq crc error.\n");
            
            break;
        }
        case SX1268_IRQ_CAD_DONE :
        {
            sx1268_interface_debug_print("sx1268: irq cad done.\n");
            
            break;
        }
        case SX1268_IRQ_CAD_DETECTED :
        {
            sx1268_interface_debug_print("sx1268: irq cad detected.\n");
            
            break;
        }
        case SX1268_IRQ_TIMEOUT :
        {
            sx1268_interface_debug_print("sx1268: irq timeout.\n");
            
            break;
        }
        default :
        {
            sx1268_interface_debug_print("sx1268: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief     sx1268 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t sx1268(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            sx1268_info_t info;
            
            /* print sx1268 info */
            sx1268_info(&info);
            sx1268_interface_debug_print("sx1268: chip is %s.\n", info.chip_name);
            sx1268_interface_debug_print("sx1268: manufacturer is %s.\n", info.manufacturer_name);
            sx1268_interface_debug_print("sx1268: interface is %s.\n", info.interface);
            sx1268_interface_debug_print("sx1268: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000)/100);
            sx1268_interface_debug_print("sx1268: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            sx1268_interface_debug_print("sx1268: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            sx1268_interface_debug_print("sx1268: max current is %0.2fmA.\n", info.max_current_ma);
            sx1268_interface_debug_print("sx1268: max temperature is %0.1fC.\n", info.temperature_max);
            sx1268_interface_debug_print("sx1268: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            sx1268_interface_debug_print("sx1268: SCK connected to GPIO11(BCM).\n");
            sx1268_interface_debug_print("sx1268: MISO connected to GPIO9(BCM).\n");
            sx1268_interface_debug_print("sx1268: MOSI connected to GPIO10(BCM).\n");
            sx1268_interface_debug_print("sx1268: CS connected to GPIO8(BCM).\n");
            sx1268_interface_debug_print("sx1268: RST connected to GPIO27(BCM).\n");
            sx1268_interface_debug_print("sx1268: DIO1 connected to GPIO22(BCM).\n");
            sx1268_interface_debug_print("sx1268: BUSY connected to GPIO17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show sx1268 help */
            
            help:
            sx1268_interface_debug_print("sx1268 -i\n\tshow sx1268 chip and driver information.\n");
            sx1268_interface_debug_print("sx1268 -h\n\tshow sx1268 help.\n");
            sx1268_interface_debug_print("sx1268 -p\n\tshow sx1268 pin connections of the current board.\n");
            sx1268_interface_debug_print("sx1268 -t reg\n\trun sx1268 register test.\n");
            sx1268_interface_debug_print("sx1268 -t sent -lora\n\trun sx1268 sent test.\n");
            sx1268_interface_debug_print("sx1268 -t receive -lora <timeout>\n\trun sx1268 receive test."
                                         "timeout is the received timeout in second.\n");
            sx1268_interface_debug_print("sx1268 -t cad -lora\n\trun sx1268 cad test.\n");
            sx1268_interface_debug_print("sx1268 -c sent -lora <data>\n\trun sx1268 sent function.data is the send data.\n");
            sx1268_interface_debug_print("sx1268 -c receive -lora <timeout>\n\trun sx1268 receive function."
                                         "timeout is the received timeout in second.\n");
            sx1268_interface_debug_print("sx1268 -c cad -lora\n\trun sx1268 cad function.\n");
            sx1268_interface_debug_print("sx1268 -c sleep -lora\n\trun sx1268 sleep function.\n");
            sx1268_interface_debug_print("sx1268 -c wake_up -lora\n\trun sx1268 wake up function.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                uint8_t res;
                
                res = sx1268_register_test();
                if (res != 0)
                {
                    return 1;
                }
                
                return 0;
            }
            
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* sent test */
            if (strcmp("sent", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    g_gpio_irq = sx1268_interrupt_test_irq_handler;
                    res = sx1268_sent_test();
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* cad test */
            else if (strcmp("cad", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    g_gpio_irq = sx1268_cad_test_irq_handler;
                    res = sx1268_cad_test();
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
            /* run cad */
            if (strcmp("cad", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    sx1268_bool_t enable;
                    
                    /* gpio init */
                    res = gpio_interrupt_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    g_gpio_irq = sx1268_lora_irq_handler;
                    
                    /* lora init */
                    res = sx1268_lora_init(a_callback);
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    /* run cad */
                    res = sx1268_lora_run_cad(&enable);
                    if (res != 0)
                    {
                        (void)sx1268_lora_deinit();
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
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
                    
                    /* deinit */
                    res = sx1268_lora_deinit();
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* sleep */
            else if (strcmp("sleep", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    
                    /* lora init */
                    res = sx1268_lora_init(a_callback);
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* sleep */
                    if (sx1268_lora_sleep() != 0)
                    {
                        return 1;
                    }
                    
                    sx1268_interface_debug_print("sx1268: sleep.\n");
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* wake up */
            else if (strcmp("wake_up", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    
                    /* lora init */
                    res = sx1268_lora_init(a_callback);
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* wake up */
                    if (sx1268_lora_wake_up() != 0)
                    {
                        return 1;
                    }
                    
                    sx1268_interface_debug_print("sx1268: wake up.\n");
                    
                    /* deinit */
                    res = sx1268_lora_deinit();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* receive test */
            if (strcmp("receive", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    g_gpio_irq = sx1268_interrupt_test_irq_handler;
                    res = sx1268_receive_test(atoi(argv[4]));
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
            /* sent function */
            if (strcmp("sent", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    
                    /* gpio init */
                    res = gpio_interrupt_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    g_gpio_irq = sx1268_lora_irq_handler;
                    
                    /* lora init */
                    res = sx1268_lora_init(a_callback);
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    /* set sent mode */
                    res = sx1268_lora_set_sent_mode();
                    if (res != 0)
                    {
                        (void)sx1268_lora_deinit();
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    sx1268_interface_debug_print("sx1268: sent %s.\n", argv[4]);
                    
                    /* sent data */
                    res = sx1268_lora_sent((uint8_t *)argv[4], (uint16_t)strlen(argv[4]));
                    if (res != 0)
                    {
                        (void)sx1268_lora_deinit();
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    /* deinit */
                    res = sx1268_lora_deinit();
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* receive function */
            else if (strcmp("receive", argv[2]) == 0)
            {
                /* lora type */
                if (strcmp("-lora", argv[3]) == 0)
                {
                    uint8_t res;
                    uint32_t timeout;
                    
                    /* gpio init */
                    res = gpio_interrupt_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    g_gpio_irq = sx1268_lora_irq_handler;
                    
                    /* lora init */
                    res = sx1268_lora_init(a_callback);
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    /* start receiving */
                    sx1268_interface_debug_print("sx1268: start receiving...\n");
                    gs_rx_done = 0;
                    timeout = atoi(argv[4]);
                    
                    /* start receive */
                    res = sx1268_lora_set_continuous_receive_mode();
                    if (res != 0)
                    {
                        (void)sx1268_lora_deinit();
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    while ((timeout != 0) && (gs_rx_done == 0))
                    {
                        timeout--;
                        sx1268_interface_delay_ms(1000);
                    }
                    if (gs_rx_done == 0)
                    {
                        /* receive timeout */
                        sx1268_interface_debug_print("sx1268: receive timeout.\n");
                        (void)sx1268_lora_deinit();
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    /* deinit */
                    res = sx1268_lora_deinit();
                    if (res != 0)
                    {
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = sx1268(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        sx1268_interface_debug_print("sx1268: run failed.\n");
    }
    else if (res == 5)
    {
        sx1268_interface_debug_print("sx1268: param is invalid.\n");
    }
    else
    {
        sx1268_interface_debug_print("sx1268: unknow status code.\n");
    }
    
    return 0;
}
