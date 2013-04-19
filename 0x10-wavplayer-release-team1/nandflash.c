#include "stdio.h"
#include "command.h"
#include "nandflash.h"


#define 	PAGE_SIZE 		512 
#define 	BUSY 			1

/* Load test */
#define 	LLED			(*((char *)0x20800000))

void nand_init(void)
{
	NFCONF = 0x1400;
	NFCONT |= 1 << 0;				
	NFCONT &= ~(1 << 1);		
	NFCONT &= ~(1 << 12);	
		
	NFCMD = 0xFF;
	while(!(NFSTAT & 0x1));
		NFSTAT |= BUSY;
}

void read_id(void)
{
	int i;
	
	char buf;
	nand_init();
		
	NFCMD = 0xFF;				
	
	NFCMD = 0x90;			

	NFADDR = 0x00;					
	
	for(i = 0; i < 4; i++)
	{
		buf = (NFDATA & 0xFF);
		myprintf("%x ",buf);
	}
	
}


/* ******************************************************************************
*										*
*	read_page:	This function is used to read data			* 
*			from Nandflash. 					*
*  										   *
**********************************************************************************/
void read_page(int block, int page, char *ptr)
{
	int i, j;
	int addr;
		
	addr = (block * 32 + page) * 512;
		
	NFCONT &= ~(1<<1);			
	
		NFCMD = 0;	

	NFADDR = (unsigned char)addr;
	NFADDR = (unsigned char)(addr >> 9);
	NFADDR = (unsigned char)(addr >> 17);
	NFADDR = (unsigned char)(addr >> 25);
							
	while(!(NFSTAT & 0x1));

	for(i = 0; i < 512; i++)
	{
		*(ptr + i) = (NFDATA & 0xFF);
		if(i % 256 == 0)
			LLED = ((j <<= 1)?  j: (j = 0x1));
	}
	
	NFCONT |= 1 << 1;		
}

#if 0
void eraseblock(int block)
{
	int block_addr = block * 32 * 512;
	char stat;
	
	NFCONT &= ~(1<<1);
	
	NFCMD = 0x60;				//erase cmd

	NFADDR = (unsigned char)block_addr;
	NFADDR = (unsigned char)(block_addr >> 9);
	NFADDR = (unsigned char)(block_addr >> 17);
	NFADDR = (unsigned char)(block_addr >> 25);
	
	NFCMD = 0xD0;
	
	NFCMD = 0x70;		
	do {
			stat = (NFDATA & 0xff);
	}while(!(stat & 0x1));

	NFCMD = 0;	
}
#endif


/* ******************************************************************************
*										*
*	nand_program:	This function is used to program data			* 
*			from memory to Nandflash. 				*
*  										*
**********************************************************************************/
void nand_program(int block, int page, char *ptr)
{
	int i = 0, j = 0x1;
	int addr;
	
	addr = (block * 32 + page) * 512;
	NFCMD = 0x80;
	/* Transmit address information */
	NFADDR = (unsigned char)addr;
	NFADDR = (unsigned char)(addr >> 9);
	NFADDR = (unsigned char)(addr >> 17);
	NFADDR = (unsigned char)(addr >> 25);

	for(i = 0; i < PAGE_SIZE; i++)
	{
		NFDATA = *(ptr + i);
		if(i % 32 == 0)
			LLED = ((j <<= 1)?  j: (j = 0x1));
	}
	
	NFCMD = 0x10;
	
	while(!(NFSTAT & (1 << 0)));
			
}


/* ******************************************************************************
*										*
*	flashl:	This function is used to load data				* 
*		from Nandflash to memory. 					*
*  										*
**********************************************************************************/
void flashl(void)
{
	int i, j, k;
	int size, block, page, page_all;
	
	read_page(2, 0, (char *)LOAD_WAV_ADDR);
	size = *((int *)(LOAD_WAV_ADDR + 4));
	myprintf(" size : %d\n\r", size);

	page_all = size / 512 + 1;
	block = page_all / 32;
	page = page_all % 32;
	
	for(j = 0; j < block ; j++)
	{
		for(i = 0; i < 32; i++)
		{
			read_page(2 + j, i,(char *)LOAD_WAV_ADDR + 512 * (i + 32*j));
		}
	}
	for(k = 0; k < page; k++)
	{
		read_page(block + 2, k, (char *)LOAD_WAV_ADDR + 512 * (32*j + k));
	}

}

/* ******************************************************************************
*										*
*	flashw:	This function is used to program data				* 
*		from memory to Nandflash. 					*
*  										*
**********************************************************************************/
void flashw(void)
{
	int i, j;
		
	for(j = 2; j < 60; j++)
		for(i = 0; i < 32; i++)
			nand_program(j, i, (char *)((char *)LOAD_WAV_ADDR + 512 * (i + 32*j)));
}