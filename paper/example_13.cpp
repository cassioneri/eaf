// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file example_13.cpp
 *
 * @brief Command line program that confirms claim of example 13.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include <cstdint>
#include <iostream>

int main() {

    std::cout << "Testing:\n  "
        "(5 * N_Y + 461) % 153 / 5 == (2141 * N_Y + 197913) % 2^16 / 2141,"
        "\n  for all N_Y in [0, 734[.\n";

    std::cout << '\n';
    bool pass = true;

    std::cout << "Using signed integers...\n";

    for (int32_t N_Y = 0; N_Y < 734; ++N_Y) {

        int32_t const lhs = (5 * N_Y + 461) % 153 / 5;
        int32_t const rhs = (2141 * N_Y + 197913) % 65536 / 2141;

        if (lhs != rhs) {
            std::cout << "  Failed for N_Y = " << N_Y << ", "
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

    for (uint32_t N_Y = 0; N_Y < 734; ++N_Y) {

        uint32_t const lhs = (5 * N_Y + 461) % 153 / 5;
        uint32_t const rhs = (2141 * N_Y + 197913) % 65536 / 2141;

        if (lhs != rhs) {
            std::cout << "  Failed for N_Y = " << N_Y << ", "
                "lhs = " << lhs << ", "
                "rhs = " << rhs << '\n';
            pass = false;
        }
    }

    if (pass)
        std::cout << "  Pass.\n";

    return 0;
}
