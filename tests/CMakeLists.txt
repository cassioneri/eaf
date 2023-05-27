# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
# SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

# This code is a supplementary material to:
#
#     Neri C, and Schneider L, "Euclidean Affine Functions and their
#     Application to Calendar Algorithms" (2022).

add_executable(tests
  tests.cpp
  ../algorithms/definitions.cpp
)
target_link_libraries(tests gtest gtest_main)
