#define ULCON0						(*(volatile unsigned int *)0x50000000)	/* UART LINE CONTROL REGISTER(ULCONn) */
#define UCON0						(*(volatile unsigned int *)0x50000004)	/* UART CONTROL REGISTER(UCONn) */
#define UTRSTAT0					(*(volatile unsigned int *)0x50000010)	/* UART TX/RX STATUS REGISTER(UTRSTATn) */
#define UTXH0						(*(volatile unsigned int *)0x50000020)	/* UART TRANSMIT BUFFER REGISTER(HOLDING REGISTER & FIFO REGISTER)*/
#define URXH0						(*(volatile unsigned int *)0x50000024)	/* UART RECEIVE BUFFER REGISTER(HOLDING REGISTER & FIFO REGISTER)*/
#define UBRDIV0						(*(volatile unsigned int *)0x50000028)	/* UART BAUD RATE DIVISOR REGISTER(UBRDIVn) */

#define GPHCON						(*(volatile unsigned int *)0x56000070)	/* PORT H CONTROL REGISTER(GPHCON, GPHDAT) */
#define MPLLCON						(*(volatile unsigned int *)0x4c000004)
#define CLKDIVN						(*(volatile unsigned int *)0x4c000014)



#define BAUD_RATE_DIV   				((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)
#define PCLK            				50000000    				// set PCLK = 50MHz
#define UART_CLK        				PCLK        				//set PLCK clock
#define UART_BAUD_RATE  				115200     				// baud rate


/* **************************************************************
*								*
*	uart_init:This function is used to init UART. 		*
*  								*
***************************************************************** */
void uart_init(void)
{
	//GPHCON = 0x2 << 6 | 0x2 << 4;							//set work mode
	GPHCON = 0x2<<6 | 0x2<<4 | 0x2<<2 | 0x2<<0;
	
	ULCON0 = 0x3 << 0;								//set frame mode

	UCON0 = 0x1 << 2 | 0x1 << 0;							//set uart control register
	
	UBRDIV0 = BAUD_RATE_DIV;							//set baud rate div

	MPLLCON = 0x44 << 12 | 0x1 << 4 | 0x1 << 0;					//set PLL control register
	
	CLKDIVN = 0x3 << 1 | 0x1 << 0;							//set clock divider control register
}

/* **************************************************************
*								*
*	uart_getchar:	This function is used to get a  	*
*			character from terminal user.		*
*  								*
***************************************************************** */
unsigned char uart_getchar(void)
{	
	while(!(UTRSTAT0 & 0x1));

	return URXH0;
}

/* **************************************************************
*								*
*	uart_putchar:	This function is used to put a  	*
*			character to terminal user.		*
*  								*
***************************************************************** */
void uart_putchar(unsigned char ch)
{	
	while(!(UTRSTAT0 & 0x2));

	UTXH0 = ch;
}
