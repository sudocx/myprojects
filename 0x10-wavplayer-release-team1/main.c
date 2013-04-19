/* ******************************************************************************
*		Date: 		20-12-2010					*
*		Author: 	TWave                               		*
*		Purpose: 	This project is used to complete  		*
*				communication bettwen s3c2440 and 		*
*				terminal user.					*
*				Including stdandad input/output on		*
*				Asynchronous serial of UART			*	
*										*
**********************************************************************************/

#include "uart.h"
#include "stdio.h"
#include "command.h"
#include "nandflash.h"


/* GPB(IO PORTS) SPECIAL REGISTER */
#define 		GPBCON					(*(volatile unsigned int *)0x56000010)
#define 		GPBDAT					(*(volatile unsigned int *)0x56000014)

/* WATCHDOG TIMER SPECIAL REGISTER */
#define 		WTCON					(*(volatile unsigned int *)0x53000000)

/* LEDDATA SPECIAL REGISTER */
#define 		LEDDATA					(*(volatile unsigned char *)0x20800000)

#define 		BWSCON					(*(volatile unsigned int *)0x48000000)

//IIS
#define 		IISCON					(*(volatile unsigned int *)0x55000000)
#define 		IISMOD					(*(volatile unsigned int *)0x55000004)
#define 		IISPSR					(*(volatile unsigned int *)0x55000008)
#define 		IISFCON					(*(volatile unsigned int *)0x5500000C)
#define 		IISFIFO					((volatile unsigned int *)0x55000010)


/* set uda1341 L3 bus */
#define 		L3C (1<<4)              		//GPB4 = L3CLOCK
#define 		L3D (1<<3)              		//GPB3 = L3DATA
#define 		L3M (1<<2)              		//GPB2 = L3MODE

#define 		L3DATA(v)				(v==1)? (GPBDAT |=L3D): (GPBDAT &= ~(L3D))
#define 		L3CLOCK(v)				(v==1)? (GPBDAT |=L3C): (GPBDAT &= ~(L3C))
#define 		L3MODE(v)				(v==1)? (GPBDAT |=L3M): (GPBDAT &= ~(L3M))


#define 		GPECON    				(*(volatile unsigned *)0x56000040)	//Port E control
#define 		GPEDAT    				(*(volatile unsigned *)0x56000044)	//Port E data
#define 		GPEDAT    				(*(volatile unsigned *)0x56000044)	//Port E data

/* IIS */
#define 		IISCON					(*(volatile unsigned int *)0x55000000)
#define 		IISMOD					(*(volatile unsigned int *)0x55000004)
#define 		IISPSR					(*(volatile unsigned int *)0x55000008)
#define 		IISFCON					(*(volatile unsigned int *)0x5500000C)
#define 		IISFIFO					((volatile unsigned int *)0x55000010)

/* DMA */
#define  		DISRC2					(*(volatile unsigned int *)0x4B000080)
#define 		DISRCC2					(*(volatile unsigned int *)0x4B000084)
#define 		DIDST2					(*(volatile unsigned int *)0x4B000088)
#define 		DIDSTC2					(*(volatile unsigned int *)0x4B00008C)
#define  		DCON2					(*(volatile unsigned int *)0x4B000090)
#define  		DSTAT2					(*(volatile unsigned int *)0x4B000094)
#define  		DMASKTRIG2				(*(volatile unsigned int *)0x4B0000A0)

#define  		SRCPND					(*(volatile unsigned int *)0x4A000000)
#define  		INTMSK					(*(volatile unsigned int *)0x4A000008)
#define  		INTPND					(*(volatile unsigned int *)0x4A000010)
#define  		DMASKTRIG0				(*(volatile unsigned int *)0x4A000020)

#define 		LOAD_WAV_ADDR 				0x31000000

void L3_write(char data)
{		
int i, j;
	
L3CLOCK(1);	

for(j=0;j<4;j++);                   	/*tsu(L3) > 190ns*/

for(i=0;i<8;i++)                    	/*LSB first*/
{
	L3DATA( data & 0x1 );
  
	L3CLOCK(0);          	
	for(j=0;j<4;j++);               /*delay for some time 190ns*/
	L3CLOCK(1);          	
	for(j=0;j<4;j++);          	/*delay for some time*/

	data >>= 1;
}
L3CLOCK(1);	
}

void L3_write_addr(char addr)
{
	L3MODE(0);
	L3_write(addr);
}

void L3_write_data(char data)
{
	L3MODE(1);
	L3_write(data);
}

void L3_init(void)
{
	L3_write_addr(0x14 + 2);	/*STATUS (000101xx+10)*/
	L3_write_data(0x50);		/*0,1,01, 000,0 : Status 0,Reset,384fs,IIS-bus,no DC-filtering*/
}

void iis_init(void)
{
	IISPSR = (5<<5)|5;	
	
	IISCON  = (1<<2) | (1<<1) | (1 << 5) | (1 << 4);	
	
	IISMOD = (0<<8)+(2<<6)+(0<<5)+(1<<4)+(1<<3)+(1<<2)+(1<<0);
	IISFCON = (1<<13);   
	IISFCON &= ~(1<<15);
	IISFCON = (1 << 13)  + (1 << 15);
	IISCON |= 0x1; 

	GPECON |= 1<<1 | 1<<3 | 1<<5 | 1<<7 | 1<<9;	
	GPECON &= ~(1<<0 | 1<<2 | 1<<4 | 1<<6 | 1<<8);
}


/* ******************************************************************************
*										*
*		periphera_init:	This function is used to init periphera 	*
*				equipment, such as BEEP, WATCHDOG, LED		*
*  										*
**********************************************************************************/
void periphera_init()
{
	GPBCON = 0x1 << 0;			/*set GPBCON*/
		
	GPBDAT = 0x0;				/*close BEEP*/

	WTCON = 0x0 << 0;			/*close WATCHDOG*/

	LEDDATA = 0xFF;				/*set LED*/
		
	BWSCON = 0x2 << 24; 			/*set*/
}


void dma_init(void)
{
	DISRC2 = LOAD_WAV_ADDR;

	DISRCC2 = 0x0;
		
	DIDST2 = 0x55000010;
		
	DIDSTC2 = 0x3;
	
	DCON2 = (1 << 29) + (0 << 24) + (1 << 23) + (1 << 22) + (1 << 20) + (*((int *)LOAD_WAV_ADDR + 1));
	DMASKTRIG2 = (0 << 2) + (1 << 1) + (0 << 0);
		
	INTMSK &= ~(1 << 19);
}

void __irq dma_handler(void)
{
	int i;

	GPBCON = 0x1;
		
	GPBDAT = 0x1;
	for(i = 0; i < 0xffff; i++);
		GPBDAT = 0x0;
	for(i = 0; i < 0xffff; i++);
		
	SRCPND = (1 << 19); 
	INTPND = (1 << 19);
		
	DMASKTRIG0 = (1 << 1);	//DMASKTRIG0
}

/* ******************************************************************************
*										*
*		mymain:	This is a Main Function. 				*
*  										*
**********************************************************************************/
int mymain()
{
	char str[10] = { 0 };
	char *name = "TWave $";
	
	periphera_init();			
		
	GPBCON |= 1<<4 | 1<<6 | 1<<8;
	GPBCON &= ~(1<<3 | 1<<5 | 1<<7);
		
	uart_init();
		
	L3_init();
	iis_init();
	nand_init();

	read_id();
		
	puts("\r\n");		
		
	while(1)
	{
		puts(name);
		gets(str);	
		operation(str);
	}
		
	return 0;
}