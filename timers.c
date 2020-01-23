/*
 * timers.c
 *
 * Created: 1/21/2020 4:44:04 AM
 *  Author: mahmo
 */ 
#include "timers.h"



#define TIMER0_PRESCALER_CLEAR_MASK		0x07
#define TIMER2_PRESCALER_CLEAR_MASK     0x07
#define F_CPU						    16
#define MICRO_SEC_DELAY_TICKS			250
#define TIMER0_RESLUTION				256
#define TCNT_DELAY_INIT_VALUE			(TIMER0_RESLUTION-MICRO_SEC_DELAY_TICKS)

static En_timer0perscaler_t	genu_timer0prescaler;
//static En_timer1perscaler_t genu_timer1prescaler;
static En_timer2perscaler_t genu_timer2prescaler;

typedef struct{
	En_timer0Mode_t			genu_timer0Mode;
	En_timer0OC_t           genu_timer0OC;
	En_timer0perscaler_t	genu_timer0Prescaler;
	En_timer0Interrupt_t	genu_timer0Interrupte;
	uint8_t					gu8_timer0InitialValue;
	uint8_t					gu8_timer0OutputCompare;
}gstrTimer0_t;	

typedef  struct{
	En_timer1Mode_t			genu_timer1Mode;
	En_timer1OC_t           genu_timer1OC;
	En_timer1perscaler_t	genu_timer1Prescaler;
	En_timer1Interrupt_t	genu_timer1Interrupte;
	uint8_t					gu8_timer1InitialValue;
	uint8_t					gu8_timer1OutputCompare;
}gstrTimer1_t;

typedef  struct{
	En_timer2Mode_t			genu_timer2Mode;
	En_timer2OC_t           genu_timer2OC;
	En_timer2perscaler_t	genu_timer2Prescaler;
	En_timer2Interrupt_t	genu_timer2Interrupte;
	uint8_t					gu8_timer2InitialValue;
	uint8_t					gu8_timer2OutputCompare;
}gstrTimer2_t;



/*===========================Timer0 Control===============================*/
/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer0Init(En_timer0Mode_t en_mode,En_timer0OC_t en_OC0,
En_timer0perscaler_t en_prescal, uint8_t u8_initialValue, 
uint8_t u8_outputCompare, En_timer0Interrupt_t en_interruptMask)
{
	
	/*saving prescaler value*/
	genu_timer0prescaler	= en_prescal;
	//genu_timer0Mode			= en_mode;
	
	/*zero all bits & registers*/
	TCCR0   =  ZERO;
	TCNT0   =  ZERO;
	CLEAR_BIT(TIMSK,TOIE0_BIT);
	CLEAR_BIT(TIMSK,OCIE0_BIT);
	SET_BIT(TIFR,TOV0_BIT);	
	
	
	switch(en_mode)
	{
		case T0_NORMAL_MODE:
			/*initialize values*/
			SET_MASK(TCCR0,(en_mode|en_OC0));
			SET_MASK(TIMSK,en_interruptMask);
			TCNT0		= u8_initialValue;
			OCR0        = u8_outputCompare;
		break;
		case T0_COMP_MODE:
		/*compare mode*/
			SET_MASK(TCCR0,(en_mode|en_OC0));
			SET_MASK(TIMSK,en_interruptMask);
			TCNT0		= u8_initialValue;
			OCR0        = u8_outputCompare;
		break;
	}	
	
}

/**
 * Description:
 * @param value
 */
void timer0Set(uint8_t u8_value)
{
		TCNT0 = u8_value;
}

/**
 * Description:
 * @return
 */
uint8_t timer0Read(void)
{
	return TCNT0;
}

/**
 * Description:
 */
void timer0Start(void)
{
		CLEAR_MASK(TCCR0,TIMER0_PRESCALER_CLEAR_MASK);
		SET_MASK(TCCR0,genu_timer0prescaler);
}
/**
 * Description:
 */
void timer0Stop(void)
{
	/*clear prescaler*/
	CLEAR_MASK(TCCR0,TIMER0_PRESCALER_CLEAR_MASK);
	/*zero all bits & registers
	TCCR0   =  ZERO;
	TCNT0   =  ZERO;
	TIMSK  &= ~(TOV0|OCF0);
	TIFR   |= TOV0;
	*/
}
/**
 * Description:
 * @param delay
 */
void timer0DelayMs(uint16_t u16_delay_in_ms)
{

	/************************************************************************/
	/*					NOTE: Normal Mode In use.							*/
	/************************************************************************/

	/*initialize timer
	*loop on microsec
	*load 250 to tcnt
	*pool for tov
	*--microsec
	*load tcnt with 250
	*clear tov flag
	*/
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64,
	TCNT_DELAY_INIT_VALUE,ZERO,T0_POLLING);
	timer0Start();

	while (u16_delay_in_ms)
	{
		/*pool for tov*/
		while(BIT_IS_CLEARD(TIFR,TOV0_BIT));
		TCNT0 = TCNT_DELAY_INIT_VALUE;
		SET_BIT(TIFR,TOV0_BIT);
		--u16_delay_in_ms;
	}
	timer0Stop();
}

/*
 * user defined
 */
void timer0DelayUs(uint32_t u32_delay_in_us)
{
/*no operation takes 0.1 micro sec*/

	while (u32_delay_in_us--);
	
}



/**
 * Description:
 * @param dutyCycle
 */
void timer0SwPWM(uint8_t u8_dutyCycle,uint8_t u8_frequency)
{
	
}




/*===========================Timer1 Control===============================*/

/**Registers needed
*			[TCCR2] Timer2 control register [FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20]
*			[TCNT2]	Timer2/conutner register [keep value of ticks]
*			[TIMR]	Timer Interrupt Mask register [TIE2 OCIE2]
*			[TIFR]	Timer Interrupt Flag Register [TOV2 OCRF2]
*			[ASSR]	Asynchronous Status Register [ AS2 TCN2UB OCR2UB TCR2UB] _just set the AS2 bit to 0 to work on internal clk_
*			[SFIOR] Special Function IO Register [PSR2]
*/

/**
 * Description:
 * @param controlA
 * @param controlB
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer1Init(En_timer1Mode_t en_mode,En_timer1OC_t en_OC,En_timer1perscaler_t en_prescal, uint16_t u16_initialValue, uint16_t u16_outputCompareA, uint16_t u16_outputCompareB,uint16_t u16_inputCapture, En_timer1Interrupt_t en_interruptMask);

/**
 * Description:
 * @param value
 */
void timer1Set(uint16_t u16_value);

/**
 * Description:
 * @return
 */
uint16_t timer1Read(void);

/**
 * Description:
 */
void timer1Start(void);

/**
 * Description:
 */
void timer1Stop(void);

/**
 * Description:
 * @param delay
 */
void timer1DelayMs(uint16_t u16_delay_in_ms);

/*
 * user defined
 */
void timer1DelayUs(uint32_t u32_delay_in_us);

/**
 * Description:
 * @param dutyCycle
 */
void timer1SwPWM(uint8_t u8_dutyCycle,uint8_t u8_frequency);







/*===========================Timer2 Control===============================*/

/**Registers needed
*			[TCCR2] Timer2 control register [FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20]
*			[TCNT2]	Timer2/conutner register [keep value of ticks]
*			[TIMR]	Timer Interrupt Mask register [TIE2 OCIE2]
*			[TIFR]	Timer Interrupt Flag Register [TOV2 OCRF2]
*			[ASSR]	Asynchronous Status Register [ AS2 TCN2UB OCR2UB TCR2UB] _just set the AS2 bit to 0 to work on internal clk_
*			[SFIOR] Special Function IO Register [PSR2]
*/

/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer2Init(En_timer2Mode_t en_mode,
 En_timer2OC_t en_OC2,En_timer2perscaler_t en_prescal,
 uint8_t u8_initialValue, uint8_t u8_outputCompare,
 En_timer2Asynchronous_t u8_assynchronous, 
 En_timer2Interrupt_t en_interruptMask)
 {
	/*saving prescaler value*/
	genu_timer2prescaler	= en_prescal;
	//genu_timer0Mode			= en_mode;
		
	/*zero all bits & registers*/
	TCCR2   =  ZERO;
	TCNT2   =  ZERO;
	CLEAR_BIT(TIMSK,TOIE2_BIT);
	CLEAR_BIT(TIMSK,OCIE2_BIT);
	SET_BIT(TIFR,TOV2_BIT);
	CLEAR_BIT(ASSR,AS2_BIT);

		
		
	switch(en_mode)
	{
		case T2_NORMAL_MODE:
		/*initialize values*/
		SET_MASK(TCCR2,(en_mode|en_OC2));
		SET_MASK(TIMSK,en_interruptMask);
		CLEAR_BIT(ASSR,u8_assynchronous);
		TCNT2		= u8_initialValue;
		OCR2        = u8_outputCompare;
		
		break;
		case T2_COMP_MODE:
		/*compare mode*/
		SET_MASK(TCCR2,(en_mode|en_OC2));
		SET_MASK(TIMSK,en_interruptMask);
		CLEAR_BIT(ASSR,u8_assynchronous);
		TCNT2		= u8_initialValue;
		OCR2        = u8_outputCompare;
		break;
	}
 }
/**
 * Description:
 * @param value
 */
void timer2Set(uint8_t u8_a_value)
{
	TCNT2 = u8_a_value;
}

/**
 * Description:
 * @return
 */
uint8_t timer2Read(void)
{
	return TCNT2;
}

/**
 * Description:
 */
void timer2Start(void)
{
	CLEAR_MASK(TCCR2,TIMER2_PRESCALER_CLEAR_MASK);
	SET_MASK(TCCR2,genu_timer2prescaler);
}

/**
 * Description:
 */
void timer2Stop(void)
{
	CLEAR_MASK(TCCR0,TIMER2_PRESCALER_CLEAR_MASK);
}


/**
 * Description:
 * @param delay
 */
void timer2DelayMs(uint16_t u16_delay_in_ms)
{
	/************************************************************************/
	/*					NOTE: Normal Mode In use.							*/
	/************************************************************************/

	/*initialize timer
	*loop on microsec
	*load 250 to tcnt
	*pool for tov
	*--microsec
	*load tcnt with 250
	*clear tov flag
	*/

	timer2Init(T2_NORMAL_MODE,T2_OC2_DIS,T2_PRESCALER_64,
	TCNT_DELAY_INIT_VALUE,ZERO,T2_INERNAL_CLK,T2_POLLING);
	
	timer2Start();
	while (u16_delay_in_ms)
	{
		/*pool for tov2*/
		while(BIT_IS_CLEARD(TIFR,TOV2_BIT));

		TCNT2 = TCNT_DELAY_INIT_VALUE;
		SET_BIT(TIFR,TOV2_BIT);
		--u16_delay_in_ms;
	}
	timer2Stop();
}
/*
 * user defined
 */
void timer2DelayUs(uint32_t u16_delay_in_us);

/**
 * Description:
 * @param dutyCycle
 */
void timer2SwPWM(uint8_t u8_dutyCycle,uint8_t u8_frequency);