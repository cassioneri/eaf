// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file definitions.cpp
 *
 * @brief Symbol definitions for the linker to find.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "dotnet.hpp"
#include "glibc.hpp"

int constexpr dotnet::s_daysToMonth365[];
int constexpr dotnet::s_daysToMonth366[];

unsigned short int constexpr glibc::undesrcore_mon_yday[2][13];
