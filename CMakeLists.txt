# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
# SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

# This code is a supplementary material to:
#
#     Neri C, and Schneider L, "Euclidean Affine Functions and their
#     Application to Calendar Algorithms" (2022).

cmake_minimum_required(VERSION 3.5)

project(EAF VERSION 1.0
  DESCRIPTION "Supplement material to \
  Neri C, and Schneider L, \"Euclidean Affine Functions and Applications \
  to Calendar Algorithms\" (2022)."
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)

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
  GIT_TAG         v1.7.1
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
  GIT_TAG        Boost_1_82_0
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

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(algorithms)
add_subdirectory(benchmarks)
add_subdirectory(paper)
add_subdirectory(tests)
