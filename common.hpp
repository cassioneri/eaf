// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file common.hpp
 *
 * @brief Common utilities.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_COMMON_HPP
#define EAF_COMMON_HPP

#include <cinttypes>
#include <cstdint>
#include <iostream>
#include <limits>
#include <tuple>

namespace eaf {

struct config {
  #if !defined(EAF_SIZE) || EAF_SIZE == 32
    using type = int32_t;
  #elif EAF_SIZE == 64
    using type = int64_t;
  #else
    #error "Invalid EAF_SIZE."
  #endif
};

using config_type = typename config::type;

/**
 * @brief Date type.
 *
 * @tparam T        Year type.
 */
template <typename T>
struct date_t {
  T        year;
  uint32_t month;
  uint32_t day;
};

/**
 * @brief Equality operator for date_t.
 *
 * @tparam T        Year type.
 *
 * @param date_1    The 1st date.
 * @param date_2    The 2nd date.
 *
 * @return true, if date_1 is the same as date_2, and false, otherwise.
 */
template <typename T>
bool constexpr
operator ==(date_t<T> const date_1, date_t<T> const date_2) noexcept {
  return std::tie(date_1.year, date_1.month, date_1.day) ==
    std::tie(date_2.year, date_2.month, date_2.day);
}

/**
 * @brief Less-than operator for date_t.
 *
 * @tparam T        Year type.
 *
 * @param date_1    The 1st date.
 * @param date_2    The 2nd date.
 *
 * @return true, if date_1 is before date_2, and false, otherwise.
 */
template <typename T>
bool constexpr
operator <(date_t<T> const date_1, date_t<T> const date_2) noexcept {
  return std::tie(date_1.year, date_1.month, date_1.day) <
    std::tie(date_2.year, date_2.month, date_2.day);
}

/**
 * @brief Output stream operator for date_t.
 *
 * @tparam T        Year type.
 *
 * @param os        The output stream.
 * @param eaf       The date_t object.
 *
 * @return os.
 */
template <typename T>
std::ostream&
operator <<(std::ostream& os, date_t<T> const date) {
  return os << date.year << ' ' << date.month << ' ' << date.day;
}

/**
 * @brief Calculates the quotient of Euclidean division.
 *
 * @tparam T        Dividend type.
 *
 * @param n         The dividend.
 * @param d         The divisor.
 *
 * @pre d > 0
 *
 * @return The quotient of Euclidean division.
 */
template <typename T>
T constexpr
quotient(T const n, int32_t const d) noexcept {
  return n >= 0 ? n / d : (n - (d - 1)) / d;
}

/**
 * @brief Calculates the remainder of Euclidean division.
 *
 * @tparam T        Dividend type.
 *
 * @param n         The dividend.
 * @param d         The divisor.
 *
 * @pre d > 0
 *
 * @return The remainder of Euclidean division.
 */
template <typename T>
uint32_t constexpr
remainder(T const n, int32_t const d) noexcept {
  return uint32_t(n >= 0 ? n % d : n - d * quotient(n, d));
}

/**
 * @brief Command line parser.
 *
 * @tparam T        Year and rata die type.
 */
template <typename T>
struct parser {

  static T
  rata_die(int argc, char* argv[]) {

    if (argc < 2) {
      std::cerr << argv[0] << ": requires a rata die.\n";
      exit (1);
    }

    char* end_ptr;

    intmax_t const N = std::strtoimax(argv[1], &end_ptr, 10);

    if (end_ptr == argv[1]) {
      std::cerr << argv[0] << ": cannot parse rata die: " << argv[1] <<
        '\n';
      std::exit(1);
    }

    T constexpr min = std::numeric_limits<T>::min();
    T constexpr max = std::numeric_limits<T>::max();

    if (N < min || max < N) {
      std::cerr << argv[0] << ": rata die " << N << " not in [ " << min <<
        ", " << max << "]\n";
      std::exit(1);
    }

    return T(N);
  }

  static date_t<T>
  date(int argc, char* argv[]) {

    if (argc != 4) {
      std::cerr << argv[0] << ": requires 3 arguments: year month day.\n";
      exit (1);
    }

    char* end_ptr;

    intmax_t const year = std::strtoimax(argv[1], &end_ptr, 10);
    if (end_ptr == argv[1]) {
      std::cerr << argv[0] << ": cannot parse year: " << argv[1] << '\n';
      std::exit(1);
    }

    T constexpr min = std::numeric_limits<T>::min();
    T constexpr max = std::numeric_limits<T>::max();

    if (year < min || max < year) {
      std::cerr << argv[0] << ": year " << year << " not in [ " << min <<
        ", " << max << "]\n";
      std::exit(1);
    }

    intmax_t const month = std::strtoimax(argv[2], &end_ptr, 10);
    if (end_ptr == argv[2]) {
      std::cerr << argv[0] << ": cannot parse month: " << argv[2] << '\n';
      std::exit(1);
    }

    if (month < 1 || month > 12) {
      std::cerr << argv[0] << ": month not in [1, 12]\n";
      std::exit(1);
    }

    intmax_t const day = std::strtoimax(argv[3], &end_ptr, 10);
    if (end_ptr == argv[3]) {
      std::cerr << argv[0] << ": cannot parse day: " << argv[2] << '\n';
      std::exit(1);
    }

    if (day < 1 || day > 31) {
      std::cerr << argv[0] << ": day not in [1, 31]\n";
      std::exit(1);
    }

    return { T(year), uint32_t(month), uint32_t(day) };
  }
};

/**
 * @brief Prints given rata die and date.
 *
 * @tparam T        Year and rata die type.
 *
 * @param N         The rata die.
 * @param data      The date.
 */
template <typename T>
void
print(T const N, date_t<T> const date) {
  std::cout <<
    "rata die = " << N    << '\n' <<
    "date     = " << date << '\n';
}

} // namespace eaf

#endif // EAF_COMMON_HPP
