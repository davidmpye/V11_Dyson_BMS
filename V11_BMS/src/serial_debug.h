/*
 * serial_debug.h
 *
 *  Author:  David Pye
 *  Contact: davidmpye@gmail.com
 *  Licence: GNU GPL v3 or later
 */ 


#ifndef SERIAL_DEBUG_H_
#define SERIAL_DEBUG_H_

#include "asf.h"
#include "config.h"

#include "bq7693.h"

extern void serial_debug_init(void);
extern void serial_debug_send_message(const char *msg);
extern void serial_debug_send_cell_voltages(void);

extern char *debug_msg_buffer;
#endif /* SERIAL_DEBUG_H_ */