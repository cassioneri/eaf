// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file to_date.cpp
 *
 * @brief Command line program that benchmarks implementations of to_date().
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "algorithms/baum.hpp"
#include "algorithms/boost.hpp"
#include "algorithms/dotnet.hpp"
#include "algorithms/fliegel_flandern.hpp"
#include "algorithms/glibc.hpp"
#include "algorithms/hatcher.hpp"
#include "algorithms/libcxx.hpp"
#include "algorithms/neri_schneider.hpp"
#include "algorithms/openjdk.hpp"
#include "algorithms/reingold_dershowitz.hpp"
#include "eaf/date.hpp"

#include <benchmark/benchmark.h>

#include <array>
#include <cstdint>
#include <random>

auto const rata_dies = [](){
  // The interval [-146097, 146097[ covers dates from 1 January 1570
  // (inclusive) to 1 January 2370 (exclusive), that is, an interval of 800
  // years centered at 1 January 1970 (Unix epoch).
  std::uniform_int_distribution<int32_t> uniform_dist(-146097, 146096);
  std::mt19937 rng;
  std::array<int32_t, 16384> ns;
  for (int32_t& n : ns)
    n = uniform_dist(rng);
  return ns;
}();

struct scan {};

template <typename A>
void time(benchmark::State& state);

template <>
void time<scan>(benchmark::State& state) {
  for (auto _ : state)
    for (int32_t rata_die : rata_dies)
      benchmark::DoNotOptimize(rata_die);
}

template <typename A>
void time(benchmark::State& state) {
  for (auto _ : state) {
    for (int32_t rata_die : rata_dies) {
      date32_t date = A::to_date(rata_die);
      benchmark::DoNotOptimize(date);
    }
  }
}

BENCHMARK(time<scan               >);
BENCHMARK(time<baum               >);
BENCHMARK(time<boost              >);
BENCHMARK(time<dotnet             >);
BENCHMARK(time<fliegel_flandern   >);
BENCHMARK(time<glibc              >);
BENCHMARK(time<hatcher            >);
BENCHMARK(time<libcxx             >);
BENCHMARK(time<openjdk            >);
BENCHMARK(time<reingold_dershowitz>);
BENCHMARK(time<neri_schneider     >);
