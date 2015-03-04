/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 27/02/2015 10:45:49
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "crc.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "main.h"
#include "ili_9341.h"
#include "ft6206.h"
// includes for GUI
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
GUI_PID_STATE TS_State;									// touch screen state
WM_HWIN hwin_home_scr;									// window handle home screen

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
extern WM_HWIN CreateHomeScreen(void);
extern WM_HWIN CreateSplashScreen(void);

void show_splash(void);
void update_touch(void);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_CRC_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
  __CRC_CLK_ENABLE();										// need this for STemWin
  /* USER CODE END 2 */

  /* USER CODE BEGIN 3 */
  TFT_reset();													// reset TFT
  FT6206_begin(0);											// reset touch panel
  /* Infinite loop */
  GUI_Init();														// init GUI and set default skin
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
  PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);

  show_splash();

  hwin_home_scr = CreateHomeScreen();		// create main window
  WM_ShowWindow(hwin_home_scr);					// display window

  while (1)
  {
 		update_touch();
  	GUI_Exec();
  }
  /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}

/* USER CODE BEGIN 4 */
void show_splash(void) {
	WM_HWIN hwin_splash_scr;
	PROGBAR_Handle hprogbar;
	uint8_t	i;

	hwin_splash_scr = CreateSplashScreen();
	WM_ShowWindow(hwin_splash_scr);					// display window
	/* Get handle of the progress bar */
	hprogbar = WM_GetDialogItem(hwin_splash_scr, (GUI_ID_USER + 0x01));
	for (i = 0; i < 100; i++) {
		PROGBAR_SetValue(hprogbar, i);
		GUI_Exec();
		HAL_Delay(10);
	}
	WM_DeleteWindow(hwin_splash_scr);
}


void update_touch(void){
	if (tstat_flags & (1 << TSTAT_TOUCHED)) {
		tstat_flags &= ~(1 << TSTAT_TOUCHED);	// clear bit
		FT6206_read_data();
		// swap XY & mirror Y
		TS_State.x = FT6206.touch_01_ypos;
		TS_State.y = 240 - FT6206.touch_01_xpos;
		TS_State.Pressed = 1;
		GUI_TOUCH_StoreStateEx(&TS_State);
	}
	else {
		if (ts_timer > 9) {
			ts_timer = 0;
			if (FT6206_touched()) {
				TS_State.Pressed = 1;
			}
			else {
				TS_State.Pressed = 0;
			}
			GUI_TOUCH_StoreStateEx(&TS_State);
		}
	}
}

void TFT_test(void){
	static uint8_t rotation = 0;
	switch (rotation) {
	case 0:
		LCD_REG = MEMORY_ACCESS_CONTROL;
		LCD_DATA =	ILI9341_MADCTL_MY |
								ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR ;
		rotation = 1;
		break;
	case 1:
		LCD_REG = MEMORY_ACCESS_CONTROL;
		LCD_DATA =	ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV |
								ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR ;
		rotation = 2;
		break;
	case 2:
		LCD_REG = MEMORY_ACCESS_CONTROL;
		LCD_DATA =	ILI9341_MADCTL_MX |
								ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR ;
		rotation = 3;
		break;
	default:
		LCD_REG = MEMORY_ACCESS_CONTROL;
		LCD_DATA =	ILI9341_MADCTL_MV |
								ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR ;
		rotation = 0;
		break;
	}
	tft_fill_rect(  0,   0, 240,  320, RGB_BLACK);
	tft_fill_rect(  0,   0,  40,   40, RGB_WHITE);
	tft_fill_rect( 40,   0,  40,   40, RGB_SILVER);
	tft_fill_rect( 80,   0,  40,   40, RGB_RED);
	tft_fill_rect(120,   0,  40,   40, RGB_YELLOW);
	tft_fill_rect(160,   0,  40,   40, RGB_GREEN);
	tft_fill_rect(200,   0,  40,   40, RGB_BLUE);
	tft_fill_rect(  0,  40,  40,   40, RGB_MAROON);
	tft_fill_rect( 40,  40,  40,   40, RGB_OLIVE);
	tft_fill_rect( 80,  40,  40,   40, RGB_NAVY);
	tft_fill_rect(120,  40,  40,   40, RGB_PURPLE);
	tft_fill_rect(160,  40,  40,   40, RGB_TEAL);
	tft_fill_rect(200,  40,  40,   40, RGB_LIME);
}
/**
  * @brief Resets the TFT: Pin B 1 low for 5 milliseconds.
  * @retval None
  */
void TFT_reset(void){
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  ILI9341_init();
}
/**
  * @brief Resets the CTP: Pin C 3 low for 5 milliseconds.
  * @retval None
  */
void CTP_reset(void){
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
