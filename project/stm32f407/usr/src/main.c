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
#include "driver_sx1268_send_receive_test.h"
#include "driver_sx1268_cad_test.h"
#include "driver_sx1268_lora.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */
static volatile uint8_t gs_rx_done;        /**< rx done */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

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
uint8_t sx12682(uint8_t argc, char **argv)
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
            sx1268_interface_debug_print("sx1268: SCK connected to GPIOA PIN5.\n");
            sx1268_interface_debug_print("sx1268: MISO connected to GPIOA PIN6.\n");
            sx1268_interface_debug_print("sx1268: MOSI connected to GPIOA PIN7.\n");
            sx1268_interface_debug_print("sx1268: CS connected to GPIOA PIN4.\n");
            sx1268_interface_debug_print("sx1268: RST connected to GPIOB PIN1.\n");
            sx1268_interface_debug_print("sx1268: DIO1 connected to GPIOB PIN0.\n");
            sx1268_interface_debug_print("sx1268: BUSY connected to GPIOA PIN8.\n");
            
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
            sx1268_interface_debug_print("sx1268 -t send -lora\n\trun sx1268 send test.\n");
            sx1268_interface_debug_print("sx1268 -t receive -lora <timeout>\n\trun sx1268 receive test."
                                         "timeout is the received timeout in second.\n");
            sx1268_interface_debug_print("sx1268 -t cad -lora\n\trun sx1268 cad test.\n");
            sx1268_interface_debug_print("sx1268 -c send -lora <data>\n\trun sx1268 send function.data is the send data.\n");
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
            /* send test */
            if (strcmp("send", argv[2]) == 0)
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
                    res = sx1268_send_test();
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
            /* send function */
            if (strcmp("send", argv[2]) == 0)
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
                    
                    /* set send mode */
                    res = sx1268_lora_set_send_mode();
                    if (res != 0)
                    {
                        (void)sx1268_lora_deinit();
                        (void)gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    sx1268_interface_debug_print("sx1268: send %s.\n", argv[4]);
                    
                    /* send data */
                    res = sx1268_lora_send((uint8_t *)argv[4], (uint16_t)strlen(argv[4]));
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
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"data", required_argument, NULL, 1},
        {"timeout", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char data[33] = "LibDriver";
    uint32_t timeout = 1000;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* data */
            case 1 :
            {
                /* set the data */
                memset(data, 0, sizeof(char) * 33);
                strncpy(data, optarg, 32);

                break;
            }
            
            /* timeout */
            case 2 :
            {
                /* set the timeout */
                timeout = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run reg test */
        res = sx1268_register_test();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_lora-send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = sx1268_interrupt_test_irq_handler;
        
        /* run send test */
        res = sx1268_send_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_lora-receive", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = sx1268_interrupt_test_irq_handler;

        /* run receive test */
        res = sx1268_receive_test(timeout);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_lora-cad", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = sx1268_cad_test_irq_handler;

        /* run cad test */
        res = sx1268_cad_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = sx1268_lora_irq_handler;
        
        /* lora init */
        res = sx1268_lora_init(a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* set send mode */
        res = sx1268_lora_set_send_mode();
        if (res != 0)
        {
            (void)sx1268_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* output */
        sx1268_interface_debug_print("sx1268: send %s.\n", data);
        
        /* send data */
        res = sx1268_lora_send((uint8_t *)data, (uint16_t)strlen(data));
        if (res != 0)
        {
            (void)sx1268_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* lora deinit */
        res = sx1268_lora_deinit();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-receive", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
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
        
        /* lora deinit */
        res = sx1268_lora_deinit();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-cad", type) == 0)
    {
        uint8_t res;
        sx1268_bool_t enable;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
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
        
        /* lora deinit */
        res = sx1268_lora_deinit();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-sleep", type) == 0)
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
        
        /* output */
        sx1268_interface_debug_print("sx1268: sleep.\n");
        
        return 0;
    }
    else if (strcmp("e_lora-wake-up", type) == 0)
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
        
        /* output */
        sx1268_interface_debug_print("sx1268: wake up.\n");
        
        /* deinit */
        res = sx1268_lora_deinit();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        sx1268_interface_debug_print("Usage:\n");
        sx1268_interface_debug_print("  sx1268 (-i | --information)\n");
        sx1268_interface_debug_print("  sx1268 (-h | --help)\n");
        sx1268_interface_debug_print("  sx1268 (-p | --port)\n");
        sx1268_interface_debug_print("  sx1268 (-t reg | --test=reg)\n");
        sx1268_interface_debug_print("  sx1268 (-t lora-send | --test=lora-send)\n");
        sx1268_interface_debug_print("  sx1268 (-t lora-receive | --test=lora-receive) [--timeout=<time>]\n");
        sx1268_interface_debug_print("  sx1268 (-t lora-cad | --test=lora-cad)\n");
        sx1268_interface_debug_print("  sx1268 (-e lora-send | --example=lora-send) [--data=<str>]\n");
        sx1268_interface_debug_print("  sx1268 (-e lora-receive | --example=lora-receive) [--timeout=<time>]\n");
        sx1268_interface_debug_print("  sx1268 (-e lora-cad | --example=lora-cad)\n");
        sx1268_interface_debug_print("  sx1268 (-e lora-sleep | --example=lora-sleep)\n");
        sx1268_interface_debug_print("  sx1268 (-e lora-wake-up | --example=lora-wake-up)\n");
        sx1268_interface_debug_print("\n");
        sx1268_interface_debug_print("Options:\n");
        sx1268_interface_debug_print("      --data=<str>            Set the send data.([default: LibDriver])\n");
        sx1268_interface_debug_print("  -e <lora-send | lora-receive | lora-cad | lora-sleep | lora-wake-up>, --example=<lora-send\n");
        sx1268_interface_debug_print("     | lora-receive | lora-cad | lora-sleep | lora-wake-up>\n");
        sx1268_interface_debug_print("                              Run the driver example.\n");
        sx1268_interface_debug_print("  -h, --help                  Show the help.\n");
        sx1268_interface_debug_print("  -i, --information           Show the chip information.\n");
        sx1268_interface_debug_print("  -p, --port                  Display the pin connections of the current board.\n");
        sx1268_interface_debug_print("  -t <reg | lora-send | lora-receive | lora-cad>, --test=<reg | lora-send | lora-receive | lora-cad>\n");
        sx1268_interface_debug_print("                              Run the driver test.\n");
        sx1268_interface_debug_print("      --timeout=<time>        Set the timeout in ms.([default: 1000])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        sx1268_info_t info;
        
        /* print sx1268 info */
        sx1268_info(&info);
        sx1268_interface_debug_print("sx1268: chip is %s.\n", info.chip_name);
        sx1268_interface_debug_print("sx1268: manufacturer is %s.\n", info.manufacturer_name);
        sx1268_interface_debug_print("sx1268: interface is %s.\n", info.interface);
        sx1268_interface_debug_print("sx1268: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sx1268_interface_debug_print("sx1268: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sx1268_interface_debug_print("sx1268: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sx1268_interface_debug_print("sx1268: max current is %0.2fmA.\n", info.max_current_ma);
        sx1268_interface_debug_print("sx1268: max temperature is %0.1fC.\n", info.temperature_max);
        sx1268_interface_debug_print("sx1268: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        sx1268_interface_debug_print("sx1268: SCK connected to GPIOA PIN5.\n");
        sx1268_interface_debug_print("sx1268: MISO connected to GPIOA PIN6.\n");
        sx1268_interface_debug_print("sx1268: MOSI connected to GPIOA PIN7.\n");
        sx1268_interface_debug_print("sx1268: CS connected to GPIOA PIN4.\n");
        sx1268_interface_debug_print("sx1268: RESET connected to GPIOB PIN1.\n");
        sx1268_interface_debug_print("sx1268: DIO1 connected to GPIOB PIN0.\n");
        sx1268_interface_debug_print("sx1268: BUSY connected to GPIOA PIN8.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register sx1268 function */
    shell_init();
    shell_register("sx1268", sx1268);
    uart_print("sx1268: welcome to libdriver sx1268.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("sx1268: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("sx1268: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("sx1268: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("sx1268: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("sx1268: param is invalid.\n");
            }
            else
            {
                uart_print("sx1268: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
