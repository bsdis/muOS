/*
 *      mµOS            - my micro OS
 *
 * Copyright (C)
 *      2018                           Christian Thäter <ct@pipapo.org>
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

#ifndef MUOS_EEPROM_H
#define MUOS_EEPROM_H

#include <stdlib.h>
#include <muos/muos.h>

#include MUOS_HW_HEADER

/*
  asynchronous EEPROM access driver
*/

enum muos_eeprom_mode
  {
   MUOS_EEPROM_IDLE,           // no operation in progress
   MUOS_EEPROM_READ,           // eeprom -> memory
   MUOS_EEPROM_VERIFY,         // eeprom == memory, will set error_eeprom_verify on fail
   MUOS_EEPROM_WRITE,          // eeprom <- memory, erase first
   MUOS_EEPROM_WRITEVERIFY,    // eeprom <- memory, eeprom == memory, erase first
   MUOS_EEPROM_WRITEONLY,      // eeprom <- memory, no erase
   //PLANNED: MUOS_EEPROM_WRITESMART,     // eeprom -> memory, smart write, erase/write only when necessary
   //PLANNED: MUOS_EEPROM_REFRESH,          // eeprom <- eeprom, read, erase, write to refresh content
   MUOS_EEPROM_ERASE,          // erase eeprom <- 0xff
   //PLANNED: MUOS_EEPROM_ERASESECURE,       // eeprom <- ^eeprom, eeprom <- 0x00, erase, secure erase (equivalent wear)
  };

/*
  WRITESMART algorithm:

  read first,
  compare if:
    no change -> return
    only bits need to be cleared -> mask, write
    else -> erase, write
  verify
*/


typedef void (*muos_eeprom_callback)(void);

extern enum muos_eeprom_mode muos_hw_eeprom_state (void);
extern muos_error muos_hw_eeprom_access (enum muos_eeprom_mode mode,
                                         void* mem,
                                         void* eeprom,
                                         size_t size,
                                         muos_eeprom_callback complete);


//eeprom_api:
//: .Query State
//: ----
//: enum muos_eeprom_mode muos_eeprom_state ()
//: ----
//:
//: Polls the state of the driver.
//:
//: Returns 0 (MUOS_EEPROM_IDLE) when no operation is in progress.
//:
inline enum muos_eeprom_mode
muos_eeprom_state (void)
{
  return muos_hw_eeprom_state ();
}

//PLANNED: query current position (ram? eeprom?)
//inline void*
//muos_eeprom_pos (void)
//{
//  return muos_hw_eeprom_pos ();
//}


//eeprom_api:
//: .Parameters and returns
//: The parameters used for accessing the eeprom are orthogonal though all access functions
//:
//: +address+::
//:   start address in ram
//:
//: +eeprom+::
//:   start address in EEPROM
//:
//: +size+::
//:   size in bytes of for the operation
//:
//: +complete+::
//:   callback function called upon completion (also on failure)
//:
//: The access function only return an error when they can not start an asynchronous job.
//: All other errors are set asynchronously and should be handled in the complete callback.
//:
//eeprom_api:
//: .Reading
//: ----
//: muos_error muos_eeprom_read (void* address,
//:                              void* eeprom,
//:                              size_t size,
//:                              muos_eeprom_callback complete)
//: ----
//:
//: Reads the EEPROM.
//:
static inline muos_error
muos_eeprom_read (void* mem,
                  void* eeprom,
                  size_t size,
                  muos_eeprom_callback complete)
{
  return muos_hw_eeprom_access (MUOS_EEPROM_READ, mem, eeprom, size, complete);
}


//eeprom_api:
//: .Verifying
//: ----
//: muos_error muos_eeprom_verify (void* address,
//:                                void* eeprom,
//:                                size_t size,
//:                                muos_eeprom_callback complete)
//: ----
//:
//: Compares a block of memory with eeprom contents. Aborts on first error, calling 'complete'.
//:
static inline muos_error
muos_eeprom_verify (void* mem,
                    void* eeprom,
                    size_t size,
                    muos_eeprom_callback complete)
{
  return muos_hw_eeprom_access (MUOS_EEPROM_VERIFY, mem, eeprom, size, complete);
}


//eeprom_api:
//: .Writing
//: ----
//: muos_error muos_eeprom_write (void* address,
//:                              void* eeprom,
//:                              size_t size,
//:                              muos_eeprom_callback complete)
//:
//: muos_error muos_eeprom_writeverify (void* address,
//:                                     void* eeprom,
//:                                     size_t size,
//:                                     muos_eeprom_callback complete)
//:
//: muos_error muos_eeprom_writeonly (void* address,
//:                                   void* eeprom,
//:                                   size_t size,
//:                                   muos_eeprom_callback complete)
//: ----
//:
//: Transfers data from memory to eeprom.
//:
//: write::
//:   Erases data before writing. Faster than erasing the block first.
//:
//: writeverify:
//:   Erases data before writing. Verifies after write.
//:
//: writeonly:
//:   Only writes (clears bits) without erasing.
//:
static inline muos_error
muos_eeprom_write (void* mem,
                   void* eeprom,
                   size_t size,
                   muos_eeprom_callback complete)
{
  return muos_hw_eeprom_access (MUOS_EEPROM_WRITE, mem, eeprom, size, complete);
}


static inline muos_error
muos_eeprom_writeverify (void* mem,
                         void* eeprom,
                         size_t size,
                         muos_eeprom_callback complete)
{
  return muos_hw_eeprom_access (MUOS_EEPROM_WRITEVERIFY, mem, eeprom, size, complete);
}


static inline muos_error
muos_eeprom_writeonly (void* mem,
                       void* eeprom,
                       size_t size,
                       muos_eeprom_callback complete)
{
  return muos_hw_eeprom_access (MUOS_EEPROM_WRITEONLY, mem, eeprom, size, complete);
}


//eeprom_api:
//: .Erasing
//: ----
//: muos_error muos_eeprom_erase (void* eeprom,
//:                               size_t size,
//:                               muos_eeprom_callback complete)
//: ----
//:
//: Erases the given range.
//:
static inline muos_error
muos_eeprom_erase (void* eeprom,
                   size_t size,
                   muos_eeprom_callback complete)
{
  return muos_hw_eeprom_access (MUOS_EEPROM_ERASE, NULL, eeprom, size, complete);
}


#endif
