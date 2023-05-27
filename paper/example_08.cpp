// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file example_08.cpp
 *
 * @brief Command line program that confirms claim of example 8.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "eaf/common.hpp"

#include <cstdint>
#include <iostream>

int main() {

  using namespace eaf;

    std::cout << "Testing:\n  "
        "(153 * M - 457) / 5 == (980 * M - 2928) / 2^5,\n  "
        "for all M in [0, 12[.\n";

    std::cout << '\n';
    bool pass = true;

    std::cout << "Using signed integers...\n";

    for (int32_t M = 0; M < 12; ++M) {

        int32_t const lhs = ::eaf::quotient(153 * M - 457, 5);
        int32_t const rhs = ::eaf::quotient(980 * M - 2928, 32);

        if (lhs != rhs) {
            std::cout << "  Failed for M = " << M << ", "
                "lhs = " << lhs << ", "
                "rhs = " << rhs << '\n';
            pass = false;
        }
    }

    if (pass)
        std::cout << "  Pass.\n";

    std::cout << '\n';
    pass = true;

    std::cout << "Using unsigned integers...\n";

    for (uint32_t M = 0; M < 12; ++M) {

        // Notice that if M <= 2, then 153 * M - 457 <= -151 < 0 and
        // 980 * M - 2928 <= -968 < 0. Therefore, under mod 2^32 arithmetic,
        // the two numerators become large positive numbers. In contrast,
        // Example 8 assumes usual integer arithmetic (as in arithmetic on
        // Z) and therefore its result is not expected to hold here.

        uint32_t const lhs = (153 * M - 457) / 5;
        uint32_t const rhs = (980 * M - 2928) / 32;

        if (lhs != rhs) {
            std::cout << "  Failed for M = " << M << ", "
                "lhs = " << lhs << ", "
                "rhs = " << rhs << '\n';
            pass = false;
        }
    }

    if (pass)
        std::cout << "  Pass.\n";

    return 0;
}
