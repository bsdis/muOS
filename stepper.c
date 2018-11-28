/*
 *      mµOS            - my micro OS
 *
 * Copyright (C)
 *      2018                            Christian Thäter <ct@pipapo.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef MUOS_STEPPER

#include <muos/stepper.h>

struct stepper_state muos_steppers[MUOS_STEPPER_COUNT];




#ifdef MUOS_STEPPER_ENABLE_ALL_HW
muos_error
muos_stepper_all_on (void)
{

  for (uint8_t i=0; i<MUOS_STEPPER_COUNT)
    {
      //muos_steppers[i]
    }

  //  if state < unknown
  // && config is valid
  // check configuration
  // init prescaler / but timer is stopped
  // enable controller
  // start timers
  // set state to on
}

muos_error
muos_stepper_all_off (void)
{
  //if state > off
  //   stop timers
  //   disable controller
  //   state = off
}
#endif


// zeros the axis relative to the current position
#if 0 //TODO: implement
muos_error
muos_stepper_set_zero (uint8_t hw, int32_t offset)
{
  //  if state == ON || READY
                //zero
}
#endif



static
enum muos_stepper_arming_state before_calibration[MUOS_STEPPER_COUNT];


muos_error
muos_hw_stepper_start (uint8_t hw, uint8_t prescale, uint16_t speed_raw);

muos_error
muos_stepper_cal_mov (uint8_t hw,
                      uint8_t prescale,
                      uint16_t speed_raw,
                      int32_t offset)
{
    MUOS_DEBUG_C1_TOGGLE;

  if (hw >= MUOS_STEPPER_COUNT)
    return muos_error_nohw;

  //FIXME: iter all states && none calibrating
  if (muos_steppers[hw].state >= MUOS_STEPPER_SLOW)
    {
      return muos_error_stepper_state;
    }

  if (speed_raw < 8192)
    {
      return muos_error_stepper_range;
    }

  (void) offset;
  //muos_hw_stepper_set_callback (hw, offset, callback); // isr callback func

  muos_hw_stepper_start (hw, prescale, speed_raw);
  before_calibration[hw]=muos_steppers[hw].state;

  muos_steppers[hw].state = MUOS_STEPPER_CAL;

  return muos_success;
}



#endif
