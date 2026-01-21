/*
 * serial.c
 *
 *  Author:  David Pye
 *  Contact: davidmpye@gmail.com
 *  Licence: GNU GPL v3 or later
 */ 

#include "asf.h"
#include "serial.h"

int serial_msgIndex = 0;
size_t serial_get_next_block(uint8_t **bytes) 
{
	return 0;
}

struct usart_module usart_instance;

static inline void pin_set_peripheral_function(uint32_t pinmux) 
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >>
	16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux &
	0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}

//Data is read into here via a callback triggered by usart_read_buffer_job
void usart_read_callback(struct usart_module *const usart_module) 
{
}

void serial_init() {	
	//Set up the pinmux settings for SERCOM2
	pin_set_peripheral_function(PINMUX_PA14C_SERCOM2_PAD2);
	pin_set_peripheral_function(PINMUX_PA15C_SERCOM2_PAD3);
	
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	
	//Load the necessary settings into the config struct.
	config_usart.baudrate    = 115200;
	config_usart.mux_setting =  USART_RX_3_TX_2_XCK_3 ;
	config_usart.parity = USART_PARITY_NONE;
	config_usart.pinmux_pad2 = PINMUX_PA14C_SERCOM2_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PA15C_SERCOM2_PAD3;
	
	//Init the UART
	while (usart_init(&usart_instance,SERCOM2, &config_usart) != STATUS_OK) {
	}
	
	//Enable a callback for bytes received.
	usart_register_callback(&usart_instance, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
	
	usart_enable(&usart_instance);
	//Start read job - the next one is kicked off by the above callback	//usart_read_buffer_job(&usart_instance, (uint8_t *)serial_read_buffer, 40);}

void serial_send_next_message()
{	

}
	
void serial_reset_message_counter()
{
}