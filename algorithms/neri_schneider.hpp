// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file neri_schneider.hpp
 *
 * @brief Algorithms on the Gregorian calendar.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_ALGORITHMS_NERI_SCHNEIDER_H
#define EAF_ALGORITHMS_NERI_SCHNEIDER_H

#include "eaf/date.hpp"

#include <cstdint>

struct neri_schneider {

  // Shift and correction constants.
  static uint32_t constexpr s = 82;
  static uint32_t constexpr K = 719468 + 146097 * s;
  static uint32_t constexpr L = 400 * s;

  /**
   * @brief Finds the proleptic Gregorian date from its rata die.
   *
   * The epoch is 1 January 1970 (Unix epoch) of the Gregorian calendar.
   *
   * @param N         The rata die.
   *
   * @return The proleptic Gregorian date.
   */
  static inline
  date32_t to_date(int32_t N_U) {

    // Rata die shift.
    uint32_t const N = N_U + K;

    // Century.
    uint32_t const N_1 = 4 * N + 3;
    uint32_t const C   = N_1 / 146097;
    uint32_t const N_C = N_1 % 146097 / 4;

    // Year.
    uint32_t const N_2 = 4 * N_C + 3;
    uint64_t const P_2 = uint64_t(2939745) * N_2;
    uint32_t const Z   = uint32_t(P_2 / 4294967296);
    uint32_t const N_Y = uint32_t(P_2 % 4294967296) / 2939745 / 4;
    uint32_t const Y   = 100 * C + Z;

    // Month and day.
    uint32_t const N_3 = 2141 * N_Y + 197913;
    uint32_t const M   = N_3 / 65536;
    uint32_t const D   = N_3 % 65536 / 2141;

    // Map. (Notice the year correction, including type change.)
    uint32_t const J   = N_Y >= 306;
    int32_t  const Y_G = (Y - L) + J;
    uint32_t const M_G = J ? M - 12 : M;
    uint32_t const D_G = D + 1;

    return { Y_G, M_G, D_G };
  }

  /**
   * @brief Calculates the rata die of a given proleptic Gregorian date.
   *
   * The epoch is 1 January 1970 of the Gregorian calendar.
   *
   * @param Y_G       The year.
   * @param M_G       The month.
   * @param D_G       The day.
   *
   * @return The rata die.
   */
  static inline
  int32_t to_rata_die(int32_t Y_G, uint32_t M_G, uint32_t D_G) {

    // Map. (Notice the year correction, including type change.)
    uint32_t const J = M_G <= 2;
    uint32_t const Y = (uint32_t(Y_G) + L) - J;
    uint32_t const M = J ? M_G + 12 : M_G;
    uint32_t const D = D_G - 1;
    uint32_t const C = Y / 100;

    // Rata die.
    uint32_t const y_star = 1461 * Y / 4 - C + C / 4;
    uint32_t const m_star = (979 * M - 2919) / 32;
    uint32_t const N      = y_star + m_star + D;

    // Rata die shift.
    uint32_t const N_U = N - K;

    return N_U;
  }

  date32_t static constexpr date_min     = {  -32800, 3,  1 };
  date32_t static constexpr date_max     = { 2906945, 2, 28 };
  int32_t  static constexpr rata_die_min =  -12699422;
  int32_t  static constexpr rata_die_max = 1061042401;

}; // struct neri_schneider

#endif // EAF_ALGORITHMS_NERI_SCHNEIDER_H
