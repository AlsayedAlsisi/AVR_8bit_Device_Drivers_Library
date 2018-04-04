/*
 * sw.c
 *
 * Created: 05-Mar-18 10:44:11 PM
 *  Author: Alsayed
 */ 
/*----------------------------------------------------------------
--------------------- File Inclusions ----------------------------
----------------------------------------------------------------*/
#include "gpio.h"
#include "sw.h"

/*----------------------------------------------------------------
--------------------- Private Constants ---------------------------
----------------------------------------------------------------*/
#define xx_SW_PORT 	  PORT_xx
#define xx_SW_PIN         PIN_xx


/*----------------------------------------------------------------
--------------------- Private Data Types -------------------------
----------------------------------------------------------------*/
typedef enum 
{
	LEVEL_LOW, 
	LEVEL_HIGH,
	LEVEL_BOUNCING
	}level_t;
/*----------------------------------------------------------------
--------------------- Private Variable Definitions ---------------
----------------------------------------------------------------*/
static sw_state_t 	xx_sw_state = RELEASED;




/*----------------------------------------------------------------
--------------------- Private Functions Prototypes ---------------
----------------------------------------------------------------*/
static void xx_sw_update(void);


/*----------------------------------------------------------------
--------------------- Public Function Definitions ----------------
----------------------------------------------------------------*/
void sw_init(void)
{
gpio_init_port_pin(xx_SW_PORT,   xx_SW_PIN,   INPUT, 0xff); //initializing and enabling the pull-up resistor of the sw pin
	
}

void sw_update(void) //This function should be called each 5ms, for example in a timer interrupt.
{
	xx_sw_update();
	
}


sw_state_t get_xx_sw_state(void) //This function is called each 5ms to check the 
{
	return xx_sw_state;
}






/*----------------------------------------------------------------
--------------------- Private Functions Definitions ---------------
----------------------------------------------------------------*/



void _sw_update(void)
{
	static uint8_t sw_samples[5] = {1,1,1,1,1};
	static level_t previous_level = LEVEL_HIGH;
	static level_t current_level  = LEVEL_HIGH;
	
	sw_samples[0] = sw_samples[1];
	sw_samples[1] = sw_samples[2];
	sw_samples[2] = sw_samples[3];
	sw_samples[3] = sw_samples[4];
	sw_samples[4] = gpio_read_port_pin(xx_sw_PORT, xx_sw_PIN);
	
	//Checking what current level of the sw it is:
	if (sw_samples[0] == 1 && sw_samples[1] == 1 && sw_samples[2] == 1 && sw_samples[3] == 1 && sw_samples[4] == 1)
	{current_level = LEVEL_HIGH;}
	
	else if (sw_samples[0] == 0 && sw_samples[1] == 0 && sw_samples[2] == 0 && sw_samples[3] == 0 && sw_samples[4] == 0)
	{current_level = LEVEL_LOW;}
	
	
	//Checking what current state of the sw it is:
	if (previous_level == LEVEL_HIGH && current_level == LEVEL_HIGH)
	{
		xx_sw_state = RELEASED;
	}
	else if (previous_level == LEVEL_LOW && current_level == LEVEL_LOW)
	{
		xx_sw_state = PRESSED;
	}
	else if (previous_level == LEVEL_HIGH && current_level == LEVEL_LOW)
	{
		xx_sw_state = FALLING_EDGE;
	}
	else if (previous_level == LEVEL_LOW && current_level == LEVEL_HIGH)
	{
		xx_sw_state = RISING_EDGE;
	}

	
	
	previous_level = current_level;
}



/*----------------------------------------------------------------
--------------------- End of File --------------------------------
----------------------------------------------------------------*/