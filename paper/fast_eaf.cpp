// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file fast_eaf.cpp
 *
 * @brief Command line program that calculates coefficients and upper bound
 * of fast EAF.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include <boost/multiprecision/cpp_int.hpp>

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdio.h>

using integer = ::boost::multiprecision::int128_t;

/**
 * @brief Coefficients of EAF.
 */
struct eaf_t {
  uint64_t a;
  int64_t  b;
  uint64_t d;
};

/**
 * @brief Coefficients and upper bound of fast EAFs.
 */
struct fast_eaf_t {
  eaf_t    fast;
  uint32_t k;
  uint64_t U;
};

/**
 * @brief Output stream operator for fast_eaf_t.
 *
 * @param os        The output stream.
 * @param eaf       The fast_eaf_t object.
 *
 * @return os.
 */
std::ostream& operator <<(std::ostream& os, fast_eaf_t const eaf) {
  os <<
    "a'          = " << eaf.fast.a << "\n"
    "b'          = " << eaf.fast.b << "\n"
    "d'          = ";
  if (eaf.k == 64)
    os << "18446744073709551616\n"; // = 2^64
  else
    os << eaf.fast.d << "\n";
  return os <<
    "k           = " << eaf.k << "\n"
    "upper bound = " << eaf.U << '\n';
}

/**
 * @brief The two approximation modes of a'.
 *
 * This selects which approximation of a' should be used: that of Theorem 2
 * (where a' is rounded up) or Theorem 3 (where a' is rounded down).
 */
enum class rounding_t {
  up,  // a' is as in Theorem 2.
  down // a' is as in Theorem 3.
};

/**
 * @brief Finds coefficients and upper bound of fast EAF.
 *
 * @param   k       Exponent of the divisor.
 * @param   eaf     Original EAF.
 */
fast_eaf_t
get_fast_eaf(rounding_t rounding, uint32_t k, eaf_t const& eaf)
noexcept {

  bool    const is_rounding_up = rounding == rounding_t::up;

  integer const p2_k     = integer{ 1 } << k; // 2^k
  integer const p2_k_a   = p2_k * eaf.a;      // 2^k * a
  integer const q_p2_k_a = p2_k_a / eaf.d;    // 2^k * a / d
  integer const r_p2_k_a = p2_k_a % eaf.d;    // 2^k * a % d

  integer const a_p      = is_rounding_up ? q_p2_k_a + 1 : q_p2_k_a; // a'
  integer const epsilon  = is_rounding_up ? eaf.d - r_p2_k_a : r_p2_k_a;

  // Helper function used in the calculation of b'.
  // g(n) = a' * n - 2^k * f(n)
  auto g = [&](integer const n) {

    integer const num = eaf.a * n + eaf.b;

    // Operator / implements truncation towards zero and must be
    // adjusted for negative numerators to give the result of Euclidean
    // division.
    integer const adj_num = num >= 0 ? num : num - (eaf.d - 1);

    integer const f_n = adj_num / eaf.d;
    integer const g_n = a_p * n - p2_k * f_n;

    return g_n;
  };

  // b'
  integer const b_p = [&]() {
    if (is_rounding_up) {
      integer min = g(0);
      for (uint64_t n = 1; n < eaf.d; ++n)
        min = std::min(min, g(n));
      return -min;
    }
    else {
      integer max = g(0);
      for (uint64_t n = 1; n < eaf.d; ++n)
        max = std::max(max, g(n));
      return p2_k - 1 - max;
    }
  }();

  auto const Q = [&](integer const n) {

    if (is_rounding_up) {

      // epsilon * q + a' * n + b' - 2^k * f(n) >= 2^k <=>
      // epsilon * q + b' + a' * n - 2^k * f(n) >= 2^k <=>
      // epsilon * q + b' + g(n) >= 2^k                <=>
      // epsilon * q >= 2^k - (g(n) + b')              <=>
      // epsilon * q >= h(n), where h(n) := 2^k - (g(n) + b').

      integer const h = p2_k - (g(n) + b_p);

      // Q(n) = min{ q >= 0 ; epsilon * q >= h(n) } (Problem 1)

      // * If h(n) <= 0, then epsilon * q >= 0 >= h(n) for all q >= 0 and
      // therefore the solution of Problem 1 is q = 0.

      // * Otherwise, the solution of Problem 1 is
      // q = (h(n) + epsilon - 1) / epsilon.

      return h <= 0 ? 0 : (h + (epsilon - 1)) / epsilon;
    }

    else {

      // -epsilon * q + a' * n + b' - 2^k * f(n) < 0 <=>
      // -epsilon * q + b' + a' * n - 2^k * f(n) < 0 <=>
      // -epsilon * q + b' + g(n) < 0                <=>
      // epsilon * q > b' + g(n)                     <=>
      // epsilon * q > h(n), where h(n) = b' + g(n).

      integer const h = g(n) + b_p;

      // Q(n) = min{ q >= 0 ; epsilon * q > h(n) } (Problem 2)

      // * If h(n) < 0, then epsilon * q >= 0 > h(n) for all q >= 0 and
      // therefore, the solution of Problem 2 is q = 0.

      // * Otherwise, the solution of Problem 2 is
      // q = h(n) / epsilon + 1.

      return h < 0 ? 0 : h / epsilon + 1;
    }
  };

  auto const P = [&](integer const n) {
    return Q(n) * eaf.d + n;
  };

  integer U = P(0);
  for (uint64_t n = 1; n < eaf.d; ++n)
    U = std::min(U, P(n));

  assert(a_p <= std::numeric_limits<uint64_t>::max());
  assert(b_p <= std::numeric_limits<int64_t >::max());
  assert(U <= std::numeric_limits<uint64_t>::max());

  return { { uint64_t(a_p), int64_t(b_p), uint64_t(p2_k) },
    k, uint64_t(U) };
}

int main(int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr << argv[0] << ": requires at least 5 arguments: rounding "
      "a b d k\n";
    exit(1);
  }

  rounding_t rounding;
  if (std::strncmp(argv[1], "up", 2) == 0 && argv[1][2] == '\0')
    rounding = rounding_t::up;
  else if (std::strncmp(argv[1], "down", 4) == 0 && argv[1][4] == '\0')
    rounding = rounding_t::down;
  else {
    std::cerr << argv[0] << ": unknown 'rounding': " << argv[1] << '\n';
    exit(1);
  }

  char* end_ptr;

  int64_t  constexpr imin = std::numeric_limits<int64_t>::min();
  int64_t  constexpr imax = std::numeric_limits<int64_t>::max();
  uint64_t constexpr umin = std::numeric_limits<uint64_t>::min();
  uint64_t constexpr umax = std::numeric_limits<uint64_t>::max();

  uintmax_t uarg = std::strtoumax(argv[2], &end_ptr, 10);

  if (end_ptr == argv[2]) {
    std::cerr << argv[0] << ": cannot parse 'a': " << argv[2] << '\n';
    std::exit(1);
  }

  if (uarg < umin || uarg > umax) {
    std::cerr << argv[0] << ": a must be in [" << umin << ", " << umax <<
      "], got: " << uarg << '\n';
    std::exit(1);
  }

  uint64_t const a = uint64_t(uarg);

  intmax_t const iarg = std::strtoimax(argv[3], &end_ptr, 10);

  if (end_ptr == argv[3]) {
    std::cerr << argv[0] << ": cannot parse 'b': " << argv[3] << '\n';
    std::exit(1);
  }

  if (iarg < imin || iarg > imax) {
    std::cerr << argv[0] << ": b must be in [" << imin << ", " << imax
      << "], got: " << iarg << '\n';
    std::exit(1);
  }

  int64_t const b = int64_t(iarg);

  uarg = std::strtoumax(argv[4], &end_ptr, 10);

  if (end_ptr == argv[4]) {
    std::cerr << argv[0] << ": cannot parse 'd': " << argv[4] << '\n';
    std::exit(1);
  }

  if (uarg < umin || uarg > umax) {
    std::cerr << argv[0] << ": d must be in [" << umin << ", " << umax <<
      "], got: " << uarg << '\n';
    std::exit(1);
  }

  uint64_t const d = uint64_t(uarg);

  eaf_t const eaf{ a, b, d };

  for (int32_t i = 5; i < argc; ++i) {

    uarg = std::strtoumax(argv[i], &end_ptr, 10);

    if (end_ptr == argv[i]) {
      std::cerr << argv[0] << ": cannot parse 'k':" << argv[i] << '\n';
      std::exit(1);
    }

    if (uarg < 1 || uarg > 64) {
      std::cerr << argv[0] << ": k must be in [1, 64] (skipping k = " <<
        uarg << ")\n\n";
      continue;
    }

    uint32_t const k = uint32_t(uarg);

    fast_eaf_t const fast_eaf = get_fast_eaf(rounding, k, eaf);

    std::cout << fast_eaf << '\n';
  }
}
