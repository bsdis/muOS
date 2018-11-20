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

#include <stdint.h>
#include <muos/muos.h>
#include <muos/error.h>

volatile uint8_t muos_errors_pending_;
//TODO: no need to store muos_error_success, array one bit smaller, fix indexing
volatile uint8_t muos_errors_[(muos_errors_end+7)/8];


muos_error
muos_error_set_isr (muos_error err)
{
  if (err && !(muos_errors_[err/8] & 1<<(err%8)))
      {
        MUOS_DEBUG_ERROR_ON;
        muos_errors_[err/8] |= 1<<(err%8);
        ++muos_errors_pending_;
      }
  return err;
}



bool
muos_error_check_isr (muos_error err)
{
  bool ret = false;

  if (muos_errors_pending_)
    {
      ret = muos_error_peek (err);

      muos_errors_[err/8] &= ~(1<<(err%8));

      if (ret)
        --muos_errors_pending_;

#ifdef MUOS_DEBUG_ERROR
      if (!muos_errors_pending_)
        {
          MUOS_DEBUG_ERROR_OFF;
        }
#endif
    }

  return ret;
}



