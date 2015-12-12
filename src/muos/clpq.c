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

#include <muos/clpq.h>
#include <muos/clock.h>

#if MUOS_CLPQ_LENGTH > 0

muos_clpq_type muos_clpq;

#if 0

          muos_clpq.descriptor.spriq[0].when < when


#endif
          //(muos_spriq_priority)(when - muos_clpq.descriptor.spriq[0].when) < (muos_spriq_priority)(((muos_spriq_priority)~0)/2

bool
muos_clpq_schedule (muos_spriq_priority when)
{
  muos_interrupt_disable ();
  if (muos_clpq.descriptor.used
      &&
      (muos_spriq_priority)(when - muos_clpq.descriptor.spriq[0].when) < (muos_spriq_priority)(((muos_spriq_priority)~0)/2)
      //muos_clpq.descriptor.spriq[0].when <= when
      )
    {
      (void) when;
      struct muos_spriq_entry event;
      muos_spriq_pop (&muos_clpq.descriptor, &event);
      muos_interrupt_enable ();
      event.fn (&event);
      return true;
    }
  else
    {
      muos_interrupt_enable ();
      return false;
    }
}

void
muos_clpq_at (muos_spriq_priority base, muos_spriq_priority when, muos_spriq_function what)
{
  muos_interrupt_disable ();
  muos_spriq_push (&muos_clpq.descriptor, base, when, what);
  muos_interrupt_enable ();
}

#endif
