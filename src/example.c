/*
 * add your license here
 */

#include <stdlib.h>
#include <util/delay_basic.h>
#include <avr/pgmspace.h>


#include <muos/muos.h>
#include <muos/error.h>
#include <muos/rtq.h>
#include <muos/bgq.h>
#include <muos/clpq.h>
#include <muos/clock.h>
#include <muos/serial.h>
#include <muos/io.h>
#include <muos/error.h>

// define events, no main here

void
serial_ping (const struct muos_spriq_entry* event)
{
  muos_output_char ('.');
  muos_clpq_repeat (event, MUOS_CLOCK_SECONDS (1));
}

void
toggle_red_timed (const struct muos_spriq_entry* event)
{
  PIND = _BV(PIND2);
  muos_clpq_repeat (event, MUOS_CLOCK_SECONDS (1));
}

void
toggle_yellow_timed (const struct muos_spriq_entry* event)
{
  PINB = _BV(PINB5);
  muos_clpq_repeat (event, MUOS_CLOCK_MILLISECONDS (125));
}

void
toggle_green_timed (const struct muos_spriq_entry* event)
{
  PIND = _BV(PIND3);
  muos_clpq_repeat (event, MUOS_CLOCK_MILLISECONDS (50));
}


void
clearerror (const struct muos_spriq_entry* event)
{
  (void) event;
  PORTD &= ~_BV(PIND2);
}

void
error (void)
{
  if (muos_error_pending ())
    {
      PORTD |= _BV(PIND2);

#define MUOS_ERROR(name)                                         \
      if (muos_error_check (muos_##name))                        \
        {                                                        \
        }

  MUOS_ERRORS;
#undef MUOS_ERROR
    }

  muos_clpq_at (muos_now (), MUOS_CLOCK_SECONDS (1), clearerror);
}


void
serial_blinkerr (const struct muos_spriq_entry* event)
{
  if (muos_error_pending ())
    {
#define MUOS_ERROR(name)                                         \
      if (muos_error_check (muos_##name))                        \
        {                                                        \
          PIND = _BV(PIND2);                                     \
        }

  MUOS_ERRORS;
#undef MUOS_ERROR
    }

  muos_clpq_repeat (event, MUOS_CLOCK_MILLISECONDS (50));
}

void
serial_printerr (const struct muos_spriq_entry* event)
{
  if (muos_error_pending ())
    {
#define MUOS_ERROR(name)                                         \
      if (muos_error_check (muos_##name))                        \
        {                                                        \
          muos_output_cstr ((const __flash char*)"\r\nE:");      \
          muos_output_uint8 (muos_##name);                       \
          muos_output_cstr ((const __flash char*)"\r\n");        \
        }

  MUOS_ERRORS;
#undef MUOS_ERROR
    }

  muos_clpq_repeat (event, MUOS_CLOCK_SECONDS (2));
}

void
serial_echo (void)
{
  uint8_t data = muos_serial_rx_byte ();

  if (!muos_error_check (muos_error_rx_buffer_underflow))
    {
      muos_output_char (data);
      if (data == '\r')
        muos_output_char ('\n');
    }

  muos_serial_rxrtq_again (serial_echo);
}

void
init (void)
{
  DDRB = _BV(PINB5) | _BV(PINB4);
  DDRD = _BV(PIND2) | _BV(PIND3);

  muos_serial_init ();

  //muos_clpq_at (0, 0, toggle_red_timed);
  //muos_clpq_at (0, 0, toggle_yellow_timed);
  muos_clpq_at (0, 0, toggle_green_timed);
  //  muos_clpq_at (0, 0, serial_ping);
  //muos_clpq_at (0, 0, serial_blinkerr);
}


