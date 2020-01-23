/*
 * sevebSeg.h
 *
 * Created: 1/17/2020 10:42:16 AM
 *  Author: mahmo
 */ 

#include "sevenSeg.h"
void sevenSegInit(En_SevenSegId_t a_segment_id)
{
	/*
	* Set SEG BCD direction
	* Set SEG BCD DP BIT direction
	* set SEG EN pin
	*/
	gpioPinDirection(SEG_BCD_DP_GPIO,SEG_BCD_BITS,OUTPUT);
	gpioPinDirection(SEG_BCD_DP_GPIO,SEG_BCD_DP_BIT,OUTPUT);
	gpioPinDirection(SEG_EN1_GPIO,SEG_EN1_BIT,OUTPUT);
	gpioPinDirection(SEG_EN2_GPIO,SEG_EN2_BIT,OUTPUT);
	
}

void sevenSegEnable(En_SevenSegId_t en_segment_id)
{
	switch(en_segment_id)
	{
		case SEG_0:
			gpioPinWrite(SEG_EN1_GPIO,SEG_EN1_BIT,HIGH);
		break;
		case SEG_1:
			gpioPinWrite(SEG_EN2_GPIO,SEG_EN2_BIT,HIGH);
		break;
	}
}

void sevenSegDisable(En_SevenSegId_t en_segment_id)
{
		switch(en_segment_id)
		{
			case SEG_0:
			gpioPinWrite(SEG_EN1_GPIO,SEG_EN1_BIT,LOW);
			break;
			case SEG_1:
			gpioPinWrite(SEG_EN2_GPIO,SEG_EN2_BIT,LOW);
			break;
		}
}

void sevenSegWrite(En_SevenSegId_t en_segment_id, uint8_t u8_number)
{
	switch(en_segment_id)
	{
		case SEG_0:
		sevenSegEnable(SEG_0);
		break;
		case SEG_1:
		sevenSegEnable(SEG_0);
		break;
	}
		//gpioPinWrite(SEG_BCD_DP_BIT,SEG_BCD_DP_BIT,HIGH);
		gpioPinWrite(SEG_BCD_GPIO,u8_number,HIGH);

}