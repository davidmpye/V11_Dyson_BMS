/*
 * crc.h
 *
 * Created: 21-Jan-26 12:27:30
 * Author : Vladislav Gyurov
 * License: GNU GPL v3 or later
 */ 


#ifndef CRC_H_
#define CRC_H_
/*-----------------------------------------------------------------------------
  INCLUDE FILES
---------------------------------------------------------------------------- */
#include "asf.h"

/*-----------------------------------------------------------------------------
  DEFINITION OF GLOBAL TYPES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  DEFINITION OF GLOBAL MACROS/#DEFINES
-----------------------------------------------------------------------------*/
// width=32  poly=0x04c11db7  init=0xbd92a495  refin=true  refout=true  xorout=0x00000000  check=0x01928599  residue=0x00000000  name=(none)
#define CRC32_INIT_MSG_C1       (uint32_t)(0xA92549BD) // reflected init 0xbd92a495 of msg 12 38 00 C1 ... 
#define CRC16_INIT_MSG_53       (uint16_t)(0xF69C)     // reflected init 0x396F of msg 12 21 00 53 ...


/*-----------------------------------------------------------------------------
  DECLARATION OF GLOBAL VARIABLES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  DECLARATION OF GLOBAL CONSTANTS
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  DECLARATION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/
extern uint32_t calc_crc32(uint32_t crc_init,  uint8_t * data_ptr, uint16_t len);
extern uint16_t calc_crc16_C9A7(uint16_t crc_init, uint8_t * data_ptr, uint16_t len);
uint16_t crc16(uint8_t *data, size_t len, uint16_t init);
uint32_t calc_msg_crc(uint8_t* msg, size_t len);
uint32_t calc_crc_init(uint8_t seq, uint8_t* matrix_table, uint16_t constant);
/*-----------------------------------------------------------------------------
  END OF MODULE DEFINITION FOR MULTIPLE INCLUSION
-----------------------------------------------------------------------------*/
#endif /* CRC_H_ */