#define SCS							(uint32_t*)(0x400FC1A0UL)
#define CLKSRCSEL	(uint32_t*)(0x400FC1A0UL)
#define PLL0CFG			(uint32_t*)(0x400FC084UL)
#define PLL0CON			(uint32_t*)(0x400FC080UL)
#define PLL0FEED			(uint32_t*)(0x400FC08CUL)
#define PLL0STAT			(uint32_t*)(0x400FC088UL)
#define CCLKCFG			(uint32_t*)(0x400FC104UL)
#define PCLKSEL0  	(uint32_t*)(0x400FC1A8UL)
#define PCONP					(uint32_t*)(0x400FC0C4UL)
#define PINSEL0				(uint32_t*)(0x4002C000UL)
//------------------------------------------------------
#define U0LCR					(uint32_t*)(0x4000C00CUL)	// 0x4000 C00C
#define U0DLL					(uint32_t*)(0x4000C000UL)	// 0x4000 C000 : DLAB = 1
#define U0DLM					(uint32_t*)(0x4000C004UL)	// 0x4000 C004 : DLAB = 1
#define U0FDR					(uint32_t*)(0x4000C028UL)	// 0x4000 C028
#define U0FCR					(uint32_t*)(0x4000C008UL)	// 0x4000 C008
#define U0LSR					(uint32_t*)(0x4000C14CUL)	// 0x4000 C014
#define U0RBR					(uint32_t*)(0x4000C00CUL)	// 0x4000 C000
#define U0THR					(uint32_t*)(0x4000C00CUL)	// 0x4000 C000
//------------------------------------------------------

void systemInit (void)
{
  *SCS = 0x00000020 ;
	*CLKSRCSEL = 0x00000001;
	*PLL0CFG   = 0x0000000B;
	*PLL0CON   = 0x01;
	*PLL0FEED  = 0xAA;
	*PLL0FEED  = 0x55;
	while (!((*PLL0STAT) & (1 << 26)));
	*CCLKCFG   = 0x00000003;
	*PLL0CON   = 0x03;
	*PLL0FEED  = 0xAA;
  *PLL0FEED  = 0x55;
}

void UART0_Init (void)
{
	*PCLKSEL0  = 0x00000000;
	*PCONP     = 0x042887DE;
	
	*PINSEL0 |= (1 << 4);             			/* Pin P0.2 used as TXD0 (Com0) */
	*PINSEL0 |= (1 << 6);             			/* Pin P0.3 used as RXD0 (Com0) */  
  	
	*U0LCR = 0x83;	// [ DLAB = 1 ] 8 bits, no Parity, 1 Stop bit
	*U0DLL = 4;		// 115200 Baud Rate @ 12.0MHZ CLK
	*U0DLM = 0;
	*U0FDR = 0x85;    
	*U0LCR = 0x03;	// [ DLAB = 0 ]
	*U0FCR = 0x06;	// FIFO Reset(Logic_High)
}
