/*
 * std_types.h
 *
 *  Created on: Oct 21, 2019
 *      Author: Sprints
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


/************************************************************************/
/*                   typedefs for standard types                        */
/************************************************************************/

#define NULL	((void *)0)

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char sint8_t;
typedef signed int sint16_t;
typedef signed long int sint32_t;
typedef signed long long sint64_t;
typedef volatile uint8_t* const reg_type8_t;
typedef volatile uint16_t* const reg_type16_t;


/************************************************************************/
/*				 		defining boolean values                         */
/************************************************************************/
#define FALSE 	0	// defines boolean false
#define TRUE 	1	// defines boolean true


/************************************************************************/
/*						 LOW/HIGH defines                               */
/************************************************************************/
#define LOW  0		// defines LOW value for a bit
#define HIGH 0xFF	// defines HIGH value for a bit

#define  ZERO     0
#define	 ONE	  1
#define SET_BIT(number,bit)			((number)|=((1)<<(bit)))
#define CLEAR_BIT(number,bit)		((number)&=(~((1)<<(bit))))
#define TOGGLE(number,bit)			((number) ^ ((1)<<(bit)))
#define SET_MASK(number,Mask)       ((number)|=(Mask))
#define CLEAR_MASK(number,Mask)     ((number)&=(~(1<<Mask)))
#define TOGGLE_MASK(number,Mask)    ((number) ^ (1<<Mask))
#define BIT_IS_SET(number,bit)  ((number)&(1<<(bit)))
#define BIT_IS_CLEARD(number,bit)   (!((number)&(1<<(bit))))

#endif /* STD_TYPES_H_ */