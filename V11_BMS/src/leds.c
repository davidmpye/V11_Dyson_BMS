/*
 * leds.c
 *
 *  Author:  David Pye
 *  Contact: davidmpye@gmail.com
 *  Licence: GNU GPL v3 or later
 */ 
#include "leds.h"

//speed of LED sequence
#define LED_SEQ_TIME 50

#define NUM_LEDS 2
uint16_t leds[] = { LED_BLOCKED, LED_ERR, };
	
void leds_init() 
{
	//Set up the LED pins as IO
	struct port_config led_port_config;
	port_get_config_defaults(&led_port_config);
	led_port_config.direction = PORT_PIN_DIR_OUTPUT;

	for (int i=0; i<NUM_LEDS; ++i) 
  {
		port_pin_set_config(leds[i], &led_port_config);
	}
}

void leds_off() 
{
	for (int i=0; i<NUM_LEDS; ++i) 
  {
		port_pin_set_output_level(leds[i], false);
	}
}

void leds_on() 
{
	for (int i=0; i<NUM_LEDS; ++i) 
  {
		port_pin_set_output_level(leds[i], true);
	}
}

void leds_blink_error_led(int ms) 
{
		port_pin_set_output_level(LED_ERR, true );
		delay_ms(ms/2);
		port_pin_set_output_level(LED_ERR, false );
		delay_ms(ms/2);
}

void leds_show_pack_flat() 
{

}