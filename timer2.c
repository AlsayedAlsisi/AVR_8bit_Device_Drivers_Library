/*
 * timer2.c
 *
 * Created: 04-Mar-2018 
 * Author: Alsayed Alsisi
 * Contact Details:
  - (+2)01066769510
  - alsayed.alsisi@gmail.com
 */ 

/*----------------------------------------------------------------
--------------------- File Inclusions ----------------------------
----------------------------------------------------------------*/
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "timer2.h"




/*----------------------------------------------------------------
--------------------- Public Function Definitions ----------------
----------------------------------------------------------------*/

void timer2_mode_set(timer_mode_t timer_mode)
{
	switch (timer_mode)
	{
		case TIMER2_NORMAL_MODE:
		TCCR2 &= ~((1<<WGM21)|(1<<WGM20));
		break;
		
		case TIMER2_CTC_MODE:
		TCCR2 |= (1<<WGM21);
		TCCR2 &= ~(1<<WGM20);
		break;
		
		case TIMER2_FAST_PWM_MODE:
		TCCR2 |= ((1<<WGM21)|(1<<WGM20));
		TCCR2 |= (1<<COM21);				//Non-inverted fast PWM mode.
		TCCR2 &= ~(1<<COM20);
		DDRD  |= (1<<7);					//setting OC2 as output to output the PWM signal
		default:
		break;
	}
	
}



void timer2_prescalar_set(timer_prescalar_t timer_prescalar)
{
	
	switch (timer_prescalar)
	{
		case TIMER2_NO_CLK:
		TCCR2 &= ~((1<<CS22)|(1<<CS21)|(1<<CS20));
		break;
		
		case TIMER2_NO_PRESCALAR:
		TCCR2 |= (1<<CS20);
		TCCR2 &= ~((1<<CS22)|(1<<CS21));
		break;
		
		case TIMER2_PRESCALAR8:
		TCCR2 |= (1<<CS21);
		TCCR2 &= ~((1<<CS22)|(1<<CS20));
		break;
		
		case TIMER2_PRESCALAR32:
		TCCR2 |=  ((1<<CS21)|(1<<CS20));
		TCCR2 &= ~(1<<CS22);
		break;
		
		case TIMER2_PRESCALAR64:
		TCCR2 |= (1<<CS22);
		TCCR2 &=  ~((1<<CS21)|(1<<CS20));
		break;
		
		case TIMER2_PRESCALAR128:
		TCCR2 |=  ((1<<CS22)|(1<<CS20));
		TCCR2 &= ~(1<<CS21);
		break;
		
		case TIMER2_PRESCALAR256:
		TCCR2 |= ((1<<CS22)|(1<<CS21));
		TCCR2 = ~(1<<CS20);
		break;
		
		case TIMER2_PRESCALAR1024:
		TCCR2 |=  ((1<<CS22)|(1<<CS21)|(1<<CS20));
		break;
		
		default:
		break;
	}
	
}



void timer2_interrupt_enable(timer_mode_t timer_mode)
{
	switch(timer_mode)
	{
		case TIMER2_NORMAL_MODE:
		TIMSK |= (1<<TOIE2);
		sei();
		break;
		
		case TIMER2_CTC_MODE:
		TIMSK |= (1<<OCIE2);
		sei();
		break;
		
		default:
		break;
	}
}

void timer2_interrupt_disable(void)
{
	TIMSK &= ~((1<<TOIE2)|(1<<OCIE2));
}


void timer2_TCNT2_load(uint8_t value_to_load)
{
	TCNT2  = value_to_load;
}

void timer2_OCR2_load( uint8_t value_to_load)
{
	OCR2 = value_to_load;
}

void timer2_create_delay_us(uint8_t value_to_load_ocr2, timer_prescalar_t timer_prescalar)
{
	
	TIMSK &= ~((1<<TOIE2)|(1<<OCIE2));		//disable the timer interrupts.
	TCNT2 = 0;								//to make sure you are starting clean.
	OCR2 = value_to_load_ocr2;				//loading ocr0 register with the desired value.
	switch (timer_prescalar)				//setting the timer clock prescalar.
	{
		case TIMER2_NO_CLK:
		TCCR2 &= ~((1<<CS22)|(1<<CS21)|(1<<CS20));
		break;
		
		case TIMER2_NO_PRESCALAR:
		TCCR2 |= (1<<CS20);
		TCCR2 &= ~((1<<CS22)|(1<<CS21));
		break;
		
		case TIMER2_PRESCALAR8:
		TCCR2 |= (1<<CS21);
		TCCR2 &= ~((1<<CS22)|(1<<CS20));
		break;
		
		case TIMER2_PRESCALAR32:
		TCCR2 |=  ((1<<CS21)|(1<<CS20));
		TCCR2 &= ~(1<<CS22);
		break;
		
		case TIMER2_PRESCALAR64:
		TCCR2 |= (1<<CS22);
		TCCR2 &=  ~((1<<CS21)|(1<<CS20));
		break;
		
		case TIMER2_PRESCALAR128:
		TCCR2 |=  ((1<<CS22)|(1<<CS20));
		TCCR2 &= ~(1<<CS21);
		break;
		
		case TIMER2_PRESCALAR256:
		TCCR2 |= ((1<<CS22)|(1<<CS21));
		TCCR2 = ~(1<<CS20);
		break;
		
		case TIMER2_PRESCALAR1024:
		TCCR2 |=  ((1<<CS22)|(1<<CS21)|(1<<CS20));
		break;
		
		default:
		break;
	}
	while((TIFR & (1<<OCF2)) == 0);										 //check compare match flag.
	TCCR2 = 0x00;														 //stop timer.
	TIFR |= (1<<OCF2);													 //clear flag.
	
}

void timer2_pwm_duty_cycle_set(uint8_t duty_cycle_percentage)
{
	OCR2 =( ( (duty_cycle_percentage * 256) / 100) - 1 );
}


/*----------------------------------------------------------------
--------------------- End of File --------------------------------
----------------------------------------------------------------*/

