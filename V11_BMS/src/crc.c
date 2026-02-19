/*
 * crc.c
 *
 * Created: 21-Jan-26 12:27:18
 * Author : Vladislav Gyurov
 * License: GNU GPL v3 or later
 */ 
 /*-----------------------------------------------------------------------------
    INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "crc.h"

/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL VARIABLES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL CONSTANTS
-----------------------------------------------------------------------------*/

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

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL CONSTANTS
-----------------------------------------------------------------------------*/
static const uint32_t c_wCRC32Table[16] = 
{
  0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC,
  0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
  0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
  0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C,
};

static const uint16_t crc16_C9A7_table_lo[] = { 0x0000,0x5B1B,0xB636,0xED2D,0xA74B,0xFC50,0x117D,0x4A66,0x85B1,0xDEAA,0x3387,0x689C,0x22FA,0x79E1,0x94CC,0xCFD7 };
static const uint16_t crc16_C9A7_table_hi[] = { 0x0000,0xC045,0x4BAD,0x8BE8,0x975A,0x571F,0xDCF7,0x1CB2,0xE593,0x25D6,0xAE3E,0x6E7B,0x72C9,0xB28C,0x3964,0xF921 };


//0x63 message crc table
const uint8_t matrix_0x63_hi[] = { 0x0e, 0x36, 0x12, 0x72, 0x30, 0x6c, 0x0c, 0x68, 0x5e, 0x40, 0x54, 0x24, 0x48, 0x62, 0x6e, 0x06 };
const uint16_t const_0x63_hi = 0xf993;

const uint8_t matrix_0x63_lo[] = { 0x5c, 0x40, 0x7a, 0x52, 0x24, 0x30, 0x62, 0x44, 0x54, 0x52, 0x24, 0x32, 0x42, 0x20, 0x38, 0x2e };
const uint16_t const_0x63_lo = 0x98ef;


/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL FUNCTIONS PROTOTYPES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/
//- **************************************************************************
//! \brief 
//!         CRC32
//*!        Poly: 0x04C11DB7
//*!        Init: crc_init
//*!        RefIn: True
//*!        RefOut: True
//*!        XorOut: 0x00000000
//- **************************************************************************
uint32_t calc_crc32(uint32_t crc_init,  uint8_t * data_ptr, uint16_t len)
{
  uint16_t i;
  uint32_t crc32 = crc_init;

  if(NULL == data_ptr)
  {
    return 0;
  }

  for (i = 0; i < len; i++)
  {
    crc32 = (crc32 >> 4) ^ c_wCRC32Table[(crc32 & 0x0000000F) ^ (*data_ptr & 0x0F)];
    crc32 = (crc32 >> 4) ^ c_wCRC32Table[(crc32 & 0x0000000F) ^ (*data_ptr >> 4)];
    data_ptr++;
  }

  return (crc32);
}

//- **************************************************************************
//! \brief 
//!         Message CRC calculation
//- **************************************************************************
uint32_t calc_msg_crc(uint8_t* msg, size_t len) {
    //Parse the message header
    uint16_t init_lo, init_hi;
    uint16_t msg_len = (msg[1] | msg[2]<<8) -1;
    uint8_t cmd = msg[3];
    uint8_t seq = msg[8];

    //Generate the correct CRC16 init values based on message type and sequence
    switch (cmd) {
        case 0x63:
            init_hi = calc_crc_init(seq, matrix_0x63_hi, const_0x63_hi);
            init_lo = calc_crc_init(seq, matrix_0x63_lo, const_0x63_lo);
            break;
        default:
            //Need to flag this as an error
            break;
    };
    //Generate the higher 2 crc bytes. (Ignore the 0x12s)
    uint16_t crc_hi = crc16(&msg[1], msg_len, init_hi);
    //Generate the first part of low crc based on message
    uint16_t crc_lo = crc16(&msg[1], msg_len , init_lo);
    //Now recalculate the low crc to include the high crc bytes
    crc_lo = crc16((uint8_t*)&crc_hi, 2, crc_lo);
    return crc_hi<<16 | crc_lo;
}

//- **************************************************************************
//! \brief
//- **************************************************************************
uint32_t calc_crc_init(uint8_t seq, uint8_t* matrix_table, uint16_t constant) {
    //For given matrix_table, sequence and constant, generate the correct CRC init from the message sequence no
    //Compute CRC16 init from seq byte via GF(2) matrix multiply.
    //Each row of the 16x8 matrix produces one bit of the 16-bit init
    uint16_t result = 0x00000000;
    for (int i=0; i<16; ++i) {
        bool bit = 0;
        for (int j=0; j<8; ++j) {
            bit ^= (matrix_table[i] >> j) & 0x01 & ((seq >> j) & 1);
        }
        if (bit) {
            result |= 0x01 << i;
        }
    }
    return result ^ constant;
}


//- **************************************************************************
//! \brief 
//- **************************************************************************
uint16_t calc_crc16_C9A7(uint16_t crc_init, uint8_t * data_ptr, uint16_t len)
{
  uint16_t tbl_tdx;
  uint16_t i;
  uint16_t crc_u16 = crc_init;

  if(NULL == data_ptr)
  {
    return 0;
  }

  for (i = 0; i < len; i++)
  {
    tbl_tdx = (crc_u16 ^ *data_ptr) & 0xFF;
    crc_u16 = (crc_u16 >> 8) ^ crc16_C9A7_table_lo[tbl_tdx & 0x0F] ^ crc16_C9A7_table_hi[tbl_tdx >> 4];
    data_ptr++;
  }

  return (crc_u16);
}


//- **************************************************************************
//! \brief 
//! Calculate a straightforward CRC16 using the dyson 0xC9A7 poly, using the 
//! given init value
//- **************************************************************************
uint16_t crc16(uint8_t *data, size_t len, uint16_t init) {
    uint16_t crc = init;
    //CRC-16 Dyson — poly 0xC9A7 (reflected 0xE593)
    for (size_t i=0; i<len; ++i) {
        crc ^= data[i];
        for (int j=0; j<8; ++j) {
            if (crc & 0x01) 
                crc = (crc >> 1) ^ 0xE593;
            else 
                crc = crc >> 1;
        }
    }
    return crc & 0xFFFF;    
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
