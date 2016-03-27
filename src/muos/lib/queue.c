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

#include <muos/lib/queue.h>
#include <muos/error.h>

static inline intptr_t
muos_queue_pop (muos_queue_vptr queue, const muos_queue_size size)
{
  intptr_t ret = queue->queue[queue->start];
  queue->start += 1;
  queue->len -= 1;
  if (queue->start >= size)
    queue->start -= size;
  return ret;
}


bool
muos_queue_schedule (muos_queue_vptr queue, muos_queue_size size)
{
  if (queue->len)
    {
      intptr_t fn = muos_queue_pop (queue, size);

      if (fn>0)
        {
          muos_interrupt_enable ();
          ((muos_queue_function)(fn))();
        }
      else
        {
          intptr_t arg = muos_queue_pop (queue, size);
          muos_interrupt_enable ();
          ((muos_queue_function_arg)(-fn))(arg);
        }
      muos_interrupt_disable ();
      return true;
    }
  return false;
}



static void
muos_queue_pushback_intern (muos_queue_vptr queue, muos_queue_size size, intptr_t value)
{
  uint8_t index = queue->start+queue->len;
  if (index >= size)
    index -= size;
  queue->queue[index] = value;
  queue->len += 1;
}


void
muos_queue_pushback (muos_queue_vptr queue, muos_queue_size size, muos_queue_function fn)
{
  muos_queue_pushback_intern (queue, size, (intptr_t) fn);
}

void
muos_queue_pushback_arg (muos_queue_vptr queue, muos_queue_size size, muos_queue_function_arg fn, intptr_t arg)
{
  muos_queue_pushback_intern (queue, size, -(intptr_t) fn);
  muos_queue_pushback_intern (queue, size, arg);
}


static void
muos_queue_pushfront_intern (muos_queue_vptr queue, muos_queue_size size, intptr_t value)
{
  queue->start -= 1;
  queue->len += 1;
  if (queue->start >= size)
    queue->start += size;
  queue->queue[queue->start] = value;
}


void
muos_queue_pushfront (muos_queue_vptr queue, muos_queue_size size, muos_queue_function fn)
{
  muos_queue_pushfront_intern (queue, size, (intptr_t) fn);
}

void
muos_queue_pushfront_arg (muos_queue_vptr queue, muos_queue_size size, muos_queue_function_arg fn, intptr_t arg)
{
  muos_queue_pushfront_intern (queue, size, arg);
  muos_queue_pushfront_intern (queue, size, -(intptr_t) fn);
}



