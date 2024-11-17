### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

RESET Pin: RST PB1.

DIO1 Pin: DIO1 PB0.

BUSY Pin: BUSY PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. SX1268

#### 3.1 Command Instruction

1. Show sx1268 chip and driver information.

   ```shell
   sx1268 (-i | --information)
   ```

2. Show sx1268 help.

   ```shell
   sx1268 (-h | --help)
   ```

3. Show sx1268 pin connections of the current board.

   ```shell
   sx1268 (-p | --port)
   ```

4. Run sx1268 register test.

   ```shell
   sx1268 (-t reg | --test=reg)
   ```

5. Run sx1268 send test.

   ```shell
   sx1268 (-t lora-send | --test=lora-send)
   ```

6. Run sx1268 receive test, time is the received timeout in second. 

   ```shell
   sx1268 (-t lora-receive | --test=lora-receive) [--timeout=<time>]
   ```

7. Run sx1268 cad test.

   ```shell
   sx1268 (-t lora-cad | --test=lora-cad)
   ```

8. Run sx1268  send function, str is the send data.

   ```shell
   sx1268 (-e lora-send | --example=lora-send) [--data=<str>]
   ```

9. Run sx1268 receive function, time is the received timeout in second. 

   ```shell
   sx1268 (-e lora-receive | --example=lora-receive) [--timeout=<time>]
   ```

10. Run sx1268 cad function.

    ```shell
    sx1268 (-e lora-cad | --example=lora-cad)
    ```

11. Run sx1268 sleep function.

    ```shell
    sx1268 (-e lora-sleep | --example=lora-sleep)
    ```

12. Run sx1268 wake up function.

    ```shell
    sx1268 (-e lora-wake-up | --example=lora-wake-up)
    ```

#### 3.2 Command Example

```shell
sx1268 -i

sx1268: chip is Semtech SX1268.
sx1268: manufacturer is Semtech.
sx1268: interface is SPI.
sx1268: driver version is 1.0.
sx1268: min supply voltage is 1.8V.
sx1268: max supply voltage is 3.7V.
sx1268: max current is 107.00mA.
sx1268: max temperature is 85.0C.
sx1268: min temperature is -40.0C.
```

```shell
sx1268 -p

sx1268: SCK connected to GPIOA PIN5.
sx1268: MISO connected to GPIOA PIN6.
sx1268: MOSI connected to GPIOA PIN7.
sx1268: CS connected to GPIOA PIN4.
sx1268: RESET connected to GPIOB PIN1.
sx1268: DIO1 connected to GPIOB PIN0.
sx1268: BUSY connected to GPIOA PIN8.
```

```shell
sx1268 -t reg

sx1268: chip is Semtech SX1268.
sx1268: manufacturer is Semtech.
sx1268: interface is SPI.
sx1268: driver version is 1.0.
sx1268: min supply voltage is 1.8V.
sx1268: max supply voltage is 3.7V.
sx1268: max current is 107.00mA.
sx1268: max temperature is 85.0C.
sx1268: min temperature is -40.0C.
sx1268: start register test.
sx1268: sx1268_write_register/sx1268_read_register test.
sx1268: set register 0xB0.
sx1268: check register ok.
sx1268: sx1268_write_buffer/sx1268_read_buffer test.
sx1268: set buffer 0xCB.
sx1268: check buffer ok.
sx1268: sx1268_timeout_convert_to_register/sx1268_timeout_convert_to_data test.
sx1268: set timeout 640.900000 us.
sx1268: check timeout 640.625000 us.
sx1268: sx1268_frequency_convert_to_register/sx1268_frequency_convert_to_data test.
sx1268: set frequency 47000000 Hz.
sx1268: check frequency 47000000 Hz.
sx1268: sx1268_gfsk_bit_rate_convert_to_register/sx1268_gfsk_bit_rate_convert_to_data test.
sx1268: set bit rate 470000 Hz.
sx1268: check bit rate 470156 Hz.
sx1268: sx1268_gfsk_frequency_deviation_convert_to_register/sx1268_gfsk_frequency_deviation_convert_to_data test.
sx1268: gfsk frequency deviation 10000 Hz.
sx1268: check gfsk frequency deviation 9999 Hz.
sx1268: sx1268_set_packet_type/sx1268_get_packet_type test.
sx1268: set gfsk type.
sx1268: check packet type ok.
sx1268: set lora type.
sx1268: check packet type ok.
sx1268: sx1268_set_fsk_whitening_initial_value/sx1268_get_fsk_whitening_initial_value test.
sx1268: set fsk whitening initial value 0x0100.
sx1268: check fsk whitening initial value ok.
sx1268: sx1268_set_fsk_crc_initical_value/sx1268_get_fsk_crc_initical_value test.
sx1268: set fsk crc initical value 0xD868.
sx1268: check fsk crc initical value ok.
sx1268: sx1268_set_fsk_crc_initical_value/sx1268_get_fsk_crc_initical_value test.
sx1268: set fsk crc polynomial value 0xAD07.
sx1268: check fsk crc polynomial value ok.
sx1268: sx1268_set_fsk_sync_word/sx1268_get_fsk_sync_word test.
sx1268: set fsk sync word 0x45 0xC8 0x7F 0x13 0xC4 0x6C 0x2B 0xBA.
sx1268: check fsk sync word ok.
sx1268: sx1268_set_fsk_node_address/sx1268_get_fsk_node_address test.
sx1268: set fsk node address 0x61.
sx1268: check fsk node address ok.
sx1268: sx1268_set_fsk_broadcast_address/sx1268_get_fsk_broadcast_address test.
sx1268: set fsk broadcast address 0x65.
sx1268: check fsk broadcast address ok.
sx1268: sx1268_set_iq_polarity/sx1268_get_iq_polarity test.
sx1268: set iq polarity 0xA1.
sx1268: check iq polarity ok.
sx1268: sx1268_set_lora_sync_word/sx1268_get_lora_sync_word test.
sx1268: set lora sync word 0xAC72.
sx1268: check lora sync word ok.
sx1268: sx1268_get_random_number test.
sx1268: random number is 0xA652A94A.
sx1268: sx1268_set_tx_modulation/sx1268_get_tx_modulation test.
sx1268: set tx modulation 0x04.
sx1268: check tx modulation ok.
sx1268: sx1268_set_rx_gain/sx1268_get_rx_gain test.
sx1268: set rx gain 0x64.
sx1268: check rx gain ok.
sx1268: sx1268_set_tx_clamp_config/sx1268_get_tx_clamp_config test.
sx1268: set tx clamp config 0x39.
sx1268: check tx clamp config ok.
sx1268: sx1268_set_ocp/sx1268_get_ocp test.
sx1268: set ocp 0x1D.
sx1268: check ocp config ok.
sx1268: sx1268_set_rtc_control/sx1268_get_rtc_control test.
sx1268: disable rtc control.
sx1268: check rtc control ok.
sx1268: sx1268_set_xta_trim/sx1268_get_xta_trim test.
sx1268: set xta trim 0x05.
sx1268: check xta trim ok.
sx1268: sx1268_set_xtb_trim/sx1268_get_xtb_trim test.
sx1268: set xtb trim 0x05.
sx1268: check xtb trim ok.
sx1268: sx1268_set_dio3_output_control/sx1268_get_dio3_output_control test.
sx1268: set dio3 output control 0x01.
sx1268: check dio3 output control ok.
sx1268: sx1268_set_event_mask/sx1268_get_event_mask test.
sx1268: set event mask 0x00.
sx1268: check event mask ok.
sx1268: sx1268_set_dio_output_enable/sx1268_get_dio_output_enable test.
sx1268: set dio output enable 0x00.
sx1268: check dio output enable ok.
sx1268: sx1268_set_dio_input_enable/sx1268_get_dio_input_enable test.
sx1268: set dio input enable 0x07.
sx1268: check dio input enable ok.
sx1268: sx1268_set_pull_up_control/sx1268_get_pull_up_control test.
sx1268: set pull up control 0x0A.
sx1268: check pull up control ok.
sx1268: sx1268_set_pull_down_control/sx1268_get_pull_down_control test.
sx1268: set pull down control 0x05.
sx1268: check pull down control ok.
sx1268: finish register test.
```

```shell
sx1268 -t lora-send

sx1268: start send test.
sx1268: irq tx done.
sx1268: finish send test.
```

```shell
sx1268 -t lora-receive --timeout=1000

sx1268: start receive test.
sx1268: start receiving...
sx1268: irq preamble detected.
sx1268: irq valid header.
sx1268: irq rx done.
sx1268: rssi is -53.0.
sx1268: snr is 11.50.
sx1268: signal rssi is -51.0.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F 0x20 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2E 0x2F 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x3A 0x3B 0x3C 0x3D 0x3E 0x3F 0x40 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4E 0x4F 0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x59 0x5A 0x5B 0x5C 0x5D 0x5E 0x5F 0x60 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x69 0x6A 0x6B 0x6C 0x6D 0x6E 0x6F 0x70 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 0x79 0x7A 0x7B 0x7C 0x7D 0x7E 0x7F 0x80 0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 0x89 0x8A 0x8B 0x8C 0x8D 0x8E 0x8F 0x90 0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 0x99 0x9A 0x9B 0x9C 0x9D 0x9E 0x9F 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 0xA9 0xAA 0xAB 0xAC 0xAD 0xAE 0xAF 0xB0 0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 0xB9 0xBA 0xBB 0xBC 0xBD 0xBE 0xBF 
sx1268: finish receive test.
```

```shell
sx1268 -t lora-cad

sx1268: start cad test.
sx1268: irq cad done.
sx1268: cad not detected.
sx1268: finish cad test.
```

```shell
sx1268 -e lora-send --data=LibDriver

sx1268: send LibDriver.
sx1268: irq tx done.
```

```shell
sx1268 -e lora-receive --timeout=100

sx1268: start receiving...
sx1268: irq rx done.
sx1268: rssi is -53.0.
sx1268: snr is 10.25.
LibDriver
```

```shell
sx1268 -e lora-cad

sx1268: irq cad done.
sx1268: cad not detected.
```

```shell
sx1268 -e lora-sleep

sx1268: sleep.
```

```shell
sx1268 -e lora-wake-up

sx1268: wake up.
```

```shell
sx1268 -h

Usage:
  sx1268 (-i | --information)
  sx1268 (-h | --help)
  sx1268 (-p | --port)
  sx1268 (-t reg | --test=reg)
  sx1268 (-t lora-send | --test=lora-send)
  sx1268 (-t lora-receive | --test=lora-receive) [--timeout=<time>]
  sx1268 (-t lora-cad | --test=lora-cad)
  sx1268 (-e lora-send | --example=lora-send) [--data=<str>]
  sx1268 (-e lora-receive | --example=lora-receive) [--timeout=<time>]
  sx1268 (-e lora-cad | --example=lora-cad)
  sx1268 (-e lora-sleep | --example=lora-sleep)
  sx1268 (-e lora-wake-up | --example=lora-wake-up)

Options:
      --data=<str>            Set the send data.([default: LibDriver])
  -e <lora-send | lora-receive | lora-cad | lora-sleep | lora-wake-up>, --example=<lora-send
     | lora-receive | lora-cad | lora-sleep | lora-wake-up>
                              Run the driver example.
  -h, --help                  Show the help.
  -i, --information           Show the chip information.
  -p, --port                  Display the pin connections of the current board.
  -t <reg | lora-send | lora-receive | lora-cad>, --test=<reg | lora-send | lora-receive | lora-cad>
                              Run the driver test.
      --timeout=<time>        Set the timeout in ms.([default: 1000])
```

