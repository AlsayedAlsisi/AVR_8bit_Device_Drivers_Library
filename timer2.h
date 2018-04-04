/*
 * timer2.h
 *
 * Created: 04-Mar-2018
 * Author: Alsayed Alsisi
 * Contact Details:
 - (+2)01066769510
 - alsayed.alsisi@gmail.com
 */


#ifndef TIMER2_H_
#define TIMER2_H_

/*----------------------------------------------------------------
--------------------- File Inclusions ----------------------------
----------------------------------------------------------------*/

#include <stdint.h>

/*----------------------------------------------------------------
--------------------- Public Data Types --------------------------
----------------------------------------------------------------*/

typedef enum{
	TIMER2_NORMAL_MODE=0,
	TIMER2_PHASE_CORRECT_PWM_MODE,
	TIMER2_CTC_MODE,
	TIMER2_FAST_PWM_MODE
}timer_mode_t;


typedef enum{
	TIMER2_NO_CLK=0,						//No clock source (Timer/Counter stopped).
	TIMER2_NO_PRESCALAR,					//clk / (No prescaling)
	TIMER2_PRESCALAR8,						//clk / 8
	TIMER2_PRESCALAR32,						//clk / 32
	TIMER2_PRESCALAR64,						//clk / 64
	TIMER2_PRESCALAR128,					//clk / 128
	TIMER2_PRESCALAR256,					//clk / 256
	TIMER2_PRESCALAR1024,					//clk / 1024
	TIMER2_EXTERNAL_CLOCK_FALLING_EDGE,		//
	TIMER2_EXTERNAL_CLOCK_RISING_EDGE		//
}timer_prescalar_t;




/*----------------------------------------------------------------
--------------------- Public Function Prototypes ----------------
----------------------------------------------------------------*/

extern void timer2_mode_set(timer_mode_t timer_mode);

extern void timer2_prescalar_set(timer_prescalar_t timer_prescalar);

extern void timer2_interrupt_enable(timer_mode_t timer_mode);

extern void timer2_interrupt_disable(void);

extern void timer2_TCNT2_load(uint8_t value_to_load);

extern void timer2_OCR2_load( uint8_t value_to_load);

extern void timer2_create_delay_us(uint8_t value_to_load_ocr2, timer_prescalar_t timer_prescalar);

extern void timer2_pwm_duty_cycle_set(uint8_t duty_cycle_percentage);


#endif /* TIMER2_H_ */