#include "stdio.h"
#include "command.h"
#include "nandflash.h"

//IIS
#define IISCON							(*(volatile unsigned int *)0x55000000)
#define IISMOD							(*(volatile unsigned int *)0x55000004)
#define IISPSR							(*(volatile unsigned int *)0x55000008)
#define IISFCON							(*(volatile unsigned int *)0x5500000C)
#define IISFIFO							((volatile unsigned int *)0x55000010)
#define LLED							(*((char *)0x20800000))


/* **************************************************************
*								*
*	strcmp: This function is used to compare length 	*
*		bettwen two character strings. 			*	
*		If the first string's value is equal with 	*
*		the sencond will return 0, smaller return -1,	*
*		greater return 1.				*
*								*
*****************************************************************/
int strcmp(char * ptr1, char * ptr2)
{
	while(*ptr1 && *ptr2)
	{
		if(*ptr1 == *ptr2)
		{
				ptr1++;
				ptr2++;
		}
		if(*ptr1 > *ptr2)
				return 1;
		if(*ptr1 < *ptr2)
				return -1;
	}
	
	return 0;
}



/* **************************************************************
*								*
*	md: 	This function is used to print address 		*
*			information and the stored data  	*
*								*	
*****************************************************************/
#if 1
int md(int add)
{
	int i = 0;
	/*If you define like this , it can only print 1 byte*/
	char *p = (char *)add;    
	
	//volatile char *p = (char *)add;
	myprintf("%xh:", add);
	
	for(i = 0; i < 16; i++)
			myprintf("%x ", *(p + i));
			
	puts(" ; ");
	for(i = 0; i < 16; i++)
			putchar(*(p + i));
			
	puts("\r\n");		
	return 1;
}
#endif
#if 0
int md(int add)
{
	volatile char *p =(volatile char *)add;

	int i;
	myprintf("%xh: ", add);

	for(i = 0; i < 16; i++)
	{
		if(*(p+i) < 16)
		{
			putchar('0');
			myprintf("%x",*(p+i));	
		}
		else
			myprintf("%x",*(p+i));	

		putchar(' ');
	}
	putchar(';');
	putchar(' ');
	
	puts("\r\n");
	return 1;
}
#endif
/* **************************************************************
*								*
*	loadwave: This function is used to load *.wave file. 	*
*								*	
*****************************************************************/
int loadwav(void)
{
	char *p = (char *)LOAD_WAV_ADDR;
	int len = 0;
	int i = 0;
	
	for(i = 0; i < 8; i++)
			*(p + i) = getchar();

	len = *((int *)(p + 4));

	for(i = 8; i < len + 8; i++)
			*(p + i) = getchar();
		
	md((int)p);
	md((int)(p + len - len % 16));

	return len;
}


void playwav(void)
{
	unsigned short *pfifo;
	int size, count;
	
	count = 0;
	size = *((int *)0x31000004);
	pfifo = (unsigned short *)LOAD_WAV_ADDR;
	
	while((int)pfifo <= (LOAD_WAV_ADDR + size))
	{
		while((IISCON & (1<<7)) == (1<<7))
					;	
		*IISFIFO = *pfifo;
		pfifo++;
		count++;
		
		if(!(count % 32))
				LLED = ((count <<= 1)?  count : (count = 0x1));
	}		
	

}

/* **************************************************************
*								*
*	operation: This function is used to execute command.    *
*  								*
*****************************************************************/
int operation(char *opt_cmd)
{
	char *p = (char *)0xC00;
	int i = 0;

	if(opt_cmd == NULL)
		return -1;
		
	if(!strcmp("dma", opt_cmd))
	{
		myprintf("dma begin\r\n");
		dma_init();

		return 1;
	}
	
	if(!strcmp("go", opt_cmd))
	{
		((void (*)(void))(p))();
		
		return 1;
	}

	if(!strcmp("loadb", opt_cmd))
{
		for(i = 0; i < BIN; i++)
			*(p + i) = getchar();
		
		return 1;
	}

	if(!strcmp("help", opt_cmd))
	{
		puts(HELP);
		puts(FLASHW);
		puts(FLASHL);
		puts(DMA);
		return 1;
	}
	
	if(!strcmp("md", opt_cmd))
	{
		return 1;		
	}

	if(!strcmp("loadwav", opt_cmd))
	{	
		return loadwav();
	}
	
	if(!strcmp("flashw", opt_cmd))
	{						
		flashw();
	}
	
	if(!strcmp("flashl", opt_cmd))
	{	
		flashl();
	}
	
	if(!strcmp("playwav", opt_cmd))
	{	
		playwav();
	}
	
	return 0;
}