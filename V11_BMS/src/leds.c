/*
 * leds.c
 *
 *  Author:  David Pye
 *  Contact: davidmpye@gmail.com
 *  Licence: GNU GPL v3 or later
 */ 

/*-----------------------------------------------------------------------------
    INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "leds.h"
#include "sw_timer.h"

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
//speed of LED sequence
#define LED_SEQ_TIME          50
#define NUM_LEDS              2

#define TIMRER_FREQ_HZ        (8000000ul)
#define PWM_FREQ_HZ           (200ul)
#define CAPTURE_VALUE         ((TIMRER_FREQ_HZ / PWM_FREQ_HZ) - 1ul)

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL TYPES
-----------------------------------------------------------------------------*/
typedef struct 
{
  uint32_t tc_base;
  uint32_t pin_out;
  uint32_t pin_mux;
  enum tc_compare_capture_channel chnl;
}leds_cfg_t;

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL VARIABLES
-----------------------------------------------------------------------------*/
struct tc_module tc_instances[LEDS_NUM];

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL CONSTANTS
-----------------------------------------------------------------------------*/
static const leds_cfg_t leds_cfg[] = 
{
  [LEDS_LED_ERR_LEFT]  = {.tc_base = TC3, .pin_out = PIN_PA19F_TC3_WO1, .pin_mux = PINMUX_PA19F_TC3_WO1, .chnl = TC_COMPARE_CAPTURE_CHANNEL_1 }, // PIN_PA19
  [LEDS_LED_ERR_RIGHT] = {.tc_base = TC2, .pin_out = PIN_PA00F_TC2_WO0, .pin_mux = PINMUX_PA00F_TC2_WO0, .chnl = TC_COMPARE_CAPTURE_CHANNEL_0 }, // PIN_PA00
};

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL FUNCTIONS PROTOTYPES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/
//- **************************************************************************
//! \brief 
//- **************************************************************************
 void leds_init()
 {
  struct tc_config config_tc;
  tc_get_config_defaults(&config_tc);

  for(uint8_t i = 0; i < (uint8_t)LEDS_NUM; i++)
  {
    config_tc.counter_size    = TC_COUNTER_SIZE_16BIT;
    config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
    config_tc.counter_16_bit.value = 0;
    config_tc.counter_16_bit.compare_capture_channel[leds_cfg[i].chnl] = CAPTURE_VALUE;
    config_tc.pwm_channel[0].enabled = true;
    config_tc.pwm_channel[0].pin_out = leds_cfg[i].pin_out;
    config_tc.pwm_channel[0].pin_mux = leds_cfg[i].pin_mux;
    tc_init(&tc_instances[i], (Tc *const)leds_cfg[i].tc_base, &config_tc);
    tc_enable(&tc_instances[i]);
    tc_set_compare_value(&tc_instances[i], leds_cfg[i].chnl, 0);
  }
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void leds_sequence(void)
{
#if 0
  for (int i=0; i<NUM_LEDS; ++i)
  {
    port_pin_set_output_level(leds[i], true);
    sw_timer_delay_ms(LED_SEQ_TIME);
  }

  for (int i = NUM_LEDS-1; i>=0; --i)
  {
    port_pin_set_output_level(leds[i], false);
    sw_timer_delay_ms(LED_SEQ_TIME);
  }
  #endif
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void leds_off()
{
  const uint32_t cc_0_ppt = 0;

  for(uint8_t i = 0; i < (uint8_t)LEDS_NUM; i++)
  {
    tc_set_compare_value(&tc_instances[i], leds_cfg[i].chnl, cc_0_ppt);
  }
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void leds_on()
{
  const uint32_t cc_100_ppt = (CAPTURE_VALUE);

  for(uint8_t i = 0; i < (uint8_t)LEDS_NUM; i++)
  {
    tc_set_compare_value(&tc_instances[i], leds_cfg[i].chnl, cc_100_ppt);
  }
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void leds_blink_leds(int ms)
{
  const uint32_t cc_100_ppt = (CAPTURE_VALUE);
  const uint32_t cc_0_ppt = 0;

  for(uint8_t i = 0; i < (uint8_t)LEDS_NUM; i++)
  {
    tc_set_compare_value(&tc_instances[i], leds_cfg[i].chnl, cc_100_ppt);
  }

  sw_timer_delay_ms(ms/2);

  for(uint8_t i = 0; i < (uint8_t)LEDS_NUM; i++)
  {
    tc_set_compare_value(&tc_instances[i], leds_cfg[i].chnl, cc_0_ppt);
  }

  sw_timer_delay_ms(ms/2);
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void leds_set_led_duty(leds_t led, uint8_t duty_ppt)
{
  if(led < LEDS_NUM)
  {
    uint32_t cc = (CAPTURE_VALUE / 100uL) * duty_ppt;
    tc_set_compare_value(&tc_instances[(uint8_t)led], leds_cfg[(uint8_t)led].chnl, cc);
  }
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void leds_show_pack_flat()
{

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




 