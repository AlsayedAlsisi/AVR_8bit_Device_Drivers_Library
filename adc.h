/*
 * ADC.h
 *
 * Created: 28-Jan-18 9:51:22 PM
 *  Author: Alsayed
 */ 

/*----------------------------------------------------------------
--------------------- Header Guard -------------------------------
----------------------------------------------------------------*/
#ifndef ADC_H_
#define ADC_H_

/*----------------------------------------------------------------
--------------------- File Inclusions ----------------------------
----------------------------------------------------------------*/

#include <stdint.h>
/*----------------------------------------------------------------
--------------------- Public Data Types --------------------------
----------------------------------------------------------------*/
typedef enum
{
	ADC_EXTERNAL_AREF=0,		// The AREF pin should be connected to +5v.
	ADC_AVCC_AREF,				// AVCC with external capacitor at AREF pin.
	ADC_INTERNAL_2_56_AREF		// Internal 2.56V Voltage Reference with external capacitor at AREF pin
} reference_voltage_t; 

typedef enum{
	ADC_PRESCALAR_2=0,					// clk/2
	ADC_PRESCALAR_4,					// clk/4
	ADC_PRESCALAR_8,					// clk/8
	ADC_PRESCALAR_16,					// clk/16
	ADC_PRESCALAR_32,	                // clk/32
	ADC_PRESCALAR_64,		            // clk/64
	ADC_PRESCALAR_128                   // clk/128
}adc_prescalar_t;

typedef enum{
	ADC_CHANNEL_0=0,					
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7
}adc_channel_t;

typedef enum{
	ADC_FREE_RUNNING_MODE=0,
	ADC_ANALOG_COMPARATOR,
	ADC_EXTERNAL_INTERRUPT_REQUEST_0,
	ADC_TIMER_COUNTER_0_COMPARE_MATCH,
	ADC_TIMER_COUNTER_0_OVERFLOW,
	ADC_TIMER_COUNTER_1_COMPARE_MATCH_B,
	ADC_TIMER_COUNTER_1_OVERFLOW,
	ADC_TIMER_COUNTER_1_CAPTURE_EVENT
	}adc_auto_triggering_source_t;
/*----------------------------------------------------------------
--------------------- Public Function Prototypes -----------------
----------------------------------------------------------------*/
    void adc_enable(void);
    void adc_disable(void);
    void adc_reference_voltage(reference_voltage_t reference_voltage);
    void adc_set_clock_prescalar(adc_prescalar_t adc_prescalar);
    uint16_t adc_read_channel(adc_channel_t adc_channel);
    uint16_t adc_read_adc_register();
    void adc_wait_conversion_complete(void);
    void adc_select_channel(adc_channel_t adc_channel);
    void adc_enable_interrupts(void);
    void adc_disable_interrupts(void);
    void adc_enable_auto_triggerig(void);
    void adc_disable_auto_triggerig(void);
    void adc_select_auto_triggering_source(adc_auto_triggering_source_t adc_auto_triggering_source);

/*----------------------------------------------------------------
--------------------- Information on How to Use this Driver -------
----------------------------------------------------------------*/
/*
There are 2 ways to use this ADC driver to read analog values:

1. Polling 
This is the simplest of the two ways. You can do that as follows:
--> The first step is Initialization. It can be implemented as follows:
void adc_init(void)
{
	adc_enable();
	adc_reference_voltage(ADC_EXTERNAL_AREF);
	adc_set_clock_prescalar(ADC_PRESCALAR_2);
}
--> The second step is reading the desired channel:
uint16_t my_desired_channel_value;
my_desired_channel_value = adc_read_channel(ADC_CHANNEL_0);
___________________________________________________________________

2. Interrupts:
- FIRST EXAMPLE:
--> The first step is Initialization. It can be implemented as follows:
void adc_init(void)
{
	adc_enable();
	adc_reference_voltage(ADC_EXTERNAL_AREF);
	adc_set_clock_prescalar(ADC_PRESCALAR_2);
	adc_select_channel(CHANNEL_2); //if not selected CHANNEL_0 will be selected by default
	adc_enable_interrupts();
	adc_start_conversion();
}

--> The second step is reading the desired channel:
 uint16_t my_desired_channel_value;
 ISR(ADC_vect)
 {

	  my_desired_channel_value = ADC;
	  //Select another desired channel
	  adc_start_conversion();
 }
 
 - SECOND EXAMPLE:
 --> The first step is Initialization. It can be implemented as follows:
 void adc_init(void)
 {
	 adc_enable();
	 adc_reference_voltage(ADC_EXTERNAL_AREF);
	 adc_set_clock_prescalar(ADC_PRESCALAR_2);
	 adc_select_channel(CHANNEL_2); //if not selected CHANNEL_0 will be selected by default
	 adc_enable_interrupts();
	 adc_select_auto_triggering_source(ADC_EXTERNAL_INTERRUPT_REQUEST_0);
 }

 --> The second step is reading the desired channel:
 uint16_t my_desired_channel_value;
 ISR(ADC_vect)
 {
	 my_desired_channel_value = ADC;
	 //Select another desired channel
 }
 //Note that in this example where you use the feature of auto triggering of the adc conversion, 
 //you don't have to start the conversion manually.
*/


#endif /* ADC_H_ */
/*----------------------------------------------------------------
--------------------- End of File --------------------------------
----------------------------------------------------------------*/