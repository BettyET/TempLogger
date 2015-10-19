/**
 * \file
 * \brief Timer driver
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements the driver for all our timers.
  */


#include "Timer.h"

static unsigned int cntr = 0;

void TMR_OnInterrupt(void) {

  if (cntr<4294967296){
	  cntr++;
  }
  else{
	  cntr = 0;
  }
}

void TMR_Init(void) {
  /* nothing needed right now */
}

void TMR_Deinit(void) {
  /* nothing needed right now */
}

unsigned int TMR_GetValue(void){
		return cntr;
}

void TMR_Reset(void){
	cntr = 0;
}
