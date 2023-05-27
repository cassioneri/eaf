// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file example_15.cpp
 *
 * @brief Command line program that confirms claims of example 15.
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
        "n % 3600 == 1193047 * n % 2^32 / 1193047,"
        "\n  for all n in [0, 2257199[.\n";

    std::cout << '\n';
    bool pass = true;

    for (uint32_t n = 0; n < 2257199; ++n) {

        uint32_t const lhs = n % 3600;
        uint32_t const rhs = uint32_t(uint64_t(1193047) * n % 4294967296) /
          1193047;

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

    std::cout << "Testing:\n  "
        "n % 60 == 71582789 * n % 2^32 / 71582789,"
        "\n  for all n in [0, 97612919[.\n";

    std::cout << '\n';
    pass = true;

    for (uint32_t n = 0; n < 9761291; ++n) {

        uint32_t const lhs = n % 60;
        uint32_t const rhs = uint32_t(uint64_t(71582789) * n % 4294967296) /
            71582789;

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

    std::cout << "Testing:\n  "
        "n % 10 == 429496730 * n % 2^32 / 429496730,"
        "\n  for all n in [0, 1073741829[.\n";

    std::cout << '\n';
    pass = true;

    for (uint32_t n = 0; n < 1073741829; ++n) {

        uint32_t const lhs = n % 10;
        uint32_t const rhs = uint32_t(uint64_t(429496730) * n % 4294967296)
            / 429496730;

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

    return 0;
}
