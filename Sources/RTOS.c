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

#define TEMP_STORAGE  1000

uint8_t voltage_mv = 0;
int ctr;
int time[TEMP_STORAGE];
uint16_t temp1[TEMP_STORAGE];
uint16_t temp2[TEMP_STORAGE];
uint16_t ref = 0.0f;
bool send = TRUE;

static uint16_t value[AD1_CHANNEL_COUNT]; 					// save ad- value

static uint8_t cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t in_buffer[USB1_DATA_BUFF_SIZE];


static void MainTask(void *pvParameters) {
	(void)pvParameters;
	TickType_t xLastWakeTime;
	AD1_Calibrate(TRUE);
	while (1) {
		send = Key_GetVal();
		if (send == FALSE) {
			LED1_On();
			for (ctr = 0; ctr < TEMP_STORAGE; ctr++) {
				xLastWakeTime = xTaskGetTickCount();						// Aktueller Tick wird gespeichert
				time[ctr]= xLastWakeTime;									// Zeit abspeichern
				(void) AD1_Measure(TRUE);									// do the conversion, wait until done
				(void) AD1_GetChanValue16(0, &value[0]);					// save the value
				(void) AD1_GetChanValue16(1, &value[1]);
				(void) AD1_GetChanValue16(2, &value[2]);
				//voltage_mv = (uint8_t)(value[0]-value[1])*3300/65536;
				ref = value[0] / 64;										// Referenzspannung
				temp1[ctr] = getTemp(((uint16_t) (value[1] / 64) - ref));
				temp2[ctr] = getTemp(((uint16_t) (value[2] / 64) - ref));
				FRTOS1_vTaskDelayUntil(&xLastWakeTime, 50/portTICK_RATE_MS);// 50ms warten
			}
			LED1_Off();
			int i;
			for (i = 0; i < ctr; i++) {
				(void) CDC1_App_Task(cdc_buffer, sizeof(cdc_buffer));
				char str[30];
				sprintf(str, "%d \t %d \t %d \r\n", time[i], temp1[i],
						temp2[i]);
				//CLS1_SendStr(str, ioLocal->stdOut);

				(void) CDC1_SendString((unsigned char*) str);

			}

		}

	}
}

void RTOS_Run(void) {
	RTOS_Init();
	FRTOS1_vTaskStartScheduler();  /* does usually not return! */
}

void RTOS_Init(void) {
  /*! \todo Add tasks here */
  if (FRTOS1_xTaskCreate(MainTask, (signed portCHAR *)"Main", configMINIMAL_STACK_SIZE, (void*)NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error */
  }
}

void RTOS_Deinit(void) {
  /* nothing needed */
}
