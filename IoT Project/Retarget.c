//------------------------------------------------------------------------------
// Copyright (c) 1999 SEUNGJUN Co., Ltd. Technology Licensing Seoul,Korea.
// Any reproduction without written permission is prohibited by law.
//------------------------------------------------------------------------------
// Produced by SEUNGJUN Co., Ltd. Laboratory, Dennis
//------------------------------------------------------------------------------
// Component : Retarget
//
// Revision : 0.1
//
// Date     : Nov 14, 2014
// Author   : Dennis
//

#include <stdio.h>
#include <rt_misc.h>
#include "Serial.h"

#pragma import(__use_no_semihosting_swi)



struct __FILE { int handle; /* Add whatever you need here */ };

FILE __stdout;
FILE __stdin;


int fputc( int c, FILE *f )
{
	return ( UART0_PutChar( c ) );
}


int fgetc( FILE *f )
{
	return ( UART0_GetChar() );
}


int ferror( FILE *f )
{
	/* Your implementation of ferror */
	return EOF;
}


void _ttywrch( int c )
{
	UART0_PutChar( c );
}


void _sys_exit( int return_code )
{
label: goto label;  /* endless loop */
}
