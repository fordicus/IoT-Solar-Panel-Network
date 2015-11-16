//------------------------------------------------------------------------------
// Copyright (c) 1999 SEUNGJUN Co., Ltd. Technology Licensing Seoul,Korea.
// Any reproduction without written permission is prohibited by law.
//------------------------------------------------------------------------------
// Produced by SEUNGJUN Co., Ltd. Laboratory, Dennis
//------------------------------------------------------------------------------
// Component : IRQ
//
// Revision : 0.1
//
// Date     : Nov 14, 2014
// Author   : Dennis
//

#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "LED.h"

static unsigned long ticks = 0;
unsigned long timetick=0;

/*----------------------------------------------------------------------------
  Systick Interrupt Handler
  SysTick interrupt happens every 10 ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
static uint8_t LedOnOff = 0;
	
	timetick++;
	
  if (ticks++ >= 99) 
	{
			ticks = 0;
			if (LedOnOff) 
			{
				LED_On(1);
				LedOnOff = 0;
			}
			else
			{
				LED_Off(1);
				LedOnOff = 1;
			}		
  }
}
