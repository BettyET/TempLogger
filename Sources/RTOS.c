/*
 * LocalRTOS.c

 *
 *  Created on: 23.10.2015
 *      Author: User
 */

#include "app.h"
#include "Wait1.h"
#include "CS1.c"
#include "LED1.h"
#include "CLS1.h"
#include "AS1.h"
#include "FRTOS1.h"
#include "USB1.h"
#include "AD1.h"
#include "Key.h"
#include "lookup.h"
#include "RTOS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMP_STORAGE  1

uint8_t voltage_mv = 0;
int ctr;
int time[TEMP_STORAGE];
uint16_t temp1[TEMP_STORAGE];
uint16_t temp2[TEMP_STORAGE];
uint16_t temp3[TEMP_STORAGE];
uint16_t temp4[TEMP_STORAGE];
uint16_t temp5[TEMP_STORAGE];
uint16_t temp6[TEMP_STORAGE];
uint16_t temp7[TEMP_STORAGE];
uint16_t temp8[TEMP_STORAGE];
uint16_t temp9[TEMP_STORAGE];
uint16_t ref = 0.0f;
bool send = TRUE;

static uint16_t value[AD1_CHANNEL_COUNT]; 					// save ad- value

static uint8_t cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t in_buffer[USB1_DATA_BUFF_SIZE];

static void MainTask(void *pvParameters) {
	(void) pvParameters;
	AD1_Calibrate(TRUE);
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount(); 					// Aktueller Tick wird gespeichert
	for (;;) {
		send = Key_GetVal();
		if (send == FALSE) {
			//LED1_On();

			for (ctr = 0; ctr < TEMP_STORAGE; ctr++) {
				time[ctr]= xTaskGetTickCount();
				(void) AD1_Measure(TRUE);					// do the conversion, wait until done
				(void) AD1_GetChanValue16(0, &value[0]);	// save the value
				(void) AD1_GetChanValue16(1, &value[1]);
				(void) AD1_GetChanValue16(2, &value[2]);
				(void) AD1_GetChanValue16(3, &value[3]);
				(void) AD1_GetChanValue16(4, &value[4]);
				(void) AD1_GetChanValue16(5, &value[5]);
				(void) AD1_GetChanValue16(6, &value[6]);
				(void) AD1_GetChanValue16(7, &value[7]);
				(void) AD1_GetChanValue16(8, &value[8]);
				(void) AD1_GetChanValue16(9, &value[9]);
				//voltage_mv = (uint8_t)(value[0]-value[1])*3300/65536;
//				ref = value[0] / 64;						// Referenzspannung
//				temp1[ctr] = getTemp(((uint16_t) ((value[1]+12710) / 64) - ref));
//				temp2[ctr] = getTemp(((uint16_t) (value[2] / 64) - ref));
//				temp3[ctr] = getTemp(((uint16_t) (value[3] / 64) - ref));
//				temp4[ctr] = getTemp(((uint16_t) (value[4] / 64) - ref));
//				temp5[ctr] = getTemp(((uint16_t) (value[5] / 64) - ref));
//				temp6[ctr] = getTemp(((uint16_t) (value[6] / 64) - ref));
//				temp7[ctr] = getTemp(((uint16_t) (value[7] / 64) - ref));
//				temp8[ctr] = getTemp(((uint16_t) (value[8] / 64) - ref));
//				temp9[ctr] = getTemp(((uint16_t) ((value[9]) / 64) - ref));
				ref = value[0];
				temp1[ctr] = value[1]- ref;
				temp2[ctr] = value[2]- ref;
				temp3[ctr] = value[3]- ref;
				temp4[ctr] = value[4]- ref;
				temp5[ctr] = value[5]- ref;
				temp6[ctr] = value[6]- ref;
				temp7[ctr] = value[7]- ref;
				temp8[ctr] = value[8]- ref;
				temp9[ctr] = value[9]- ref;

			}
			//LED1_Off();
			int i;
			for (i = 0; i < ctr; i++) {
				(void) CDC1_App_Task(cdc_buffer, sizeof(cdc_buffer));
				char str[200];
				sprintf(str,
						"%d \t %d \t %d \t %d \r\n",
						temp2[i], temp3[i], temp8[i], temp9[i]);
				(void) CDC1_SendString((unsigned char*) str);
			}

		}
		FRTOS1_vTaskDelayUntil(&xLastWakeTime, 50/portTICK_RATE_MS);	// 50ms warten
	}
}

void RTOS_Run(void) {
	RTOS_Init();
	FRTOS1_vTaskStartScheduler(); /* does usually not return! */
}

void RTOS_Init(void) {
	/*! \todo Add tasks here */
	if (FRTOS1_xTaskCreate(MainTask, (signed portCHAR *)"Main",
			configMINIMAL_STACK_SIZE, (void*)NULL, tskIDLE_PRIORITY,
			NULL) != pdPASS) {
		for (;;) {
		} /* error */
	}
}

void RTOS_Deinit(void) {
	/* nothing needed */
}
