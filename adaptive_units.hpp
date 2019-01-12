#ifndef ADAPTIVE_UNITS_HPP
#define ADAPTIVE_UNITS_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <set>

template <typename Cost>
struct adaptive_units
{
  // The maximal modulation level.
  static constexpr int m_m_max = 4;

  // The distance divisor.
  static constexpr int m_dist_div = 1 << m_m_max;

  // The maximal reach of the m_m_max modulation level.
  static Cost m_max_dist;

  // The maximal reach of the first modulation level.
  static Cost m_min_dist;

  // The base_dist.
  static Cost m_base_dist;

  // Report the longest shortest path length.
  static void
  longest_reach(Cost length)
  {
    m_max_dist = length;
    m_base_dist = m_max_dist / m_dist_div;
    m_min_dist = 2 * m_base_dist;
  }

  // The required number of units at cost dist, when at cost 0 the
  // required number is min_units.
  static int
  units(int min_units, Cost dist)
  {
    assert(dist >= 0);

    if (dist <= m_min_dist)
      return min_units;

    // If the distance is above the threshold, return the highest
    // required number of units so that there is no path.
    if (m_max_dist < dist)
      return std::numeric_limits<int>::max();

    return std::ceil(min_units * std::log2(dist / m_base_dist));
  }

  static
  Cost
  reach(int min_units, int units)
  {
    double m = static_cast<double>(units) / min_units;
    return m_base_dist * std::pow(2.0, m);
  }

  static std::set<int>
  ncus(int min_units)
  {
    std::set<int> s;

    for (int units = min_units; units <= m_m_max * min_units; ++units)
      s.insert(units);

    return s;
  }
};

template <typename Cost>
Cost adaptive_units<Cost>::m_max_dist;

template <typename Cost>
Cost adaptive_units<Cost>::m_min_dist;

template <typename Cost>
Cost adaptive_units<Cost>::m_base_dist;

#endif // ADAPTIVE_UNITS_HPP
