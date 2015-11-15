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

#ifndef MUOS_HW_ATMEL_ATMEGA_H
#define MUOS_HW_ATMEL_ATMEGA_H

#include <muos/hw/atmel/avr.h>

#define MUOS_HW_TIMER_OFF       0
#define MUOS_HW_TIMER_NODIV     _BV(CS00)
#define MUOS_HW_TIMER_DIV8      _BV(CS01)
#define MUOS_HW_TIMER_DIV64     _BV(CS01) | _BV(CS00)
#define MUOS_HW_TIMER_DIV256    _BV(CS02)
#define MUOS_HW_TIMER_DIV1024   _BV(CS02) | _BV(CS00)

#define MUOS_HW_TIMER_ISRNAME_OVERFLOW(hw) TIMER##hw##_OVF_vect
#define MUOS_HW_TIMER_REGISTER(hw) TCNT##hw


#define MUOS_HW_TIMER_PRESCALE_SET(hw, prescale)  \
  TCCR##hw##B = (MUOS_HW_TIMER_##prescale)

#define MUOS_HW_TIMER_ISR_OVERFLOW_ENABLE(hw)            \
  TIMSK##hw |= _BV(TOIE0)

#endif
