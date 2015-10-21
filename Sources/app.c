/*
 * app.c
 *
 *  Created on: 25.09.2015
 *      Author: User
 */
#include "app.h"
#include "LED1.h"
#include "WAIT1.h"
#include "AD1.h"
#include "AS1.h"
#include "CLS1.h"
#include "Key.h"
#include "lookup.h"
#include "Timer.h"
#include "TI1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMP_STORAGE  1

static uint16_t value[AD1_CHANNEL_COUNT]; 					// save ad- value

uint8_t voltage_mv = 0;
int ctr;
int time[TEMP_STORAGE];
uint16_t temp1[TEMP_STORAGE];
uint16_t temp2[TEMP_STORAGE];
uint16_t ref = 0.0f;
bool send = TRUE;



void APP_run(void) {
	AS1_Init();
	CLS1_Init();
	AD1_Calibrate(TRUE);
	CLS1_ConstStdIOTypePtr ioLocal = CLS1_GetStdio();
	for (;;) {
		send = Key_GetVal();
		if(send==FALSE){
			LED1_On();
			for(ctr=0; ctr < TEMP_STORAGE;ctr++){
				WAIT1_Waitms(50);
				(void)AD1_Measure(TRUE);				// do the conversion, wait until done
				time[ctr]=TMR_GetValue();				// aktuelle Zeit
				(void)AD1_GetChanValue16(0,&value[0]);	// save the value
				(void)AD1_GetChanValue16(1,&value[1]);
				(void)AD1_GetChanValue16(2,&value[2]);
				//voltage_mv = (uint8_t)(value[0]-value[1])*3300/65536;
				ref = value[0]/64;						// Referenzspannung
				temp1[ctr]= getTemp(((uint16_t)(value[1]/64)-ref));
				temp2[ctr]= getTemp(((uint16_t)(value[2]/64)-ref));
				if(send==TRUE){
					break;								// Messung abbrechen
				}
			}
			LED1_Off();
			int i;
			for(i=0; i<ctr; i++){
				//char str[30];
				//sprintf(str, "%d \t %d \t %d \n",time[i], temp1[i], temp2[i]);
				//CLS1_SendStr(str, ioLocal->stdOut);
				CLS1_SendNum16u(time[i], ioLocal->stdOut);CLS1_SendChar('\t');
				CLS1_SendNum16u(temp1[i], ioLocal->stdOut);CLS1_SendChar('\t');
				CLS1_SendNum16u(temp2[i], ioLocal->stdOut);CLS1_SendChar('\t');
				CLS1_SendChar('\n');
			}

		}

	}
}




