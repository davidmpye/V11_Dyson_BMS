/*
 * sw_timer.c
 *
 * Created: 21-Jan-26 16:31:01
 *  Author: GYV1SF4
 */ 
 /*-----------------------------------------------------------------------------
    INCLUDE FILES
-----------------------------------------------------------------------------*/
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
#define SW_TIMER_CLOCK_MULTIPLIER   1

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL TYPES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL VARIABLES
-----------------------------------------------------------------------------*/
static volatile uint32_t sw_timer_clock = 0;
static volatile sw_timer delay_timer = 0;

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
void SysTick_Handler(void)
{
  sw_timer_clock += SW_TIMER_CLOCK_MULTIPLIER;

  if(sw_timer_clock < SW_TIMER_CLOCK_MULTIPLIER)
  {
    sw_timer_clock++;
  }
}

//- **************************************************************************
//! \brief 
//- **************************************************************************
void sw_timer_init(void)
{
  uint32_t cycles_per_ms = system_gclk_gen_get_hz(0);
  cycles_per_ms /= 1000;

  SysTick_Config(cycles_per_ms);

  sw_timer_clock = 0;
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void sw_timer_start(sw_timer * sw_timer_ptr)
{
  *sw_timer_ptr = sw_timer_clock;
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void sw_timer_stop(sw_timer * sw_timer_ptr)
{
  *sw_timer_ptr = 0;
}

//- **************************************************************************
//! \brief
//- **************************************************************************
bool sw_timer_is_started(sw_timer * sw_timer_ptr)
{
  // A timer is never equal to 0
  // The 0 value is reserved to the timer stopped
  if (*sw_timer_ptr != 0)
    return(1);

  return(0);
}

//- **************************************************************************
//! \brief
//- **************************************************************************
bool sw_timer_is_elapsed(sw_timer * sw_timer_ptr, uint32_t timeout)
{
  uint32_t Delay;

  // A timer is never equal to 0
  // The 0 value is reserved to the timer stopped
  if (*sw_timer_ptr == 0)
  {
    return(1);
  }
  else
  {
    Delay = (sw_timer)(sw_timer_clock - *sw_timer_ptr);

    if(sw_timer_clock < *sw_timer_ptr)
    {
      // The 0 value had been "jump" so we must substract 1 to the delay
      --Delay;
    }

    if ((Delay > timeout) || (timeout == 0))
    {
      // The timer is stopped or elapsed
      *sw_timer_ptr = 0;
      return true;
    }
  }

  return false;
}

//- **************************************************************************
//! \brief
//- **************************************************************************
sw_timer sw_timer_get_elapsed_time(sw_timer * sw_timer_ptr)
{
  uint32_t Delay = 0;

  // A timer is never equal to 0
  // The 0 value is reserved to the timer stopped
  if ( *sw_timer_ptr == 0 )
  {
    // This function must not be call with a stopped tempo
    Assert(false);
  }
  else
  {
    Delay = (sw_timer)(sw_timer_clock - *sw_timer_ptr);

    if(sw_timer_clock < *sw_timer_ptr)
    {
      // The 0 value had been "jump" so we must substract 1 to the delay
      --Delay;
    }
  }

  return Delay;
}

//- **************************************************************************
//! \brief
//- **************************************************************************
void sw_timer_delay_ms(uint32_t sw_timer_delay_ms)
{
  sw_timer_start((sw_timer *)&delay_timer);
  while (false == sw_timer_is_elapsed((sw_timer *)&delay_timer, sw_timer_delay_ms)) {}
}

/*-----------------------------------------------------------------------------
    DEFINITION OF LOCAL FUNCTIONS
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/