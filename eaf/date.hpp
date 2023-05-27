// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file date.hpp
 *
 * @brief Definition of the date32_t struct.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_EAF_DATE_HPP
#define EAF_EAF_DATE_HPP

#include "eaf/common.hpp"

#include <cstdint>

using date32_t = ::eaf::date_t<int32_t>;
using date64_t = ::eaf::date_t<int64_t>;

#endif // EAF_EAF_DATE_HPP
