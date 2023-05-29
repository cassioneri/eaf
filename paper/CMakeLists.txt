# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
# SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

# This code is a supplementary material to:
#
#     Neri C, and Schneider L, "Euclidean Affine Functions and their
#     Application to Calendar Algorithms" (2022).

#---------------------------------------------------------------------------
# EAF
#---------------------------------------------------------------------------

foreach(ALGORITHM 01;02;03;04;05;06)
  foreach(SIZE 32;64)

    add_executable(algorithm_${ALGORITHM}_${SIZE}
      algorithm_${ALGORITHM}.cpp
    )

    target_compile_definitions(algorithm_${ALGORITHM}_${SIZE} PUBLIC
      EAF_SIZE=${SIZE}
    )

  endforeach()
endforeach()

foreach(EXAMPLE 08;09;10;11;12;13;14;15;16)
  add_executable(example_${EXAMPLE} example_${EXAMPLE}.cpp)
endforeach()

add_executable(fast_eaf fast_eaf.cpp)
target_link_libraries(fast_eaf boost_multiprecision)

add_executable(info info.cpp)
