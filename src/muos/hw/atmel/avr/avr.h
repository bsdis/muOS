/*
 *      mµOS            - my micro OS
 *
 * Copyright (C)
 *      2015                            Christian Thäter <ct@pipapo.org>
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

#ifndef MUOS_HW_ATMEL_AVR_H
#define MUOS_HW_ATMEL_AVR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#define MUOS_PSTR(s) PSTR(s)
typedef const __flash char muos_flash_cstr[];


static inline void
muos_interrupt_enable ()
{
  sei ();
}

static inline void
muos_interrupt_disable ()
{
  cli ();
}

static inline void
muos_hw_sleep_prepare (const uint8_t mode)
{
  //TODO: configureable mode
  set_sleep_mode (mode);
  sleep_enable();
}

static inline void
muos_hw_sleep (void)
{
#if MUOS_DEBUG_BUSY ==1
  PORTB &= ~_BV(PINB5);
#endif
  sei ();
  sleep_cpu ();
  cli ();
#if MUOS_DEBUG_BUSY ==1
  PORTB |= _BV(PINB5);
#endif
}

static inline void
muos_hw_sleep_done (void)
{
  sleep_disable();
}


#endif
