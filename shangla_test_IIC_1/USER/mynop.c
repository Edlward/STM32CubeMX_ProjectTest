#include "mynop.h"

/* nop�ӳ�1΢�� */
void nop_delay_1us()
{
	uint8_t i = 0;
	
	for(i=0; i<6 ; i++)
	{
	__NOP();
	}
}

/* nop�ӳ�2΢�� */
void nop_delay_2us()
{
	uint8_t i = 0;
	
	for(i=0; i<12 ; i++)
	{
	__NOP();__NOP();
	}
}


void nop_delay_4us()
{
	uint8_t i = 0;
	
	for(i=0; i<23 ; i++)
	{
	__NOP();__NOP();
	}
}





