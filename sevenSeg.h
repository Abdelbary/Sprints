/*
 * sevenSeg_BCD.h
 *
 *  Created on: Oct 24, 2019
 *      Author: Sprints
 */

#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include "gpio.h"
#include "sevenSegConfig.h"


typedef enum En_SevenSegId_t{
	SEG_0,
	SEG_1,
}En_SevenSegId_t;

/**
 * Description: initiate seven segment BCD
 * @param : En_SevenSegId_t: -SEG0
 *							 -SEG1	
 */
void sevenSegInit(En_SevenSegId_t a_segment_id);

/**
 * Description: enable the enable pins on specified seg
 * @param : En_SevenSegId_t: -SEG0
 *						       -SEG1
 */
void sevenSegEnable(En_SevenSegId_t en_segment_id);

/**
 * Description: disable enable pin on specified seg
 * @param En_SevenSegId_t: -SEG0
 *						   -SEG1
 */
void sevenSegDisable(En_SevenSegId_t en_segment_id);

/**
 * Description: wirte value form 0-9 to 7-SEG
 * @param En_SevenSegId_t: -SEG0
 *						   -SEG1
 *        u8_number:       -0->9
 */
void sevenSegWrite(En_SevenSegId_t en_segment_id, uint8_t u8_number);


#endif /* SEVENSEG_BCD_H_ */
