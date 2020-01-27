/*
 * DIO.c
 *
 * Created: 1/17/2020 4:09:01 AM
 * Author : mahmo
 */ 

#include "gpio.h"
#include "softwareDelay.h"
#include "pushButton.h"
#include "sevenSeg.h"
#include "led.h"
#include "timers.h"
#include "interrupt.h"
#include "dcMotor.h"

#define  MAXCOUNT 100
#define  TEN	  10
#define  TUNEDTIMEDECRMENTER    60
#define  FIFTY    50

ISR(TIMER1_COMPA_vect)
{
	gpioPinWrite(MOTOR_EN_1_GPIO,MOTOR_EN_1_BIT,HIGH);
}
ISR(TIMER1_COMPB_vect)
{
	gpioPinWrite(MOTOR_EN_2_GPIO,MOTOR_EN_2_BIT,HIGH);
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 63036;
	gpioPinWrite(MOTOR_EN_1_GPIO,MOTOR_EN_1_BIT,LOW);
	gpioPinWrite(MOTOR_EN_2_GPIO,MOTOR_EN_2_BIT,LOW);

}
/**
 * Description: function to count from 0 to 99 on two
 * seven seg using software delay.
 * @param  void
 * return  void
 */

/*
*Description : Enum used for 9th requirment as traffic  
*light states
*/
enum EN_trafficLightState{
	go,
	stop,
	ready
}trafficLightState = stop;


/* an Function that makes a 00-99 count up 
* counter using the two seven-segment 
* displays with a 1-second delay from count to count.
* @param : void
* return : void
*/
void counter_up(void){
	uint8_t counter = 0;
	uint8_t MeliSec = 0;
	
	while(counter < MAXCOUNT)
	{
		/*
	     *disable SEG1 enable SEG0
		 *send first bit
		 */
		sevenSegInit(SEG_0);
		sevenSegEnable(SEG_0);
		sevenSegDisable(SEG_1);
		sevenSegWrite(SEG_0,counter/TEN);
		/*
	     *disable SEG0 enable SEG1
		 *send second bit
		 */
		sevenSegInit(SEG_1);
		sevenSegEnable(SEG_1);
		sevenSegDisable(SEG_0);
		sevenSegWrite(SEG_1,counter%TEN);
		/*software delay 1ms */
		softwareDelayMs(MSEC);
		MeliSec++;
		if(MeliSec == SEC){
		counter++;
		MeliSec = 0;
		}
	}
}

/*Description: a Function that implements a state machine
*of a traffic light with states: -stop
*								 -ready
*								 -go
*@param : void
*return : void
*/
void trafficLight()
{
	/*initialize led pins
	*turnoff all lights
	*get next state
	*turn on state led
	*delay
	*repeat
	*/	
	Led_Init(LED_1);
	Led_Init(LED_2);
	Led_Init(LED_3);
	Led_Off(LED_1);
	Led_Off(LED_2);
	Led_Off(LED_3);
	
	switch(trafficLightState)
	{
		case go:
		trafficLightState = stop;
		Led_On(LED_1);
		break;
		case stop:
		trafficLightState = ready;
		Led_On(LED_2);
		break;
		case ready:
		trafficLightState = go;
		Led_On(LED_3);
		break;
	}
	softwareDelayMs(SEC);
}

/*
* Description: a Function that turn on a led 1 sec 
* for every button push on the condition that the time
* between two pushes is less than 1 sec
* after the time elapse the led turns off
* @param : void
* return : void
*/
void led_button(void){
	En_buttonStatus_t buttonStatus = Released;
	uint8_t ledOnTime = 0 ;
	/* initialize and pull up on button
	*  initialize led 
	*/
	pushButtonInit(BTN_0);
	gpioPinWrite(BTN_0_GPIO,BTN_0_BIT,HIGH);
	Led_Init(LED_0);
	
	/*get button state*/
	buttonStatus = pushButtonGetStatus(BTN_0);
	
	/*for each button press increase led_on time by 1 sec*/
	if(buttonStatus == Pressed)
	{
		ledOnTime+=SEC;
		Led_On(LED_0);
		
		while(ledOnTime > 0){
			softwareDelayMs(FIFTY);
			buttonStatus = pushButtonGetStatus(BTN_0);
			ledOnTime-=TUNEDTIMEDECRMENTER;
			if(buttonStatus == Pressed)
			ledOnTime+= SEC;
		}	
		Led_Off(LED_0);
	}
}


int main(void)
{
	
	sei();
	//Led_Init(LED_0);
	//Led_Init(LED_1);
	//Led_On(LED_0);
	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	

    
		sint8_t i ;
		MotorDC_Dir(MOT_1,FORWARD);
		MotorDC_Dir(MOT_2,FORWARD);
		for( i = 20 ; i <= 100 ; i+=20)	
		{
			MotorDC_Speed_PollingWithT0(i);
			timer2DelayMs(1000);
		}
		for( i = 80 ; i >= 0  ; i-=20)
		{
			MotorDC_Speed_PollingWithT0(i);
			timer2DelayMs(1000);
		}
		MotorDC_Dir(MOT_1,FORWARD);
		MotorDC_Dir(MOT_2,BACKWARD);
		MotorDC_Speed_PollingWithT0(20);
		timer2DelayMs(2000);
		MotorDC_Speed_PollingWithT0(1);
		timer1Stop();
	
		/*uncomment a function to choose application 
		*/
		//trafficLight();
		//led_button();
		//counter_up();
		/*
		Led_Off(LED_0);
		timer1DelayMs(10);
		Led_On(LED_0);
		timer1DelayMs(10);
*/
		
    	
}

