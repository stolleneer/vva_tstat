/*
 * tstat.h
 *
 *  Created on: Mar 4, 2015
 *      Author: Steven
 */
/**
  ******************************************************************************
  * File Name         : tstat.h
  * Author						:	Steven Tolleneer
  * Date              : 2015-03-04
  * Description       : Header file for verventa tstat.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSTAT_H
#define __TSTAT_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
 /**
   * @brief FT6206 register structure definition
*/

typedef enum TSTAT_MODE_t {
	mode_heat_req				= 0,
	mode_cool_req				= 1,
	mode_switchover			= 2,
	mode_standby				= 3
} TSTAT_MODE_t;

// TODO: put in global file used by controller etc
//typedef enum TS_UNITS_t {
//	units_celsius				= 0,
//	units_fahrenheit		= 1
//} TS_UNITS_t;

typedef struct TSTAT_t {
	float ts_setpoint;										// current setpoint
	float ts_temperature;									// current temperature


} TSTAT_t;







/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
