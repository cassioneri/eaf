// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file algorithm_02.cpp
 *
 * @brief Command line program that runs algorithm 2.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "eaf/common.hpp"
#include "eaf/julian.hpp"

#include <cstdint>
#include <iostream>

int main(int argc, char* argv[]) {

  using namespace eaf;
  using rata_die_type = config_type;
  using date_type     = date_t<config_type>;

  date_type     const date = parser<rata_die_type>::date(argc, argv);
  rata_die_type const N    = julian::to_rata_die(date.year, date.month,
    date.day);

  print(N, date);

  return 0;
}
