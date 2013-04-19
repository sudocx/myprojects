#include "uart.h"

/* **********************************************************************
*									*
*	getchar: This function is used to get a character.		*
*  									*
*************************************************************************/
char getchar(void)
{
	return uart_getchar();	
}

/* **********************************************************************
*								 	*
*	putchar: This function is used to output a character.		*
*  									*
*************************************************************************/
void putchar(char ch)
{
	uart_putchar(ch);	
}

/* **********************************************************************
 *									*
 *	puts: This function is used to output a character string.	*
 *  									*
*************************************************************************/
void puts(char *string)
{ 
	char *str = string;
	while(*str != '\0')
	{
		uart_putchar(*str);
		str++;
	}
}

/* **********************************************************************
*									*
*	gets: This function is used to get a character string. 		*
*  									*
*************************************************************************/
void gets(char *string)
{
	char *buf = string;
	int len = 0;

	while(1)
	{
		if((*buf = uart_getchar()) == '\r')
		{
			putchar('\r');
			putchar('\n');
			break;
		}

		if(*buf == '\b')
		{
			if(len == 0)
				continue;
			
			len -= 1;
			
			putchar('\b');
			putchar(' ');
			putchar('\b');

			*buf = '\0';
			buf -= 1;

		}
		else
		{
			len += 1;
			putchar(*buf);
			buf += 1;	
		}
	}
	*buf = '\0';
}