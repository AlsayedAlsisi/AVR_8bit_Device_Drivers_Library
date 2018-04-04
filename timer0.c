/*
 * timer.c
 *
 *	Created: 02-Mar-2018 
 *  Author: Alsayed Alsisi
 *	Contact Details:
	- (+2)01066769510
	- alsayed.alsisi@gmail.com
 */ 
/*----------------------------------------------------------------
--------------------- File Inclusions ----------------------------
----------------------------------------------------------------*/
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "timer0.h"



	
/*----------------------------------------------------------------
--------------------- Public Function Definitions ----------------
----------------------------------------------------------------*/

	void timer0_mode_set(timer_mode_t timer_mode)
	{
		switch (timer_mode)
		{
			case TIMER0_NORMAL_MODE:
			TCCR0 &= ~((1<<WGM01)|(1<<WGM00));
			break;
		
			case TIMER0_CTC_MODE:
			TCCR0 |= (1<<WGM01);
			TCCR0 &= ~(1<<WGM00);
			break;
		
			case TIMER0_FAST_PWM_MODE:
			TCCR0 |= ((1<<WGM01)|(1<<WGM00));
			TCCR0 |= (1<<COM01);				//Non-inverted fast PWM mode.
			TCCR0 &= ~(1<<COM00);
			DDRB  |= (1<<3);					//setting OC0 as output to output the PWM signal
			break;

			default:
			break;
		}
	
	}



	void timer0_prescalar_set(timer_prescalar_t timer_prescalar)
	{
		
		switch (timer_prescalar)
		{
			case TIMER0_NO_CLK:
			TCCR0 &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
			break;
		
			case TIMER0_NO_PRESCALAR:
			TCCR0 &= ~((1<<CS02)|(1<<CS01));
			TCCR0 |= (1<<CS00);
			break;
		
			case TIMER0_PRESCALAR8:
			TCCR0 &= ~((1<<CS02)|(1<<CS00));
			TCCR0 |= (1<<CS01);
			break;
		
			case TIMER0_PRESCALAR64:
			TCCR0 |=  ((1<<CS01)|(1<<CS00));
			TCCR0 &= ~(1<<CS02);
			break;
		
			case TIMER0_PRESCALAR256:
			TCCR0 &= ~((1<<CS01)|(1<<CS00));
			TCCR0 |= (1<<CS02);
			break;
		
			case TIMER0_PRESCALAR1024:
			TCCR0 |=  ((1<<CS02)|(1<<CS00));
			TCCR0 &= ~(1<<CS01);
			break;
		
			case TIMER0_EXTERNAL_CLOCK_FALLING_EDGE:
			TCCR0 |=  ((1<<CS02)|(1<<CS01));
			TCCR0 &= ~(1<<CS00);
			break;
		
			case TIMER0_EXTERNAL_CLOCK_RISING_EDGE:
			TCCR0 |= ((1<<CS02)|(1<<CS01)|(1<<CS00));
			break;
		
			default:
			break;
		}
	
	}

	
	
	void timer0_interrupt_enable(timer_mode_t timer_mode)
	{
		switch(timer_mode)
		{
		case TIMER0_NORMAL_MODE:
		TIMSK |= (1<<TOIE0);
		sei();
		break;
	
		case TIMER0_CTC_MODE:
		TIMSK |= (1<<OCIE0);
		sei();
		break;
	
		default:
		break;
		}
	}

	void timer0_interrupt_disable(void)
	{
			TIMSK &= ~((1<<TOIE0)|(1<<OCIE0));
	}


	void timer0_TCNT0_load(uint8_t value_to_load)
	{
		TCNT0  = value_to_load;
	}

	void timer0_OCR0_load( uint8_t value_to_load)
	{
		OCR0 = value_to_load;
	}

	void timer0_create_delay_us(uint8_t value_to_load_ocr0, timer_prescalar_t timer_prescalar)
	{
			
			TIMSK &= ~((1<<TOIE0)|(1<<OCIE0));		//disable the timer interrupts.
			TCNT0 = 0;								//to make sure you are starting clean.
			OCR0 = value_to_load_ocr0;				//loading ocr0 register with the desired value.
			switch (timer_prescalar)				//setting the timer clock prescalar.
			{
				case TIMER0_NO_PRESCALAR:
				TCCR0 &= ~((1<<CS02)|(1<<CS01));
				TCCR0 |= (1<<CS00);
				break;
				
				case TIMER0_PRESCALAR8:
				TCCR0 &= ~((1<<CS02)|(1<<CS00));
				TCCR0 |= (1<<CS01);
				break;
				
				case TIMER0_PRESCALAR64:
				TCCR0 |=  ((1<<CS01)|(1<<CS00));
				TCCR0 &= ~(1<<CS02);
				break;
				
				case TIMER0_PRESCALAR256:
				TCCR0 &= ~((1<<CS01)|(1<<CS00));
				TCCR0 |= (1<<CS02);
				break;
				
				case TIMER0_PRESCALAR1024:
				TCCR0 |=  ((1<<CS02)|(1<<CS00));
				TCCR0 &= ~(1<<CS01);
				break;
				
				default:
				break;
			}
			while((TIFR & (1<<OCF0)) == 0);										 //check compare match flag. 
			TCCR0 = 0x00;														 //stop timer. 
			TIFR |= (1<<OCF0);													 //clear flag.
		
	}

 void timer0_pwm_duty_cycle_set(uint8_t duty_cycle_percentage)
 {
	 OCR0 =( ( (duty_cycle_percentage * 256) / 100) - 1 );
 }

/*----------------------------------------------------------------
--------------------- End of File --------------------------------
----------------------------------------------------------------*/


