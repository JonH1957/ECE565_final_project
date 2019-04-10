/*
 * Copyright (c) 2019 Inria
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2005-2006 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Kevin Lim
 *          Daniel Carvalho
 */

#ifndef __BASE_SAT_COUNTER_HH__
#define __BASE_SAT_COUNTER_HH__

#include <cstdint>

#include "base/logging.hh"
#include "base/types.hh"

/**
 * Implements an n bit saturating counter and provides methods to
 * increment, decrement, and read it.
 */
class SatCounter
{
  public:
    /**
     * Constructor for the counter. The explicit keyword is used to make
     * sure the user does not assign a number to the counter thinking it
     * will be used as a counter value when it is in fact used as the number
     * of bits.
     *
     * @param bits How many bits the counter will have.
     * @param initial_val Starting value for the counter.
     */
    explicit SatCounter(unsigned bits, uint8_t initial_val = 0)
        : initialVal(initial_val), maxVal((1 << bits) - 1),
          counter(initial_val)
    {
        fatal_if(bits > 8*sizeof(uint8_t),
                 "Number of bits exceeds counter size");
        fatal_if(initial_val > maxVal,
                 "Saturating counter's Initial value exceeds max value.");
    }

    /** Pre-increment operator. */
    SatCounter&
    operator++()
    {
        if (counter < maxVal) {
            ++counter;
        }
        return *this;
    }

    /** Post-increment operator. */
    SatCounter
    operator++(int)
    {
        SatCounter old_counter = *this;
        ++*this;
        return old_counter;
    }

    /** Pre-decrement operator. */
    SatCounter&
    operator--()
    {
        if (counter > 0) {
            --counter;
        }
        return *this;
    }

    /** Post-decrement operator. */
    SatCounter
    operator--(int)
    {
        SatCounter old_counter = *this;
        --*this;
        return old_counter;
    }

    /**
     * Read the counter's value.
     */
    operator uint8_t() const { return counter; }

    /** Reset the counter to its initial value. */
    void reset() { counter = initialVal; }

  private:
    uint8_t initialVal;
    uint8_t maxVal;
    uint8_t counter;
};

#endif // __BASE_SAT_COUNTER_HH__