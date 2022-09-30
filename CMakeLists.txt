# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
# SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

# This code is a supplementary material to:
#
#     Neri C, and Schneider L, "Euclidean Affine Functions and their
#     Application to Calendar Algorithms" (2022).

cmake_minimum_required(VERSION 3.1)

project(EAF VERSION 1.0
  DESCRIPTION "Supplement material to \
  Neri C, and Schneider L, \"Euclidean Affine Functions and Applications \
  to Calendar Algorithms\" (2022)."
  LANGUAGES CXX
)

set_property(GLOBAL PROPERTY CXX_STANDARD 11)
if (MSVC)
  string(REGEX REPLACE "/W[0-4]" "/W4 /WX" CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS}")
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include(FetchContent)

#---------------------------------------------------------------------------
# Benchmark
#---------------------------------------------------------------------------

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  # Tag v1.7.0 contains issue 1466 [1] which was fixed by commit 77d1e74.
  # [1] https://github.com/google/benchmark/issues/1466
  GIT_TAG        77d1e74d29d4ee309b1c08eaea40ad789f08d125
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/tp/benchmark
)

set(BENCHMARK_ENABLE_TESTING off)
FetchContent_MakeAvailable(benchmark)
target_compile_definitions(benchmark PUBLIC -DBENCHMARK_STATIC_DEFINE)

#---------------------------------------------------------------------------
# Boost Multiprecision
#---------------------------------------------------------------------------

FetchContent_Declare(
  multiprecision
  GIT_REPOSITORY https://github.com/boostorg/multiprecision.git
  GIT_TAG        Boost_1_80_0
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/tp/multiprecision
)

set(BOOST_MP_STANDALONE on)
set(BUILD_TESTING off)
FetchContent_MakeAvailable(multiprecision)

#---------------------------------------------------------------------------
# Google Test
#---------------------------------------------------------------------------

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/tp/googletest
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

add_executable(to_date
  benchmark/to_date.cpp
  benchmark/definitions.cpp
)
target_link_libraries(to_date benchmark benchmark_main)

add_executable(to_rata_die
  benchmark/to_rata_die.cpp
  benchmark/definitions.cpp
)
target_link_libraries(to_rata_die benchmark benchmark_main)

add_executable(tests
  benchmark/tests.cpp
  benchmark/definitions.cpp
)
target_link_libraries(tests gtest gtest_main)
