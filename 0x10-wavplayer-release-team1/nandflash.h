#ifndef _NANDFLASH_H_
#define _NANDFLASH_H_


#define NFCONF		(*(volatile unsigned int *)0x4e000000)	/* NAND flash configuration register*/
#define NFCONT		(*(volatile unsigned int *)0x4e000004)	/* NAND flash control register */

#define NFCMD		(*(volatile unsigned char *)0x4e000008)	/* NAND flash command */
#define NFADDR    	(*(volatile unsigned char *)0x4e00000c)	/* NAND flash address */
#define NFDATA    	(*(volatile unsigned char *)0x4E000010)	/* NAND flash data register */
#define NFSTAT    	(*(volatile unsigned char *)0x4E000020)


void nand_init(void);
void read_id(void);
void read_page(int block, int page, char *ptr);
void nand_program(int block, int page, char *ptr);
void eraseblock(int block);
void flashl(void);
void clear_memory(void);
void flashw(void);
void dma_init(void);


#endif