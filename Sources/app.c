/*
 * app.c
 *
 *  Created on: 25.09.2015
 *      Author: User
 */
#include "app.h"
#include "LED1.h"
#include "WAIT1.h"
#include "AS1.h"
#include "CLS1.h"
#include "Key.h"
#include "lookup.h"
#include "AD1.h"
#include "Timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <RTOS.h>




void APP_run(void) {
	AS1_Init();
	CLS1_Init();
	CLS1_ConstStdIOTypePtr ioLocal = CLS1_GetStdio();
	RTOS_Run();
}


