
#include "stdio.h"

/* type definitions */
typedef char *va_list;

/* macros */
#define va_arg(ap, T) \
	(* (T *)(((ap) += _Bnd(T, 3U)) - _Bnd(T, 3U)))
#define va_end(ap) (void)0

#define va_start(ap, A) \
	(void)((ap) = (char *)&(A) + _Bnd(A, 3U))

#define _Bnd(X, bnd) (sizeof (X) + (bnd) & ~(bnd))

/* **************************************************************
 *								*
 *	itoa: This function is used to transform data form. 	*
 *  								*
*************************************************************** */
char *itoa(int num, int base)
{
	char buf[20];
	char temp, *tbuf = buf;
	unsigned int n;
	int i, j, digit;
	
	/* If num is a negative number */
	if(num < 0 && base == 10)
	{
		buf[0] = '-';
		tbuf++;
		n = -num;	
	}
	
	n = num;
	for(i = 0; n > 0; i++)
	{
		/* If num is a hexadecimal number */
		if(base == 16)
		{
			digit = n % 16;
			n /= 16;
		}
		
		/* If num is a decimal number */
		if(base == 10)
		{
			digit = n % 10;
			n /= 10;
		}
		if(digit > 9) tbuf[i] = digit - 10 + 'a';
		else tbuf[i] = digit + '0';
	}
	
	tbuf[i] = 0;
	
	for(j = 0 ;j < i/2; j++)
	{
		temp = tbuf[j];
		tbuf[j] = tbuf[i - 1 - j];
		tbuf[i - 1 - j] = temp;
	}
	if(!num)
			tbuf[i++] = '0';
	tbuf[i] = 0;
	return buf;
}

/* **************************************************************
 *								*
 *	myprintf: This function is used to formating output. 	*
 *  								*
*************************************************************** */
void myprintf(const char *format, ...)
{
	va_list ap;
	char ch;
	int c, i;
	int state = 0;
	va_start(ap, format);

	while(*format) 
	{
		switch(*format) 
		{
		case '%':
			if(state == 0)
			{
				state = 1;
				break;
			}
			
			putchar(ch);
			state = 1;
			break;
			
		case 'c':
			if(state == 0)
			{
				putchar(ch); 
				break;
			}

			c = va_arg(ap,int);
			putchar(c);
			state = 0;
			break;
			
		case 's': 
			if(state == 0)
			{
				putchar(ch);
				break;
			}
			
			puts(va_arg(ap, char *));
			state = 0;
			break;
	
		case 'd':
			if(state == 0)
			{
				putchar(ch);
				break;
			}
			
			i = va_arg(ap, int );
			puts(itoa(i, 10));
			state = 0;			  
			break;
	
		case 'x': 
			if(state == 0)
			{
				putchar(ch);
				break;
			}
			
			i= va_arg(ap, int);
			//puts("0x");
			if(i < 16)
				putchar('0');
			puts(itoa(i, 16));
			state = 0;
			break;
			
		case '\n': 
			if(state == 0)
			{
				putchar('\r');
				putchar('\n');
			}
			
			state = 0;
			break;
					
		default:
			putchar(*format);
			break;
		}
		format++;
	}
	va_end(ap);
}

