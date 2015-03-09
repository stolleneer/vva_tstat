/**
  ******************************************************************************
  * File Name         : FT6206.c
  * Author						:	Steven Tolleneer
  * Date              : 2015-02-19
  * Description       : Driver for FT6206 based capacitive touc panels
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
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
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
#include <ft6206.h>

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"

/* Private variables -------------------------------------------------*/
/* FT6206 Instance */
FT6206_t FT6206;

/* Private variables ---------------------------------------------------------*/
/* I2C buffers */
uint8_t FT6206_tx_buffer[2];
uint8_t FT6206_rx_buffer[2];

/* Private Functions -----------------------------------------------------*/
static uint8_t FT6206_read_register(uint8_t reg);
static uint8_t FT6206_write_register(uint8_t reg, uint8_t data);

static uint8_t FT6206_read_register(uint8_t reg) {
	uint8_t return_val = 0;
	// wait till we are ready
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	// store register to read in transmit buffer
	FT6206_tx_buffer[0] = reg;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint8_t) FT6206_ADDR << 1, (uint8_t*) FT6206_tx_buffer, 1, 1000) != HAL_OK) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	}
	/*	Wait till end of transmit	*/
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	/* recept data through "aRxBuffer" buffer */
	if(HAL_I2C_Master_Receive(&hi2c1, (uint8_t)FT6206_ADDR<<1, (uint8_t *)FT6206_rx_buffer, 1, 1000) != HAL_OK) {
		/* Error_Handler() function is called when Timeout error occurs.
		When Acknowledge failure occurs (Slave don't acknowledge it's address)
		Master restarts communication */
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}
	return_val = FT6206_rx_buffer[0];
	return return_val;
}

static uint8_t FT6206_write_register(uint8_t reg, uint8_t data){
	uint8_t return_val = 0;
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	FT6206_tx_buffer[0] = reg;
	FT6206_tx_buffer[1] = data;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint8_t)FT6206_ADDR<<1, (uint8_t*)FT6206_tx_buffer, TXBUFFERSIZEI2C, 1000)!= HAL_OK){
		/* Error_Handler() function is called when Timeout error occurs.
		When Acknowledge failure occurs (Slave don't acknowledge it's address)
		Master restarts communication */
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	}
	return return_val;
}

void FT6206_begin(uint8_t threshold){
	// reset CTP
	CTP_reset();
	// write threshold
	if (threshold == 0) {
		threshold = FT6206_DEFAULT_THRESHOLD;
	}
	FT6206_write_register((uint8_t)FT6206_REG_THRESHOLD, threshold);
	// Set INT_Mode to trigger
	FT6206_write_register((uint8_t)FT6206_REG_G_MODE, FT6206_INT_TRIGGER_MODE);
	// fill data structure
	FT6206.vendor_id 	= FT6206_read_register((uint8_t)FT6206_REG_VENDID);
	FT6206.chip_id 		= FT6206_read_register((uint8_t)FT6206_REG_CHIPID);
	FT6206.firmware_version = FT6206_read_register((uint8_t)FT6206_REG_FIRMVERS);
	FT6206.pointrate 	= FT6206_read_register((uint8_t)FT6206_REG_POINTRATE);
	FT6206.threshold 	= FT6206_read_register((uint8_t)FT6206_REG_THRESHOLD);

	FT6206_read_data();
/*
 * printf("Vendor ID       : %6d\n", FT6206.vendor_id);
  printf("Chip ID         : %6d\n", FT6206.chip_id);
  printf("Firmware version: %6d\n", FT6206.firmware_version);
  printf("Pointrate       : %6d\n", FT6206.pointrate);
  printf("Threshold       : %6d\n", FT6206.threshold);
*/
}

uint8_t FT6206_touched(void){
	//  1...2 touches
	uint8_t n;
	n = FT6206_read_register((uint8_t)FT6206_REG_NUMTOUCHES);
	if((n == 1) || (n == 2)){
		return n;
	}
	else{
		return 0;
	}
}

void FT6206_read_data(void){
	// array to hold values
  uint8_t raw_tp_data[16];		// array to hold packed info
  uint8_t i;
  uint8_t x;
  
  for (i=0; i<9; i++){
	  x = FT6206_read_register(i);
  	  raw_tp_data[i] = x;
  }

  FT6206.gesture_id = raw_tp_data[1];

  FT6206.touch_points = raw_tp_data[2];

  if(FT6206.touch_points > 2){	// error! only 0/1/2 are valid
  	FT6206.touch_points = 0;
  	FT6206.touch_01_xpos = 0;
  	FT6206.touch_01_ypos = 0;
  	FT6206.touch_01_weight = 0;
  	FT6206.touch_01_area = 0;
  	FT6206.touch_02_xpos = 0;
  	FT6206.touch_02_ypos = 0;
  	FT6206.touch_02_weight = 0;
  	FT6206.touch_02_area = 0;
  	return;
  }
/*
 *
  if(FT6206.touch_points == 0){	// error! only 0/1/2 are valid
  	FT6206.touch_points = 0;
  	FT6206.touch_01_xpos = 0;
  	FT6206.touch_01_ypos = 0;
  	FT6206.touch_01_weight = 0;
  	FT6206.touch_01_area = 0;
  	FT6206.touch_02_xpos = 0;
  	FT6206.touch_02_ypos = 0;
  	FT6206.touch_02_weight = 0;
  	FT6206.touch_02_area = 0;
    return;
  }
*/
  FT6206.touch_01_xpos = ((raw_tp_data[3]) & 0x0F);	// Mask low nibble of byte
  FT6206.touch_01_xpos <<= 8;					// left shift 8
  FT6206.touch_01_xpos |= raw_tp_data[4];		// OR with reg(0x04)
  
  FT6206.touch_01_event = ((raw_tp_data[3]) >> 4) & 0x03;	// mask with 0b11: 0b00->PressDown 0b01->LiftUp

  FT6206.touch_01_ypos = ((raw_tp_data[5]) & 0x0F);	// Mask low nibble of byte
  FT6206.touch_01_ypos <<= 8;					// left shift 8
  FT6206.touch_01_ypos |= raw_tp_data[6];		// OR with reg(0x06)

  FT6206.touch_01_id = ((raw_tp_data[5]) >> 4) & 0x03;	//Invalid when ID 0x0F
	FT6206.touch_01_weight = raw_tp_data[7];
	FT6206.touch_01_area = raw_tp_data[8];

	/*	Can be used later if multiTouch is desired */
/*
  FT6206.touch_02_xpos = raw_tp_data[9] & 0x0F;	// Mask byte
  FT6206.touch_02_xpos <<= 8;								// left shift 8
  FT6206.touch_02_xpos |= raw_tp_data[10];			// OR with reg(0x04)
  FT6206.touch_02_ypos = raw_tp_data[11] & 0x0F;	// Mask byte
  FT6206.touch_02_ypos <<= 8;								// left shift 8
  FT6206.touch_02_ypos |= raw_tp_data[12];			// OR with reg(0x04)

  FT6206.touch_02_id |= (raw_tp_data[11] >> 4);
 	FT6206.touch_02_weight = raw_tp_data[13];
 	FT6206.touch_02_area = raw_tp_data[14];
*/
}

