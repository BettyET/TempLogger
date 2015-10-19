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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint16_t value[AD1_CHANNEL_COUNT]; 					// save ad- value

uint8_t voltage_mv = 0;
float temp1 = 0.0f;
float temp2 = 0.0f;
float ref = 0.0f;
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
			//WAIT1_Waitms(200);
			(void)AD1_Measure(TRUE);				// do the conversion, wait until done
			(void)AD1_GetChanValue16(0,&value[0]);	// save the value
			(void)AD1_GetChanValue16(1,&value[1]);
			(void)AD1_GetChanValue16(2,&value[2]);
			//voltage_mv = (uint8_t)(value[0]-value[1])*3300/65536;
			ref = value[0]/64;						// Referenzspannung
			temp1= getTemp(((uint16_t)(value[1]/64)-ref));
			temp2= getTemp(((uint16_t)(value[2]/64)-ref));
			char str[100];
			sprintf(str, "%d \t %.2f \t %.2f \n",TMR_GetValue(), temp1, temp2);
			CLS1_SendStr(str, ioLocal->stdOut);
			LED1_Off();
		}

	}
}




