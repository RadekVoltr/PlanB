/*******************************************************************************
 * Copyright (C) 2014 DFRobot						       *
 *									       *
 * DFPlayer_Mini_Mp3, This library provides a quite complete function for      * 
 * DFPlayer mini mp3 module.                                                   *
 * www.github.com/dfrobot/DFPlayer_Mini_Mp3 (github as default source provider)*
 *  DFRobot-A great source for opensource hardware and robot.                  *
 *                                                                             *
 * This file is part of the DFplayer_Mini_Mp3 library.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is free software: you can redistribute it and/or          *
 * modify it under the terms of the GNU Lesser General Public License as       *
 * published by the Free Software Foundation, either version 3 of              *
 * the License, or any later version.                                          *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public            *
 * License along with DFPlayer_Mini_Mp3. If not, see                           *
 * <http://www.gnu.org/licenses/>.                                             *
 ******************************************************************************/

/*
 *	name:		DFPlayer_Mini_Mp3
 *	version:	1.0
 *	Author:		lisper <lisper.li@dfrobot.com>
 *	Date:		2014-05-22
 *	Description:	mp3 library for DFPlayer mini board
 *			note: mp3 file must put into mp3 folder in your tf card
 */

/*
 * 
 */

#include  "DFPlayer_Mini_Mp3.hpp"

#ifdef ARDUINO_ARCH_ESP32
  HardwareSerial* DFSerial;
#else
  PIO pio = pio0;
  uint sm = 0;
#endif
    
#define BUF_SIZE 10
uint8_t send_buf[BUF_SIZE] = {0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF};

unsigned long _last_call = 0;

static bool is_reply = false;

//
void mp3_set_reply (bool state) {
	is_reply = state;
	send_buf[4] = is_reply;
}

//
static void fill_uint16_bigend (uint8_t *thebuf, uint16_t data) {
	*thebuf =	(uint8_t)(data>>8);
	*(thebuf+1) =	(uint8_t)data;
}


//calc checksum (1~6 byte)
uint16_t mp3_get_checksum (uint8_t *thebuf) {
	uint16_t sum = 0;
	for (int i=1; i<7; i++) {
		sum += thebuf[i];
	}
	return -sum;
}

//fill checksum to send_buf (7~8 byte)
void mp3_fill_checksum () {
	uint16_t checksum = mp3_get_checksum (send_buf);
	fill_uint16_bigend (send_buf+7, checksum);
}

//
void h_send_func () {
  #ifdef ARDUINO_ARCH_ESP32
    DFSerial->write(send_buf,BUF_SIZE);
  #else
  for (int i=0; i<BUF_SIZE; i++) {
    uart_tx_program_putc(pio, sm, send_buf[i]);
  }
  #endif
}

//
#ifdef ARDUINO_ARCH_ESP32
  void mp3_set_serial(HardwareSerial Serial, uint SERIAL_BAUD)
  {
    DFSerial = &Serial;
    DFSerial->begin(SERIAL_BAUD);
  }
#else
  void mp3_set_serial (PIO pio_, uint sm_, uint PIN_TX, uint SERIAL_BAUD) {
    pio = pio_;
    sm = sm_;

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &uart_tx_program);
    uart_tx_program_init(pio, sm, offset, PIN_TX, SERIAL_BAUD);
  }
#endif

//
void mp3_send_cmd_int (uint8_t cmd, uint16_t high_arg, uint16_t low_arg) {
	/*
    
	 uint16_t d = abs(TIME_I2MS(chVTGetSystemTime()) - _last_call);
	if (d < 50) {
		  chThdSleepMilliseconds(50-d);

	}
 */

	send_buf[3] = cmd;

	send_buf[5] = high_arg;
	send_buf[6] = low_arg;

	mp3_fill_checksum ();
	h_send_func ();

	_last_call = millis();//TIME_I2MS(chVTGetSystemTime());
}

//
void mp3_send_cmd_arg (uint8_t cmd, uint16_t low_arg) {
	mp3_send_cmd_int(cmd, 0, low_arg);
}

//
void mp3_send_cmd (uint8_t cmd) {
	mp3_send_cmd_int(cmd, 0, 0);
}

// Send byte1 (0-15) as first byte of high and byte234 (0 - 4095) splitted in high byte 2 and low
void mp3_send_cmd_234 (uint8_t cmd, uint8_t byte1, uint32_t byte234) {
	uint16_t low = byte234 & 0x00FF;
	uint16_t high = (byte1 << 4) | (byte234 >> 8);

	mp3_send_cmd_int (cmd, high, low);
}


// Receive replay
/*
uint8_t* mp3_recv () {
	return mp3_recv_cmd(0);
}

// Wait and receive replay as int for specific command
int mp3_recv_int_cmd (int wait) {
	int res = 0;
	uint8_t* result = mp3_recv_cmd(wait);

    res = (unsigned char)result[0];
    res = res * 0xFF + (unsigned char)result[1];

    return res;
}

// Receive replay as int
int mp3_recv_int () {
	return mp3_recv_int_cmd(0);
}
*/


//
void mp3_play_physical_num (uint16_t num) {
	mp3_send_cmd_arg (0x03, num);
}

//
void mp3_play_physical () {
	mp3_send_cmd (0x03);
}

//
void mp3_next () {
	mp3_send_cmd (0x01);
}

//
void mp3_prev () {
	mp3_send_cmd (0x02);
}

//0x06 set volume 0-30
void mp3_set_volume (uint16_t volume) {
	mp3_send_cmd_arg (0x06, volume);
}

//0x07 set EQ0/1/2/3/4/5    Normal/Pop/Rock/Jazz/Classic/Bass
void mp3_set_EQ (uint16_t eq) {
	mp3_send_cmd_arg (0x07, eq);
}

//0x09 set device 1/2/3/4/5 U/SD/AUX/SLEEP/FLASH
void mp3_set_device (uint16_t device) {
	mp3_send_cmd_arg (0x09, device);
}

//
void mp3_sleep () {
	mp3_send_cmd (0x0a);
}

//
void mp3_reset () {
	mp3_send_cmd (0x0c);
}

//
void mp3_play () {
	mp3_send_cmd (0x0d);
}

//
void mp3_pause () {
	mp3_send_cmd (0x0e);
}

//
void mp3_stop () {
	mp3_send_cmd (0x16);
}

//play mp3 file in mp3 folder in your tf card
void mp3_play_num (uint16_t num) {
	mp3_send_cmd_arg (0x12, num);
}

//
void mp3_get_state () {
	mp3_send_cmd (0x42);
}
// Wait for mp3_get_state reply
/*
int mp3_wait_state () {
	return mp3_recv_int_cmd(0x42);
}
*/
//
void mp3_get_volume () {
	mp3_send_cmd (0x43);
}
// Wait for mp3_get_volume reply
/*
int mp3_wait_volume () {
	return mp3_recv_int_cmd(0x43);
}
*/
//
void mp3_get_u_sum () {
	mp3_send_cmd (0x47);
}
// Wait for mp3_get_u_sum reply
/*
int mp3_wait_u_sum () {
	return mp3_recv_int_cmd(0x47);
}
*/
//
void mp3_get_tf_sum () {
	mp3_send_cmd (0x48);
}
// Wait for mp3_get_tf_sum reply
/*
int mp3_wait_tf_sum () {
	return mp3_recv_int_cmd(0x48);
}
*/
//
void mp3_get_flash_sum () {
	mp3_send_cmd (0x49);
}
// Wait for mp3_get_flash_sum reply
/*
int mp3_wait_flash_sum () {
	return mp3_recv_int_cmd(0x49);
}
*/
//
void mp3_get_tf_current () {
	mp3_send_cmd (0x4c);
}
// Wait for mp3_get_tf_current reply
/*
int mp3_wait_tf_current () {
	return mp3_recv_int_cmd(0x4c);
}
*/
//
void mp3_get_u_current () {
	mp3_send_cmd (0x4b);
}
// Wait for mp3_get_u_current reply
/*
int mp3_wait_u_current() {
	return mp3_recv_int_cmd(0x4b);
}
*/
//
void mp3_get_flash_current () {
	mp3_send_cmd (0x4d);
}
// Wait for mp3_get_flash_current reply
/*int mp3_wait_flash_current () {
	return mp3_recv_int_cmd(0x4d);
}
*/
//
void mp3_single_loop (bool state) {
	mp3_send_cmd_arg (0x19, !state);
}

//add 
void mp3_single_play (uint16_t num) {
	mp3_play_num (num);
	delay (10);
	mp3_single_loop (true); 
	//mp3_send_cmd (0x19, !state);
}

//
void mp3_DAC (bool state) {
	mp3_send_cmd_arg (0x1a, !state);
}

//
void mp3_random_play () {
	mp3_send_cmd (0x18);
}

// Query total file numbers of a folder
void mp3_get_folder_sum (uint16_t folder) {
	mp3_send_cmd_arg (0x4E, folder);
}
// Wait for mp3_get_folder_sum reply
/*int mp3_wait_folder_sum () {
	return mp3_recv_int_cmd(0x4E);
}
*/
// Play mp3 file in selected folder
void mp3_play_file_in_folder (uint8_t folder, uint32_t num) {
	mp3_send_cmd_int (0x14, folder, num);
}
