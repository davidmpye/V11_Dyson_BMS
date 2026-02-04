/*
 * serial_debug.c
 *
 *  Author:  David Pye
 *  Contact: davidmpye@gmail.com
 *  Licence: GNU GPL v3 or later
 */ 

/*-----------------------------------------------------------------------------
    INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "serial_debug.h"
#include "sw_timer.h"
#ifdef SERIAL_DEBUG
#include <string.h>
#endif
#include "eeprom_handler.h"

/*-----------------------------------------------------------------------------
    DECLARATION OF LOCAL FUNCTIONS
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DECLARATION OF LOCAL MACROS/#DEFINES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL TYPES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL VARIABLES
-----------------------------------------------------------------------------*/
#ifdef SERIAL_DEBUG
static struct usart_module debug_usart;
static char debug_buffer[80];
#endif

/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL CONSTANTS
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL VARIABLES
-----------------------------------------------------------------------------*/
#ifdef SERIAL_DEBUG
char *debug_msg_buffer = debug_buffer;
#endif

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL CONSTANTS
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL FUNCTIONS PROTOTYPES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/
//- **************************************************************************
//! \brief 
//- **************************************************************************
void serial_debug_init()
{
#ifdef SERIAL_DEBUG
  //Set up the pinmux settings for SERCOM0
  //pin_set_peripheral_function(PINMUX_PA11C_SERCOM0_PAD3);
  //pin_set_peripheral_function(PINMUX_PA10C_SERCOM0_PAD2);
  
  struct usart_config config_usart;
  usart_get_config_defaults(&config_usart);
  
  //Load the necessary settings into the config struct.
  config_usart.baudrate    = 115200ul;
  config_usart.mux_setting = USART_RX_3_TX_2_XCK_3 ;
  config_usart.parity      = USART_PARITY_NONE;
  config_usart.pinmux_pad0 = PINMUX_UNUSED;
  config_usart.pinmux_pad1 = PINMUX_UNUSED;
  config_usart.pinmux_pad2 = PINMUX_PA10C_SERCOM0_PAD2;
  config_usart.pinmux_pad3 = PINMUX_PA11C_SERCOM0_PAD3;
  
  //Init the UART
  while (usart_init(&debug_usart,SERCOM0, &config_usart) != STATUS_OK) { }
  //Enable
  usart_enable(&debug_usart);
  
  //Initial debug blurb
  serial_debug_send_message("Dyson V11/V15 BMS After market firmware\r\n");
  serial_debug_send_cell_voltages();
  serial_debug_send_pack_capacity();
#endif
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void serial_debug_send_message(const char *msg)
{
#ifdef SERIAL_DEBUG
  size_t msg_len = strlen(msg);

  if(msg_len > 0)
  {
    usart_write_buffer_wait(&debug_usart, (const uint8_t*)msg, (uint16_t)msg_len);
  }
#endif
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void serial_debug_send_cell_voltages(void)
{
#ifdef SERIAL_DEBUG
  uint16_t *cell_voltages = bq7693_get_cell_voltages();
  
  serial_debug_send_message("Pack cell voltages:\r\n");
  
  for (int i=0; i<7; ++i) 
  {
    sprintf(debug_msg_buffer, "Cell %d: %d mV\r\n", i, cell_voltages[i]);
    serial_debug_send_message(debug_msg_buffer);
  }
#endif
}
//- **************************************************************************
//! \brief
//- **************************************************************************
void serial_debug_send_pack_capacity(void)
{
#ifdef SERIAL_DEBUG
  sprintf(debug_msg_buffer, "Pack capacity %ldmAh\r\n", eeprom_data.current_charge_level/1000);
  serial_debug_send_message(debug_msg_buffer);
#endif
}


/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL FUNCTIONS
-----------------------------------------------------------------------------*/
//- **************************************************************************
//! \brief 
//- **************************************************************************

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/






