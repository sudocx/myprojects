#ifndef _COMMAND_H_
#define _COMMAND_H_


#define BIN 					264 					/* BIN FILE LENGTH */
#define NULL 					(void *)0
#define HELP 					"--TWave--HELP--\r\ns3c2440 help manual\r\n1>\r\n"
#define FLASHW 					"-flashw-    	program data from mem to nandflash\r\n"
#define FLASHL					"-flashl-  	load data from nandflash to mem\r\n"
#define DMA					"-dma-  	play music in dma-mod(if there is any music base block 2)\r\n"

#define LOAD_WAV_ADDR 				0x31000000

int md(int add);
int operation(char *opt_cmd);
void playwav(void);



#endif