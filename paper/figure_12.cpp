// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file algorithm_03.cpp
 *
 * @brief Command line program that runs algorithm of figure 12.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "eaf/common.hpp"
#include "eaf/gregorian.hpp"

#include <cstdint>
#include <iostream>

int main(int argc, char* argv[]) {

  using namespace eaf;
  using rata_die_type = config_type;
  using date_type     = date_t<config_type>;

  rata_die_type const N    = parser<rata_die_type>::rata_die(argc, argv);
  date_type     const date = gregorian::to_date_opt
    <rata_die_type, 719468, 82>(N);

  print(N, date);

  return 0;
}
