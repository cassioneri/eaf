// SPDX-License-Identifier: GPL-2.0-only
// SPDX-SnippetCopyrightText: Copyright (c) 2012-2019, Oracle and/or its affiliates.
// SPDX-SnippetCopyrightText: Copyright (c) 2007-2012, Stephen Colebourne & Michael Nascimento Santos

/*
 Copyright (c) 2012, 2019, Oracle and/or its affiliates. All rights reserved.
 DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.

 This code is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License version 2 only, as
 published by the Free Software Foundation.  Oracle designates this
 particular file as subject to the "Classpath" exception as provided
 by Oracle in the LICENSE file that accompanied this code.

 This code is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 version 2 for more details (a copy is included in the LICENSE file that
 accompanied this code).

 You should have received a copy of the GNU General Public License version
 2 along with this work; if not, write to the Free Software Foundation,
 Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.

 Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 or visit www.oracle.com if you need additional information or have any
 questions.

 ---

 This file is available under and governed by the GNU General Public
 License version 2 only, as published by the Free Software Foundation.
 However, the following notice accompanied the original version of this
 file:

 Copyright (c) 2007-2012, Stephen Colebourne & Michael Nascimento Santos

 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of JSR-310 nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file openjdk.hpp
 *
 * @brief Algorithms on the Gregorian calendar from OpenJDK [1].
 *
 * This code is a supplementary material to [2].
 *
 *     [1] https://openjdk.org
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_ALGORITHMS_OPENJDK_HPP
#define EAF_ALGORITHMS_OPENJDK_HPP

#include "eaf/date.hpp"

#include <cstdint>

struct openjdk {

  // Original epoch: 1 January 1970.

  // https://tinyurl.com/ybb3nnhw
  static inline
  date32_t to_date(int32_t epochDay) {

    long zeroDay = epochDay + DAYS_0000_TO_1970;
    // find the march-based year
    zeroDay -= 60;  // adjust to 0000-03-01 so leap day is at end of four year cycle
    long adjust = 0;
    if (zeroDay < 0) {
        // adjust negative years to positive for calculation
        long adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
        adjust = adjustCycles * 400;
        zeroDay += -adjustCycles * DAYS_PER_CYCLE;
    }
    long yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
    long doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
    if (doyEst < 0) {
        // fix estimate
        yearEst--;
        doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
    }
    yearEst += adjust; // reset any negative year
    int marchDoy0 = (int) doyEst;

    // convert march-based values back to january-based
    int marchMonth0 = (marchDoy0 * 5 + 2) / 153;
    int month = (marchMonth0 + 2) % 12 + 1;
    int dom = marchDoy0 - (marchMonth0 * 306 + 5) / 10 + 1;
    yearEst += marchMonth0 / 10;

    // check year now we are certain it is correct
    int year = (int) yearEst;

    return { int32_t(year), uint32_t(month), uint32_t(dom) };
  }

  // https://tinyurl.com/ydxp5346
  static inline
  int32_t to_rata_die(int32_t year, uint32_t month, uint32_t day) {

    long y = year;
    long m = month;
    long total = 0;
    total += 365 * y;
    if (y >= 0) {
      total += (y + 3) / 4 - (y + 99) / 100 + (y + 399) / 400;
    } else {
      total -= y / -4 - y / -100 + y / -400;
    }
    total += ((367 * m - 362) / 12);
    total += day - 1;
    if (m > 2) {
      total--;
      if (isLeapYear(y) == false) {
          total--;
      }
    }

    return total - DAYS_0000_TO_1970;
  }

private:

  // https://tinyurl.com/y99vybz9
  static int constexpr DAYS_PER_CYCLE = 146097;
  static int constexpr DAYS_0000_TO_1970 = (DAYS_PER_CYCLE * 5L) - (30L * 365L + 7L);

  // https://tinyurl.com/ybdspcjc
  static inline
  bool isLeapYear(long prolepticYear) {
    return ((prolepticYear & 3) == 0) && ((prolepticYear % 100) != 0 || (prolepticYear % 400) == 0);
  }

}; // struct openjdk

#endif // EAF_ALGORITHMS_OPENJDK_HPP
