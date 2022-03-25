### 1. chip

#### 1.1 chip info

chip name : STM32F407ZGT6.

extern oscillator : 8MHz.

uart pin: TX/RX PA9/PA10.

spi pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

rst pin: RST PB1.

dio1 pin: DIO1 PB0

busy pin: BUSY PA8

### 2. shell

#### 2.1 shell parameter

baud rate: 115200

data bits : 8

stop bits: 1

parity: none

flow control: none

### 3. sx1268

#### 3.1 command Instruction

​           sx1268 is a basic command which can test all sx1268 driver function:

​           -i        show sx1268 chip and driver information.

​           -h       show sx1268 help.

​           -p       show sx1268 pin connections of the current board.

​           -t (reg | sent -lora | receive -lora <timeout> | cad -lora)

​           -t  reg        run sx1268 register test.

​           -t sent -lora      run sx1268 sent test.

​           -t receive -lora <timeout>        run sx1268 receive test. timeout is the received timeout in second. 

​           -t cad -lora       run sx1268 cad test.

​           -c (sent -lora <data> | receive  -lora <timeout>| cad -lora | sleep -lora | wake_up -lora)

​           -c sent -lora <data>        run sx1268  sent function. data is the send data.

​           -c receive  -lora <timeout>        run sx1268 receive function. timeout is the received timeout in second. 

​           -c cad -lora       run sx1268 cad function.

​           -c sleep -lora       run sx1268 sleep function.

​           -c wake_up  -lora       run sx1268 wake up function.

#### 3.2 command example

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
sx1268: RST connected to GPIOB PIN1.
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
sx1268: random number is 0xA1A88922.
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
sx1268 -t sent -lora  

sx1268: start sent test.
sx1268: irq tx done.
sx1268: finish sent test.
```

```shell
sx1268 -t receive -lora 100

sx1268: start receive test.
sx1268: start receiving...
sx1268: irq preamble detected.
sx1268: irq valid header.
sx1268: irq rx done.
sx1268: rssi is -23.0.
sx1268: snr is 10.50.
sx1268: signal rssi is -23.0.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F 0x20 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2E 0x2F 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x3A 0x3B 0x3C 0x3D 0x3E 0x3F 0x40 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4E 0x4F 0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x59 0x5A 0x5B 0x5C 0x5D 0x5E 0x5F 0x60 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x69 0x6A 0x6B 0x6C 0x6D 0x6E 0x6F 0x70 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 0x79 0x7A 0x7B 0x7C 0x7D 0x7E 0x7F 0x80 0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 0x89 0x8A 0x8B 0x8C 0x8D 0x8E 0x8F 0x90 0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 0x99 0x9A 0x9B 0x9C 0x9D 0x9E 0x9F 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 0xA9 0xAA 0xAB 0xAC 0xAD 0xAE 0xAF 0xB0 0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 0xB9 0xBA 0xBB 0xBC 0xBD 0xBE 0xBF 
sx1268: finish receive test.
```

```shell
sx1268 -t cad -lora

sx1268: start cad test.
sx1268: irq cad done.
sx1268: irq cad detected.
sx1268: cad detected.
sx1268: finish cad test.
```

```shell
sx1268 -c sent -lora libdriver

sx1268: sent libdriver.
sx1268: irq tx done.
```

```shell
sx1268 -c receive -lora 100

sx1268: start receiving...
sx1268: irq rx done.
sx1268: rssi is -17.0.
sx1268: snr is 10.50.
libdriver
```

```shell
sx1268 -c cad -lora   

sx1268: irq cad done.
sx1268: cad not detected.
```

```shell
sx1268 -c sleep -lora

sx1268: sleep.
```

```shell
sx1268 -c wake_up -lora   

sx1268: wake up.
```

```shell
sx1268 -h

sx1268 -i
	show sx1268 chip and driver information.
sx1268 -h
	show sx1268 help.
sx1268 -p
	show sx1268 pin connections of the current board.
sx1268 -t reg
	run sx1268 register test.
sx1268 -t sent -lora
	run sx1268 sent test.
sx1268 -t receive -lora <timeout>
	run sx1268 receive test.timeout is the received timeout in second.
sx1268 -t cad -lora
	run sx1268 cad test.
sx1268 -c sent -lora <data>
	run sx1268 sent function.data is the send data.
sx1268 -c receive -lora <timeout>
	run sx1268 receive function.timeout is the received timeout in second.
sx1268 -c cad -lora
	run sx1268 cad function.
sx1268 -c sleep -lora
	run sx1268 sleep function.
sx1268 -c wake_up -lora
	run sx1268 wake up function.
```

