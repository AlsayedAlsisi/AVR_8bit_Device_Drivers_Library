/*
 * timer.h
 *
 *	Created: 02-Mar-18 11:15:34 PM
 *  Author: Alsayed Alsisi
 *	Contact Details:
  - (+2)01066769510
  - alsayed.alsisi@gmail.com
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

/*----------------------------------------------------------------
--------------------- File Inclusions ----------------------------
----------------------------------------------------------------*/

#include <stdint.h>

/*----------------------------------------------------------------
--------------------- Public Data Types --------------------------
----------------------------------------------------------------*/

typedef enum{
	TIMER0_NORMAL_MODE=0,
	TIMER0_PHASE_CORRECT_PWM_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FAST_PWM_MODE
	}timer_mode_t;
	
	
typedef enum{
	TIMER0_NO_CLK=0,						//No clock source (Timer/Counter stopped).
	TIMER0_NO_PRESCALAR,					//clk / (No prescaling)
	TIMER0_PRESCALAR8,						//clk / 8
	TIMER0_PRESCALAR64,						//clk / 64
	TIMER0_PRESCALAR256,					//clk / 256
	TIMER0_PRESCALAR1024,					//clk / 1024
	TIMER0_EXTERNAL_CLOCK_FALLING_EDGE,		//
	TIMER0_EXTERNAL_CLOCK_RISING_EDGE		//
	}timer_prescalar_t;



/*----------------------------------------------------------------
--------------------- Public Function Prototypes ----------------
----------------------------------------------------------------*/

extern void timer0_mode_set(timer_mode_t timer_mode);

extern void timer0_prescalar_set(timer_prescalar_t timer_prescalar);

extern void timer0_interrupt_enable(timer_mode_t timer_mode);

extern void timer0_interrupt_disable(void);

extern void timer0_TCNT0_load(uint8_t value_to_load);

extern void timer0_OCR0_load( uint8_t value_to_load);

extern void timer0_create_delay_us(uint8_t value_to_load_ocr0, timer_prescalar_t timer_prescalar);

extern void timer0_pwm_duty_cycle_set(uint8_t duty_cycle_percentage);


/*----------------------------------------------------------------
--------------------- Information on How to Use this Driver -------
----------------------------------------------------------------*/
/*
The three most common uses of timers are:
___________________________________________________________________________
1. Generating timer interrupts:

This example is for creating a 5ms timer interrupt using CTC mode, 8MHZ system clock frequency, and 1024 clock prescalar.
- First  step is 'initialization'. That would be as follows:
void timer0_init(void)
{
	//Setting the timer to work in Clear Timer on Compare Match (CTC) Mode:
	timer0_mode_set(CTC_MODE);
	
	//To make sure you are starting clean:
	timer0_TCNT0_load(0); 
	
	//Loading OCR0 register with a value that achieves the desired delay from the equation: 
	//OCR0 = ((delay_microsec * cpu_frequency_mhz) / (prescalar))
	//Note that you should try different prescalars and choose the one that achieves the most.. 
	//precise delay:
	timer0_OCR0_load(39); 
	
	
	//Enabling the timer interrupts:
	timer0_interrupt_enable(CTC_MODE); 
}

- Second step is to start the timer by connecting the clock:
void timer0_start(void)
{
	timer0_prescalar_set(TIMER0_PRESCALAR1024);
}

- Third step is to write the ISR: 
ISR(TIMER0_COMP_vect) //Note that the interrupt vector is the one for the CTC mode.
{
	PORTC ^= 0xff; //Toggle port c pins for example.
}
________________________________________________________________________
2. Creating delays using polling technique (where the CPU stops doing anything else):

All you you have to do is to find the value to be loaded to the OCR0 register from the following equation:
OCR0 = ((delay_microsec * cpu_frequency_mhz) / timer_prescalar)
Note that you should try different prescalars and choose the one that achieves the most precise delay. 
The following is an example of creating a 5ms delay:
	- TIMER0_NO_PRESCALAR:
		OCR0 = ((5000 microsec. * 8MHZ) / 1) = 40000 (non-applicable using 8-bit timer)
	- TIMER0_PRESCALAR8:
		OCR0 = ((5000 microsec. * 8MHZ) / 8) = 5000 (non-applicable using 8-bit timer)
	- TIMER0_PRESCALAR64:
		OCR0 = ((5000 microsec. * 8MHZ) / 64) = 625 (non-applicable using 8-bit timer)
	- TIMER0_PRESCALAR256:		
		OCR0 = ((5000 microsec. * 8MHZ) / 256) = 156.25 (applicable)
	- TIMER0_PRESCALAR1024:
		OCR0 = ((5000 microsec. * 8MHZ) / 1024) = 39.0625 (applicable and best choice)
		
timer0_create_delay_us(39, TIMER0_PRESCALAR1024);

__________________________________________________________________________
3. Creating PWM signals:
- First  step is 'initialization'. It would be implemented as follows:
void timer0_pwm_init(void)
{
	//Setting the timer to work in Fast PWM Mode:
	timer0_mode_set(TIMER0_FAST_PWM_MODE);
	
	//set the desired initial duty cycle:
	timer0_pwm_duty_cycle_set(75); // 75%
	
}
- Second step is to start the PWM signal:
void timer0_pwm_start(void)
{
	//Note that the pwm frequency is determined from the Equ.: freq.(pwm) = freq.(Sysclock)/(N*256). N is the timer prescalar.
	timer0_prescalar_set(TIMER0_NO_PRESCALAR);
}

- Third step is to change the duty cycle as required. And note that 0% duty cycle means no output on the pwm OC0 pin.
timer0_pwm_duty_cycle_set(duty_cycle_percentage);

*/
#endif /* TIMER0_H_ */