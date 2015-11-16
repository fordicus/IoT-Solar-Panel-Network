;------------------------------------------------------------------------------
; Copyright (c) 1999 SEUNGJUN Co., Ltd. Technology Licensing Seoul,Korea.
; Any reproduction without written permission is prohibited by law.
;------------------------------------------------------------------------------
; Produced by SEUNGJUN Co., Ltd. Laboratory, Dennis
;------------------------------------------------------------------------------
; Component: Serial.S(UART1)
;
; Revision : 0.1
;
; Date     : Nov 14, 2014
; Author   : Dennis
;

		PRESERVE8
		THUMB       
LPC_GPIO_BASE_ADDR			EQU 0x2009C000
FIO0DIR_OFFSET					EQU	0x00
FIO1DIR_OFFSET					EQU 0x20
FIO2DIR_OFFSET					EQU	0x40

FIO0PIN_OFFSET					EQU 0x14
FIO1PIN_OFFSET					EQU 0x34
FIO2PIN_OFFSET					EQU 0x54

LPC_SC_PCONP					EQU	0x400fc0c4  
LPC_PINSEL_BASE_ADDR		EQU	0x4002C000
PINSEL0_OFFSET					EQU	0x00
PINSEL1_OFFSET					EQU	0x04
PINSEL2_OFFSET					EQU	0x08
PINSEL3_OFFSET					EQU	0x0C
PINSEL4_OFFSET					EQU	0x10
PINSEL9_OFFSET					EQU	0x24

;------------------------------------------------------------------------------------------------------------
;UART0 Register
;------------------------------------------------------------------------------------------------------------
UART0_BASE_ADDR				EQU	0x4000C000
UART0_RBR_OFFSET			EQU	0x00
UART0_THR_OFFSET			EQU	0x00
UART0_DLL_OFFSET				EQU	0x00
UART0_DLM_OFFSET			EQU	0x04
UART0_LCR_OFFSET			EQU	0x0C
UART0_LSR_OFFSET			EQU	0x14
UART0_FDR_OFFSET			EQU	0x28

;------------------------------------------------------------------------------------------------------------
;UART1 Register
;------------------------------------------------------------------------------------------------------------
UART1_BASE_ADDR				EQU	0x40010000
UART1_RBR_OFFSET			EQU	0x00
UART1_THR_OFFSET			EQU	0x00
UART1_DLL_OFFSET			EQU	0x00
UART1_DLM_OFFSET			EQU	0x04
UART1_LCR_OFFSET			EQU	0x0C
UART1_LSR_OFFSET			EQU	0x14
UART1_FDR_OFFSET			EQU	0x28

;------------------------------------------------------------------------------------------------------------
;UART3 Register
;------------------------------------------------------------------------------------------------------------
UART3_BASE_ADDR				EQU	0x4009C000
UART3_RBR_OFFSET			EQU	0x00
UART3_THR_OFFSET			EQU	0x00
UART3_DLL_OFFSET			EQU	0x00
UART3_DLM_OFFSET			EQU	0x04
UART3_LCR_OFFSET			EQU	0x0C
UART3_LSR_OFFSET			EQU	0x14
UART3_FDR_OFFSET			EQU	0x28

; The program
; Linker requires Reset_Handler
 
		AREA    MYCODE, CODE, READONLY

		ENTRY
		EXPORT	SER_Init
		EXPORT	RS485_Init
		EXPORT	DIP_SW_Init
		EXPORT	UART0_GetChar
		EXPORT	UART0_PutChar
		EXPORT	UART1_GetChar
		EXPORT	UART1_PutChar
		EXPORT	UART3_GetChar
		EXPORT	UART3_PutChar
		EXPORT	RS485_SET_RX
		EXPORT	RS485_SET_TX
		EXPORT	DIP_value
		EXPORT	Delay

;------------------------------------------------------------------------------------------------------------
SER_Init

		;-------------------------------------------------------------------------------------------------------
		; PCONP : Power Control for Peripherals register ( PCONP - address 0x400FC0C4 )
		;-------------------------------------------------------------------------------------------------------
		LDR		R0, =LPC_SC_PCONP														; ADDR to LOAD FROM
		LDR		R1, [R0, #0]																	; LOAD from ADDR
		LDR		R2, =0x3008018																; enable power to UART[3 downto 0] & GPIO
		ORR		R1,	R1,	R2																	; OR operation
		STR		R1, [R0, #0]																	; STORE to ADDR
		
		;-------------------------------------------------------------------------------------------------------
		; UART0
		;-------------------------------------------------------------------------------------------------------
		LDR		R0,=LPC_PINSEL_BASE_ADDR
		LDR		R1, [R0, #PINSEL0_OFFSET]
		ORR		R1, R1, #0x50																	; TXD0 RXD0
		STR		R1, [R0, #PINSEL0_OFFSET]

		LDR		R1, =UART0_BASE_ADDR
		MOV   	R0,#0x83																		; 8 bits, no Parity, 1 Stop bit
		STRB		R0, [R1, #UART0_LCR_OFFSET]

		MOV   	R0,#0x6C																		; 9600 Baud Rate @ 25.0 MHZ PCLK
		STRB		R0, [R1, #UART0_DLL_OFFSET]

		MOV   	R0,#0x21																		; FR 1,507, DIVADDVAL=1, MULVAL=2
		STRB		R0, [R1, #UART0_FDR_OFFSET]

		MOV   	R0,#0x00																		; High divisor latch = 0
		STRB		R0, [R1, #UART0_DLM_OFFSET]

		MOV   	R0,#0x03																		; DLAB = 0 
		STRB		R0, [R1, #UART0_LCR_OFFSET]
		
		;-------------------------------------------------------------------------------------------------------
		; UART1
		;-------------------------------------------------------------------------------------------------------
		LDR		R0,=LPC_PINSEL_BASE_ADDR
		LDR		R1, [R0, #PINSEL4_OFFSET]
		ORR		R1, R1, #2																		; Pin P2.0 used as TXD1 
		STR		R1, [R0, #PINSEL4_OFFSET]

		LDR		R1, [R0, #PINSEL4_OFFSET]
		ORR		R1, R1, #8																		; Pin P2.1 used as RXD1 
		STR		R1, [R0, #PINSEL4_OFFSET]

		LDR		R1, =UART1_BASE_ADDR
		MOV   	R0,#0x83																		; 8 bits, no Parity, 1 Stop bit
		STRB	R0, [R1, #UART1_LCR_OFFSET]

		MOV   	R0,#0x6C																		; 9600 Baud Rate @ 25.0 MHZ PCLK
		STRB	R0, [R1, #UART1_DLL_OFFSET]

		MOV   	R0,#0x21																		; FR 1,507, DIVADDVAL=1, MULVAL=2
		STRB	R0, [R1, #UART1_FDR_OFFSET]

		MOV   	R0,#0x00																		; High divisor latch = 0
		STRB	R0, [R1, #UART1_DLM_OFFSET]

		MOV   	R0,#0x03																		; DLAB = 0 
		STRB	R0, [R1, #UART1_LCR_OFFSET]
		
		BX		LR

RS485_Init

		;-------------------------------------------------------------------------------------------------------
		; UART3
		;-------------------------------------------------------------------------------------------------------
		LDR		R0,=LPC_PINSEL_BASE_ADDR
		LDR		R1, [R0, #PINSEL9_OFFSET]
		ORR		R1, R1, #0xF000000														; TXD3 RXD3
		STR		R1, [R0, #PINSEL9_OFFSET]

		LDR		R1, =UART3_BASE_ADDR
		MOV   	R0,#0x83																		; 8 bits, no Parity, 1 Stop bit
		STRB		R0, [R1, #UART3_LCR_OFFSET]

		MOV   	R0,#0x6C																		; 115200 Baud Rate @ 25.0 MHZ PCLK (0x6C : 9600)(0x09 : 115200)
		STRB		R0, [R1, #UART3_DLL_OFFSET]

		MOV   	R0,#0x21																		; FR 1,507, DIVADDVAL=1, MULVAL=2
		STRB		R0, [R1, #UART3_FDR_OFFSET]

		MOV   	R0,#0x00																		; High divisor latch = 0
		STRB		R0, [R1, #UART3_DLM_OFFSET]

		MOV   	R0,#0x03																		; DLAB = 0 
		STRB		R0, [R1, #UART3_LCR_OFFSET]
		
		;-------------------------------------------------------------------------------------------------------
		; RS485 Enable Signal
		;-------------------------------------------------------------------------------------------------------
		
		LDR		R0,=LPC_PINSEL_BASE_ADDR
		LDR		R1, [R0, #PINSEL0_OFFSET]			; PIN SELECT
		BIC		R1, R1, #0x300								; P0.4 is GPIO
		STR		R1, [R0, #PINSEL0_OFFSET]
		
		LDR		R0,=LPC_GPIO_BASE_ADDR
		LDR		R1, [R0, #FIO0DIR_OFFSET]			; PIN DIRECTION
		ORR		R1, R1, #0x10
		STR		R1, [R0, #FIO0DIR_OFFSET]
		
		BX		LR
		
;------------------------------------------------------------------------------------------------------------		
DIP_SW_Init

		LDR		R0, =LPC_PINSEL_BASE_ADDR
		LDR		R1, [R0, #PINSEL0_OFFSET]
		BIC		R1, R1, #0x3		; P0.0 P0.1 GPIO
		STR		R1, [R0, #PINSEL0_OFFSET]
		
		LDR		R1, [R0, #PINSEL1_OFFSET]
		BIC		R1, R1, #0x3C00	; P0.21 P0.22 GPIO
		STR		R1, [R0, #PINSEL1_OFFSET]
		
		LDR		R0, =LPC_GPIO_BASE_ADDR
		LDR		R1, [R0, #FIO0DIR_OFFSET]
		BIC		R1, R1, #0x3 ; DIR OUT 
		STR		R1, [R0, #FIO0DIR_OFFSET]
		
		LDR		R1, [R0, #FIO0DIR_OFFSET]
		BIC		R1, R1, #0x600000 ; DIR OUT
		STR		R1, [R0, #FIO0DIR_OFFSET]
		
		BX		LR
		
;------------------------------------------------------------------------------------------------------------
DIP_value
		LDR		R2,=LPC_GPIO_BASE_ADDR
		LDR		R1, [R2, #FIO0PIN_OFFSET]
		AND		R0, R0, R1
		BX		LR
		
;------------------------------------------------------------------------------------------------------------
RS485_SET_RX
		LDR		R2, =LPC_GPIO_BASE_ADDR
		LDR		R1, [R2, #FIO0PIN_OFFSET]
		BIC		R1, R1, #0x10
		STR		R1, [R2, #FIO0PIN_OFFSET]
		
		BX		LR
	
	
;------------------------------------------------------------------------------------------------------------
RS485_SET_TX
		LDR		R2, =LPC_GPIO_BASE_ADDR
		LDR		R1, [R2, #FIO0PIN_OFFSET]
		ORR		R1, R1, #0x10
		STR		R1, [R2, #FIO0PIN_OFFSET]

		BX		LR

;------------------------------------------------------------------------------------------------------------
UART0_Void
		LDR		R0, =0
		BX		LR
		
;------------------------------------------------------------------------------------------------------------
UART0_GetChar
		NOP
CheckUart0RxReady		
		LDR		R2, =UART0_BASE_ADDR
		LDRB	R0, [R2, #UART0_LSR_OFFSET]
		TST		R0, #1
		BEQ		UART0_Void
		LDRB	R0, [R2, #UART0_RBR_OFFSET]
		BX      	LR

;------------------------------------------------------------------------------------------------------------
UART0_PutChar
		LDR		R2, =UART0_BASE_ADDR
CheckUart0TxReady		
		LDRB	R1, [R2, #UART0_LSR_OFFSET]
		TST		R1, #0x20	
		BEQ		CheckUart0TxReady
		STRB	R0, [R2, #UART0_THR_OFFSET]
		BX    	LR

;------------------------------------------------------------------------------------------------------------
UART1_Void
		LDR		R0, =0
		BX		LR

;------------------------------------------------------------------------------------------------------------
UART1_GetChar
		NOP
		LDR		R2, =UART1_BASE_ADDR
		LDRB	R1, [R2, #UART1_LSR_OFFSET]
		TST		R1, #1
		BEQ		UART3_Void
		LDRB	R0, [R2, #UART1_RBR_OFFSET]
		BX      	LR

;------------------------------------------------------------------------------------------------------------
UART1_PutChar
		LDR		R2, =UART1_BASE_ADDR
CheckUart1TxReady		
		LDRB	R1, [R2, #UART1_LSR_OFFSET]
		TST		R1, #0x20	
		BEQ		CheckUart1TxReady
		STRB	R0, [R2, #UART1_THR_OFFSET]
		BX      	LR		


;------------------------------------------------------------------------------------------------------------
UART3_Void
		LDR		R0, =0
		BX		LR

;------------------------------------------------------------------------------------------------------------
UART3_GetChar
		NOP
		LDR		R2, =UART3_BASE_ADDR
		LDR		R1, [R2, #UART3_LSR_OFFSET]
		TST		R1, #1
		BEQ		UART3_Void		
		LDR		R0, [R2, #UART3_RBR_OFFSET]
		BX      	LR
		
;------------------------------------------------------------------------------------------------------------
UART3_PutChar
		LDR		R2, =UART3_BASE_ADDR
CheckUart3TxReady		
		LDR		R1, [R2, #UART3_LSR_OFFSET]
		TST		R1, #0x20	
		BEQ		CheckUart3TxReady
		STR		R0, [R2, #UART3_THR_OFFSET]
		BX      	LR		
		
		
Delay
		SUBS	R0, R0, #1
		BNE		Delay
		
		BX		LR
		

;------------------------------------------------------------------------------------------------------------
		END	;End of the program