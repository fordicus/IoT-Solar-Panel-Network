#include <stdio.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "Serial.h"
#include "LED.h"

void consoleEntry(void);

void delay_ms(unsigned int times);
void readDip(void);
char* getOperationMode(unsigned int addr);

void UART3_SendString(unsigned char *Sdata);
void UART1_SendByte (unsigned char Data);
void UART1_SendString(unsigned char *Sdata);
void UART0_SendString(unsigned char *Sdata);

unsigned int dip[4];
unsigned int maxTerminal = 0;
unsigned int addr;
char* operationMode;
unsigned int txHolder = 0;
unsigned int UART_RxC = 0;

unsigned int testCnt = 0;
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main ( void )
{
	LED_Init();
	SER_Init();
	RS485_Init();
	DIP_SW_Init();
	RS485_SET_RX();
	
	//==========================================================
	// Zigbee Status
	//==========================================================
	/*
	while(1)
	{
		UART_RxC = UART3_GetChar();
		if (UART_RxC != 0)
		UART0_PutChar(UART_RxC);	
	}	
	*/
	
	consoleEntry();
	
	//==========================================================
	// Print Operation Mode
	//==========================================================
	readDip();
	operationMode = getOperationMode(addr);		
	
	printf("[system] Address : %d\r\n", addr);	
	printf("[system] Operation Mode : %s\r\n", operationMode);
		
	delay_ms(1000);
	
	//===============================================================================
	// setTarget
	//===============================================================================
	if (operationMode == "setTarget")
	{
		///*
		//==============================
		// Zigbee Configuration Mode
		//==============================
		while(1)
		{
			UART_RxC = UART0_GetChar();
			if ( UART_RxC != 0)
			{
				if (UART_RxC == '\r')
				{
					UART0_PutChar('\r');
					UART0_PutChar('\n');
					UART3_PutChar(UART_RxC);
				}
				else
				{
					UART0_PutChar(UART_RxC);
					UART3_PutChar(UART_RxC);
				}
			}
			else
			{
				UART_RxC = UART3_GetChar();
				if ( UART_RxC != 0)
				{
					UART0_PutChar(UART_RxC);
				}
			}	
		}
		//*/
		
		/*
		//==============================
		// Test SendString with Zigbee
		//==============================
		while (1)
		{	
			while(1)
			{
				UART_RxC = UART3_GetChar();
				if ( UART_RxC == ':') break;
			}
			
			UART3_SendString("AbCd");
			UART0_SendString("AbCd\r\n");
		}
		*/
		
	}
	
	//===============================================================================
	// uServer
	//===============================================================================
	else if (operationMode == "uServer")
	{
		printf("[system] maxTerminal : %d\r\n\r\n", maxTerminal);	
		
		while(1)
		{
			if ( txHolder < maxTerminal ) ++txHolder;
			else txHolder = 1;
						
			// Should Wait for a while until the line is not Busy !!
			delay_ms(1000);
			
			printf("[uServer] Request Status of : %d\r\n", txHolder);
			UART1_SendByte(txHolder+48);
						
			
			while(1)
			{
				UART_RxC = UART1_GetChar();
				if (UART_RxC != 0)
				{
					UART0_PutChar(UART_RxC);
					if (UART_RxC == 255)
					{
						UART0_SendString("\r\n\r\n");
						break;					
					}
				}				
			}
			
			
			
			
		}
	}
	//===============================================================================
	// uTerminal
	//===============================================================================
	else if (operationMode == "uTerminal")
	{
		UART0_SendString("\r\n");
		
		while(1)
		{
			UART_RxC = UART1_GetChar();
			
			// if (UART_RxC != 0) UART0_PutChar(UART_RxC);
			
			if ( UART_RxC == addr+48 )	// myTurn
			{
				UART0_SendString("[system] my Turn\r\n");
				// delay_ms(5);
				
				UART1_SendString("[uTerminal - ");
				UART1_SendByte(addr+48);
				UART1_SendString("] :\r\n");
				// delay_ms(10);
				
				UART3_PutChar(':');
				UART3_PutChar('\r');
				
				while(1)	// Receive by Byte (EOB == '\r') (EOL == ';')
				{					
					
					UART_RxC = UART3_GetChar();
					if (UART_RxC != 0)
					{
						UART0_PutChar(UART_RxC);
						UART1_SendByte(UART_RxC);
						
							if (UART_RxC == ';')
						{
							UART0_SendString("\r\n\r\n");
							break;
						}
					}
					
					
				}
					
					
				
				
				UART1_SendByte(255); // End of Transmission
				UART_RxC = 0;
			}
			else
			{
				UART0_SendString("[system] waiting Request\r\n");
				delay_ms(1000);	
			}
			
		}
	}
	//===============================================================================
	// uPanel
	//===============================================================================
	else if (operationMode == "uPanel")
	{
		while (1)
		{

			///*
			UART_RxC = 0;
			while(1)
			{
				UART_RxC = UART3_GetChar();
				if ( UART_RxC == ':') break;
			}
			

			UART3_SendString(" >> Test ADC Val. 3v");
			UART0_SendString("Test ADC Val. 3v\r\n");
			//*/
			
			/*
			//=======================================
			// Arbitrary Integer Test
			//=======================================
			if ( testCnt > 8) testCnt = 0;
			else ++testCnt;
			
			
			UART3_PutChar(testCnt+48);
			UART3_PutChar(';');
			UART3_PutChar('\r');
			
			delay_ms(1000);
			
			printf("[system] testCnt : %d\r\n", testCnt);
			*/
			
		}
			
			
			
			
	}
	
	

}


//==================================================================================
// C - Functions
//==================================================================================


//----------------------------------------------------------------------------------
// delay_ms() : 1ms basis delay at 4MHz default frequency by internal clock source of LPC176x
//----------------------------------------------------------------------------------
void delay_ms(unsigned int times)
{
	unsigned int delay = times * 33333;
	Delay(delay);
}

//----------------------------------------------------------------------------------
// First Console Print Outs
//----------------------------------------------------------------------------------
void consoleEntry(void)
{
	UART0_SendString("\r\n\r\n ================================\r\n");
	UART0_SendString("   Solar Panel Monitoring System\r\n");
	UART0_SendString(" ================================\r\n\r\n");
}


//----------------------------------------------------------------------------------
// Read Dip-Switch and returns (unsigned int)addr
//----------------------------------------------------------------------------------
	void readDip(void)
	{
		#define	DIP3		0x1							// P0.0
		#define	DIP2		0x2							// P0.1
		#define	DIP1		0x200000			// P0.21
		#define	DIP0		0x400000			// P0.22
				
		  dip[3] = (DIP_value(DIP3) << 3);
			dip[2] = (DIP_value(DIP2) << 1);
			dip[1] = (DIP_value(DIP1) >> 20);
			dip[0] = (DIP_value(DIP0) >> 22);
			
			// High Position of Dip-SW pulls down the signal (ON)
			// Hence invert it for better intuitive manipulation
			// Range of DIP Switch 4bits : 0b1111 ~ 0b0000 (Little Endian)
			
			addr = ~(
				dip[3]		// 0b1000
			+ dip[2]		// 0b0100
			+ dip[1]		// 0b0010
			+ dip[0])		// 0b0001
			& 0xF;
		
		maxTerminal = addr & 0x7;		
	}


//----------------------------------------------------------------------------------
// Read Dip-Switch and returns (unsigned int)addr
//----------------------------------------------------------------------------------
char* getOperationMode(unsigned int addr)
	{
		char* operationMode;
		
		if ( addr == 15) operationMode = "setTarget";
		else if ( addr >= 8 && addr != 15) operationMode = "uServer";
		else if ( addr == 0 ) operationMode = "uPanel";
		else operationMode = "uTerminal";
		
		
		return operationMode;
	}


//----------------------------------------------------------------------------------
// UART3 Send String
//----------------------------------------------------------------------------------
void UART3_SendString(unsigned char *Sdata)
{
	while(*Sdata != '\0') 
	{
		UART3_PutChar(*Sdata++);
		// delay_ms(10);
	}
	UART3_PutChar(';');
	UART3_PutChar('\r');
	// delay_ms(500);
}
	
//----------------------------------------------------------------------------------
// UART1 Send Byte
//----------------------------------------------------------------------------------
void UART1_SendByte (unsigned char Data)
{
	RS485_SET_TX();
	UART1_PutChar(Data);
	
	//delay should be reserved
	delay_ms(2);
	RS485_SET_RX();
}


//----------------------------------------------------------------------------------
// UART1 Send String
//----------------------------------------------------------------------------------
void UART1_SendString(unsigned char *Sdata)
{
	while(*Sdata != '\0') UART1_SendByte(*Sdata++);
	// delay_ms(10);
}


//----------------------------------------------------------------------------------
// UART0 Send String
//----------------------------------------------------------------------------------
void UART0_SendString(unsigned char *Sdata)
{
	while(*Sdata != '\0') UART0_PutChar(*Sdata++);
}
