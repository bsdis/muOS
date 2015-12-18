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

#include <muos/clock.h>
#include <muos/clpq.h>

volatile muos_clock muos_clock_count_;
muos_clock muos_now_;

MUOS_HW_ISR(MUOS_HW_CLOCK_ISRNAME_OVERFLOW(MUOS_CLOCK_HW))
{
  ++muos_clock_count_;
}

// compmatch interrupt is only used to wake the mainloop
MUOS_HW_EMPTY_ISR(MUOS_HW_CLOCK_ISRNAME_COMPMATCH(MUOS_CLOCK_HW, MUOS_CLOCK_HW_COMPAREMATCH));




