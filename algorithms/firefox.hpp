// SPDX-License-Identifier: MPL-2.0
// SPDX-SnippetCopyrightText: The Mozilla Foundation

/**
 * @brief Algorithms on the Gregorian calendar from firefox [1].
 *
 * This code is a supplement material to [2].
 *
 *     [1] https://hg.mozilla.org/mozilla-central/
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_ALGORITHMS_FIREFOX_HPP
#define EAF_ALGORITHMS_FIREFOX_HPP

#include "eaf/date.hpp"

#include <cmath>
#include <cstdint>

struct firefox {

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/vm/DateTime.h#l25
  /* Constants defined by ES5 15.9.1.10. */
  static constexpr double HoursPerDay = 24;
  static constexpr double MinutesPerHour = 60;
  static constexpr double SecondsPerMinute = 60;
  static constexpr double msPerSecond = 1000;
  static constexpr double msPerMinute = msPerSecond * SecondsPerMinute;
  static constexpr double msPerHour = msPerMinute * MinutesPerHour;

  /* ES5 15.9.1.2. */
  static constexpr double msPerDay = msPerHour * HoursPerDay;

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l178
  static inline double Day(double t) { return floor(t / msPerDay); }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l182
  /* ES5 15.9.1.3. */
  static inline bool IsLeapYear(double year) {
    // <note> Not relevant.
    // MOZ_ASSERT(ToInteger(year) == year);
    // </note>
    return fmod(year, 4) == 0 && (fmod(year, 100) != 0 || fmod(year, 400) == 0);
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l188
  static inline double DaysInYear(double year) {
    // <note> Not relevant.
    // if (!IsFinite(year)) {
    //   return GenericNaN();
    // }
    // </note>
    return IsLeapYear(year) ? 366 : 365;
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l195
  static inline double DayFromYear(double y) {
    return 365 * (y - 1970) + floor((y - 1969) / 4.0) -
          floor((y - 1901) / 100.0) + floor((y - 1601) / 400.0);
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l200
  static inline double TimeFromYear(double y) {
    return DayFromYear(y) * msPerDay;
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l204
  static double YearFromTime(double t) {
    // <note> Not relevant.
    // if (!IsFinite(t)) {
    //   return GenericNaN();
    // }
    //
    // MOZ_ASSERT(ToInteger(t) == t);
    // </note>

    double y = floor(t / (msPerDay * 365.2425)) + 1970;
    double t2 = TimeFromYear(y);

    /*
    * Adjust the year if the approximation was wrong.  Since the year was
    * computed using the average number of ms per year, it will usually
    * be wrong for dates within several hours of a year transition.
    */
    if (t2 > t) {
      y--;
    } else {
      if (t2 + msPerDay * DaysInYear(y) <= t) {
        y++;
      }
    }
    return y;
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l229
  static inline int DaysInFebruary(double year) {
    return IsLeapYear(year) ? 29 : 28;
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l233
  /* ES5 15.9.1.4. */
  static inline double DayWithinYear(double t, double year) {
    // <note> (CN): Not relevant.
    // MOZ_ASSERT_IF(IsFinite(t), YearFromTime(t) == year);
    // </note>
    return Day(t) - DayFromYear(year);
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l239
  static double MonthFromTime(double t) {
    // <note> (CN): Not relevant.
    // if (!IsFinite(t)) {
    //   return GenericNaN();
    // }
    // </note>

    double year = YearFromTime(t);
    double d = DayWithinYear(t, year);

    int step;
    if (d < (step = 31)) {
      return 0;
    }
    if (d < (step += DaysInFebruary(year))) {
      return 1;
    }
    if (d < (step += 31)) {
      return 2;
    }
    if (d < (step += 30)) {
      return 3;
    }
    if (d < (step += 31)) {
      return 4;
    }
    if (d < (step += 30)) {
      return 5;
    }
    if (d < (step += 31)) {
      return 6;
    }
    if (d < (step += 31)) {
      return 7;
    }
    if (d < (step += 30)) {
      return 8;
    }
    if (d < (step += 31)) {
      return 9;
    }
    if (d < (step += 30)) {
      return 10;
    }
    return 11;
  }

  // https://hg.mozilla.org/mozilla-central/file/f2178beb09db4f3548827808718f1ea1f2892fc5/js/src/jsdate.cpp#l284
  /* ES5 15.9.1.5. */
  static double DateFromTime(double t) {
    // <note> (CN): Not relevant.
    // if (!IsFinite(t)) {
    //   return GenericNaN();
    // }
    // </note>

    double year = YearFromTime(t);
    double d = DayWithinYear(t, year);

    int next;
    if (d <= (next = 30)) {
      return d + 1;
    }
    int step = next;
    if (d <= (next += DaysInFebruary(year))) {
      return d - step;
    }
    step = next;
    if (d <= (next += 31)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 30)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 31)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 30)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 31)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 31)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 30)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 31)) {
      return d - step;
    }
    step = next;
    if (d <= (next += 30)) {
      return d - step;
    }
    step = next;
    return d - step;
  }

  static inline
  date32_t to_date(int32_t n) {
    double const time = n * msPerDay;
    double const y = YearFromTime(time);
    double const m = MonthFromTime(time);
    double const d = DateFromTime(time);
    return { int32_t(y), uint32_t(m) + 1, uint32_t(d) };
  }

  static inline
  int32_t to_rata_die(int32_t /*year*/, uint32_t /*month*/, uint32_t /*day*/) {
    return 0;
  }

}; // struct firefox

#endif // EAF_ALGORITHMS_FIREFOX_HPP
