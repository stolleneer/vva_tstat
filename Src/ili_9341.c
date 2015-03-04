/**
  ******************************************************************************
  * File Name         : ili_9341.c
  * Author						:	Steven Tolleneer
  * Date              : 2015-02-19
  * Description       : ili_9341.c display driver
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 Multicalor Industries NV
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Multicalor nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "ili_9341.h"
#include "stdint.h"

/**
  * @brief  Move to rectangle position
  * @param  x1: column address start
  * 				x2: column address end
  * 				y1:	page address start
  * 				y2:	page address end
  * @retval None
  */
void set_tft_window(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	LCD_REG  = COLUMN_ADDRESS_SET;
	LCD_DATA = 0;
	LCD_DATA = x1;
	LCD_DATA = 0;
	LCD_DATA = x2;

	LCD_REG  = PAGE_ADDRESS_SET;
	LCD_DATA = (y1 >> 8);
	LCD_DATA = (y1 & 0xFF);
	LCD_DATA = (y2 >> 8);
	LCD_DATA = (y2 & 0xFF);
}
/**
  * @brief  Fill a rectangle with specified color
  * @param  x: 	start x
  * 				y: 	start y
  * 				w:	width
  * 				y:	height
  * 				color: color value
  * @retval None
  */
void tft_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	uint32_t pixels;
	pixels = w * h;
	set_tft_window(x, y, (x + w - 1), (y + h - 1));
	LCD_REG = MEMORY_WRITE;
	while (pixels--)
		LCD_DATA = color;
}

void tft_write_cmd (uint8_t cmd, uint16_t value) {
	LCD_REG = cmd;
	LCD_DATA = value;
}

uint16_t tft_read_reg(uint8_t reg){
	LCD_REG = reg;
	return LCD_DATA;
}

void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
	if ((x>TFT_WIDTH) || (y>TFT_HEIGHT)) {
		return;
	}
	set_tft_window(x, y, TFT_WIDTH, TFT_HEIGHT);
	LCD_REG = MEMORY_WRITE;
	LCD_DATA = color;
}

void ILI9341_init(void) {
	LCD_REG = EXIT_SLEEP_MODE;
	HAL_Delay(5);
 	LCD_REG  = POWER_CONTROL_B;
	LCD_DATA = 0x00;
	LCD_DATA = 0xc3;
	LCD_DATA = 0X30;
 	LCD_REG  = POWER_ON_SEQUENCE_CONTROL;
	LCD_DATA = 0x64;
	LCD_DATA = 0x03;
	LCD_DATA = 0X12;
	LCD_DATA = 0X81;
 	LCD_REG  = DRIVER_TIMING_CTRL_A;
	LCD_DATA = 0x85;
	LCD_DATA = 0x10;
	LCD_DATA = 0x79;
 	LCD_REG  = POWER_CONTROL_A;
	LCD_DATA = 0x39;
	LCD_DATA = 0x2C;
	LCD_DATA = 0x00;
	LCD_DATA = 0x34;
	LCD_DATA = 0x02;
 	LCD_REG  = PUMP_RATIO_CONTROL;
	LCD_DATA = 0x20;
 	LCD_REG  = DRIVER_TIMING_CTRL_B;
	LCD_DATA = 0x00;
	LCD_DATA = 0x00;
 	LCD_REG  = POWER_CONTROL_1;
	LCD_DATA = 0x22;   //VRH[5:0]
 	LCD_REG  = POWER_CONTROL_2;
	LCD_DATA = 0x11;
 	LCD_REG  = VCOM_CONTROL_1;
	LCD_DATA = 0x3d;
	LCD_DATA = 0x20;
 	LCD_REG = VCOM_CONTROL_2;
	LCD_DATA = 0xAA;
 	LCD_REG  = MEMORY_ACCESS_CONTROL;
	LCD_DATA = 0x88;
 	LCD_REG  = PIXEL_FORMAT_SET;
	LCD_DATA = 0x55;
 	LCD_REG  = FRAME_RATE_CONTROL;
	LCD_DATA = 0x00;
	LCD_DATA = 0x13;
 	LCD_REG  = DISPLAY_FUNCTION_CONTROL;
	LCD_DATA = 0x0A;
	LCD_DATA = 0xA2;
 	LCD_REG  = INTERFACE_CONTROL;
	LCD_DATA = 0x01;
	LCD_DATA = 0x30;
 	LCD_REG  = GAMMA_CONTROL;
	LCD_DATA = 0x00;
 	LCD_REG  = GAMMA_SET;
	LCD_DATA = 0x01;
 	LCD_REG  = GAMMA_POS_CORRECT;
	LCD_DATA = 0x0F;
	LCD_DATA = 0x3F;
	LCD_DATA = 0x2F;
	LCD_DATA = 0x0C;
	LCD_DATA = 0x10;
	LCD_DATA = 0x0A;
	LCD_DATA = 0x53;
	LCD_DATA = 0XD5;
	LCD_DATA = 0x40;
	LCD_DATA = 0x0A;
	LCD_DATA = 0x13;
	LCD_DATA = 0x03;
	LCD_DATA = 0x08;
	LCD_DATA = 0x03;
	LCD_DATA = 0x00;
 	LCD_REG  = GAMMA_NEG_CORRECT;    //Set Gamma
	LCD_DATA = 0x00;
	LCD_DATA = 0x00;
	LCD_DATA = 0x10;
	LCD_DATA = 0x03;
	LCD_DATA = 0x0F;
	LCD_DATA = 0x05;
	LCD_DATA = 0x2C;
	LCD_DATA = 0xA2;
	LCD_DATA = 0x3F;
	LCD_DATA = 0x05;
	LCD_DATA = 0x0E;
	LCD_DATA = 0x0C;
	LCD_DATA = 0x37;
	LCD_DATA = 0x3C;
	LCD_DATA = 0x0F;
	LCD_REG  = EXIT_SLEEP_MODE;
	HAL_Delay(10);
 	LCD_REG  = DISPLAY_ON;
 	HAL_Delay(10);
	tft_fill_rect(  0,   0, 240,  320, RGB_BLACK);

	/*
	 * 36h MADCTL (Memory Access Control)
	 * D7 D6 D5 D4  D3 D2 D1 D0 HEX
	 *  0  0  1  1   0  1  1  0 36h
	 * MY MX MV ML BGR MH  0  0 00
	 * MY -> Row address order
	 * MX -> Column address order
	 * MV -> Row/Column exchange
	 *
	 * LCD_DATA = 0x88;
	 *  +------+
	 *  |      |
	 *  |      |
	 * 0+------+
	 *  0|TAIL|
	 * Linksonder naar boven
	 *
	 * LCD_DATA = 0x48;
	 *         0
	 *  +------+0
	 *  |      |
	 *  |      |
	 *  +------+
	 *   |TAIL|
	 * Rechts boven naar beneden
	 *
	 * LCD_DATA = 0x08;
	 *  0
	 * 0+------+
	 *  |      |
	 *  |      |
	 *  +------+
	 *   |TAIL|
	 *  Links boven naar beneden
	 */
}
