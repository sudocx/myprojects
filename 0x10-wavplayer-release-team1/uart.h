#ifndef	UART0_H
#define UART0_H 

extern void uart_init(void);
extern void uart_putchar(char ch);
extern char uart_getchar(void);

#endif	//UART0_H