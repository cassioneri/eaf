// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file example_12.cpp
 *
 * @brief Command line program that confirms claim of example 12.
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
        "n % 1461 == 2939745 * n % 2^32 / 2939745,\n  "
        "for all n in [0, 28825529[.\n";

    std::cout << '\n';
    bool pass = true;

    std::cout << "Using signed integers...\n";

    for (int32_t n = 0; n < 28825529; ++n) {

        int32_t const lhs = n % 1461;
        int32_t const rhs = int64_t(2939745) * n % 4294967296 /
            2939745;

        if (lhs != rhs) {
            std::cout << "  Failed for n = " << n << ", "
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

    for (uint32_t n = 0; n < 28825529; ++n) {

        uint32_t const lhs = n % 1461;
        uint32_t const rhs = int64_t(2939745) * n % 4294967296 /
            2939745;

        if (lhs != rhs) {
            std::cout << "  Failed for n = " << n << ", "
                "lhs = " << lhs << ", "
                "rhs = " << rhs << '\n';
            pass = false;
        }
    }

    if (pass)
        std::cout << "  Pass.\n";

    return 0;
}
