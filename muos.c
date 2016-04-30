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

#include <muos/muos.h>
#include <muos/error.h>
#include <muos/clock.h>
#include <muos/hpq.h>
#include <muos/bgq.h>
#include <muos/clpq.h>
#include <muos/serial.h>
#include <muos/cppm.h>

//PLANNED: stash muos_now_ away on recursive mainloops

volatile struct muos_status_flags muos_status;

void
muos_sleep (void)
{
  if (muos_clpq_set_compmatch ())
    {
      //TODO: select sleep mode depending on active hardware (adc, usart)
      muos_hw_sleep_prepare (MUOS_SCHED_SLEEP);
      // muos_hw_sleep () enables interrupts while sleeping
      muos_hw_sleep ();
      muos_hw_sleep_done ();
    }
  else
    {
      // busywait for timespans which are to small for compmatch
      muos_interrupt_enable ();
      while (MUOS_CLOCK_REGISTER < (typeof(MUOS_CLOCK_REGISTER)) muos_clpq.descriptor.spriq[0].when);
      muos_interrupt_disable ();
    }
}

//PLANNED: document and implement
void muos_die (void)
{
  muos_hw_shutdown ();
}

void
muos_start (void)
{
#include <muos/init.inc>
}


static uint8_t sched_depth_;


muos_error
muos_wait (muos_wait_fn fn, intptr_t param, muos_shortclock timeout)
{
  if (sched_depth_ >= MUOS_SCHED_DEPTH)
    {
      return muos_warn_sched_depth;
    }

  muos_clock start = muos_now_ = muos_clock_now ();

  ++sched_depth_;

  muos_interrupt_disable ();

  while (1)
    {
      do
        {
          do
            {
              do
                {
                  if (fn && fn (param))
                    {
                      muos_interrupt_enable ();
                      --sched_depth_;
                      return muos_success;
                    }

                  muos_now_ = muos_clock_now_isr ();

                  if (muos_clock_elapsed (muos_now_, start) > timeout)
                    {
                      muos_interrupt_enable ();
                      --sched_depth_;
                      return muos_warn_wait_timeout;
                    }

                  if (muos_error_pending ())
                    {
                      MUOS_ERRORFN ();
                      muos_interrupt_disable ();
                    }
                }
               while (muos_clpq_schedule (muos_now_));
            }
          while (muos_hpq_schedule ());
        }
      while (muos_bgq_schedule ());

      muos_sleep ();
    }
}


muos_error
muos_yield (uint8_t count)
{
  if (sched_depth_ >= MUOS_SCHED_DEPTH)
    {
      return muos_warn_sched_depth;
    }

  ++sched_depth_;
  ++count;
  muos_interrupt_disable ();

  do
    {
      do
        {
          do
            {
              --count;
              muos_now_ = muos_clock_now_isr ();

              if (muos_error_pending ())
                {
                  MUOS_ERRORFN ();
                  muos_interrupt_disable ();
                }
            }
          while (count && muos_clpq_schedule (muos_now_));
        }
      while (count && muos_hpq_schedule ());
    }
  while (count && muos_bgq_schedule ());

  muos_interrupt_enable ();
  --sched_depth_;

  return muos_success;
}



int __attribute__((OS_main))
main()
{
  CLKPR = _BV(CLKPCE);
  CLKPR = 0;

  #if 0
#if F_CPU != 16000000UL
  CLKPR = _BV(CLKPCE);
#if F_CPU == 62500UL
  CLKPR = _BV(CLKPS3);
#endif
#if F_CPU == 125000UL
  CLKPR = _BV(CLKPS2) | _BV(CLKPS1)| _BV(CLKPS0);
#endif
#if F_CPU == 250000UL
  CLKPR = _BV(CLKPS2) | _BV(CLKPS1);
#endif
#if F_CPU == 500000UL
  CLKPR = _BV(CLKPS2) | _BV(CLKPS0);
#endif
#if F_CPU == 1000000UL
  CLKPR = _BV(CLKPS2);
#endif
#if F_CPU == 2000000UL
  CLKPR = _BV(CLKPS1) | _BV(CLKPS0);
#endif
#if F_CPU == 4000000UL
  CLKPR = _BV(CLKPS1);
#endif
#if F_CPU == 8000000UL
  CLKPR = _BV(CLKPS0);
#endif
#endif
#endif

  //TODO: how to init all muos structures .. #define MUOS_EXPLICIT_INIT

#ifdef MUOS_DEBUG
  muos_debug_init ();
#endif

#if MUOS_DEBUG_BUSY ==1
  //TODO: hardware dependent
  //PLANNED: debug driver
  DDRB = _BV(PINB5) | _BV(PINB4);
  PORTB = _BV(PINB5);
#endif

#if MUOS_HPQ_LENGTH >= 2
  muos_hpq_pushback (MUOS_INITFN);
  muos_hpq_pushback (muos_start);
#elif MUOS_BGQ_LENGTH >= 2
  muos_bgq_pushback (MUOS_INITFN);
  muos_bgq_pushback (muos_start);
#else
  MUOS_INITFN ();
  muos_start ();
#endif

  while (1)
    {
      do
        {
          do
            {
              do
                {
                  muos_now_ = muos_clock_now_isr ();

                  if (muos_error_pending ())
                    {
                      MUOS_ERRORFN ();
                      muos_interrupt_disable ();
                    }
                }
              while (muos_clpq_schedule (muos_now_));
            }
          while (muos_hpq_schedule ());
        }
      while (muos_bgq_schedule ());

      muos_sleep ();
    }
}
