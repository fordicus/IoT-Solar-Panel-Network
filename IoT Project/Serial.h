//------------------------------------------------------------------------------
// Copyright (c) 1999 SEUNGJUN Co., Ltd. Technology Licensing Seoul,Korea.
// Any reproduction without written permission is prohibited by law.
//------------------------------------------------------------------------------
// Produced by SEUNGJUN Co., Ltd. Laboratory, Dennis
//------------------------------------------------------------------------------
// Component : Serial
//
// Revision : 0.1
//
// Date     : Nov 14, 2014
// Author   : Dennis
//

#ifndef __SERIAL_H
#define __SERIAL_H

extern void SER_Init      (void);
extern void RS485_Init(void);
extern void DIP_SW_Init(void);
extern int  UART0_GetChar   (void);
extern int  UART0_PutChar   (int c);
extern int  UART1_GetChar   (void);
extern int  UART1_PutChar   (int c);
extern int  UART3_GetChar   (void);
extern int  UART3_PutChar   (int c);
extern int	RS485_SET_RX(void);
extern int	RS485_SET_TX(void);
extern int	DIP_value(int n);
extern int 	Delay(int n);

#endif
