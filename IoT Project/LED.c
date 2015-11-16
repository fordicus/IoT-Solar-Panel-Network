//------------------------------------------------------------------------------
// Copyright (c) 1999 SEUNGJUN Co., Ltd. Technology Licensing Seoul,Korea.
// Any reproduction without written permission is prohibited by law.
//------------------------------------------------------------------------------
// Produced by SEUNGJUN Co., Ltd. Laboratory, Dennis
//------------------------------------------------------------------------------
// Component : LED
//
// Revision : 0.1
//
// Date     : Nov 14, 2014
// Author   : Dennis
//

#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "LED.h"

const unsigned long led_mask[] =
    {
        1UL << 28, 1UL << 29, 1UL << 31, 1UL << 2,
        1UL << 3, 1UL << 4, 1UL << 5, 1UL << 6, 1UL <<8
    };


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init ( void )
{

	LPC_SC->PCONP |= ( 1 << 15 );            /* enable power to GPIO & IOCON  */

	LPC_GPIO1->FIODIR |= 0xB0000000;           /* LEDs on PORT1 are output      */
	LPC_GPIO2->FIODIR |= 0x0000007C;           /* LEDs on PORT2 are output      */
}

/*----------------------------------------------------------------------------
  8-bits On
 *----------------------------------------------------------------------------*/
void LED_All_On (void)
{

	LPC_GPIO1->FIOPIN |= 0xB0000000;
	LPC_GPIO2->FIOPIN |= 0x0000007C;
}

/*----------------------------------------------------------------------------
  8-bits Off
 *----------------------------------------------------------------------------*/
void LED_All_Off (void)
{

	LPC_GPIO1->FIOPIN |= ~0xB0000000;
	LPC_GPIO2->FIOPIN |= ~0x0000007C;
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On ( unsigned int num )
{

	if ( num < 3 ) LPC_GPIO1->FIOPIN |= led_mask[ num ];
	else LPC_GPIO2->FIOPIN |= led_mask[ num ];
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off ( unsigned int num )
{

	if ( num < 3 ) LPC_GPIO1->FIOPIN &= ~led_mask[ num ];
	else LPC_GPIO2->FIOPIN &= ~led_mask[ num ];
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out( unsigned int value )
{
	int i;

	for ( i = 0; i < LED_NUM; i++ )
	{
		if ( value & ( 1 << i ) )
		{
			LED_On ( i );
		}
		else
		{
			LED_Off( i );
		}
	}
}
