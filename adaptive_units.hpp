#ifndef ADAPTIVE_UNITS_HPP
#define ADAPTIVE_UNITS_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <set>

template <typename Weight>
struct adaptive_units
{
  // The maximal modulation level.
  static constexpr int m_M = 4;

  // The distance divisor.
  static constexpr int m_dist_div = 1 << m_M;

  // The reach of the m = 1 modulation level.
  static Weight m_reach_1;

  // The reach of the m = M modulation level.
  static Weight m_reach_M;

  // The base_dist, needed to speed up our calculations.
  static Weight m_base_dist;

  // Report the reach for modulation with level m = 1.
  static void
  set_reach_1(Weight length)
  {
    m_reach_1 = length;
    m_base_dist = m_reach_1 / m_dist_div;
    m_reach_M = 2 * m_base_dist;
  }

  // The required number of units at weight dist, when the required
  // number is ncu_M for modulation level m = M.
  static int
  units(int ncu_M, Weight dist)
  {
    // Make sure that set_reach_1 was called.
    assert(m_reach_1 != 0);
    assert(dist >= 0);

    if (dist <= m_reach_M)
      return ncu_M;

    // If the distance is above the threshold, return the highest
    // required number of units so that there is no path.
    if (m_reach_1 < dist)
      return std::numeric_limits<int>::max();

    return std::ceil(ncu_M * std::log2(dist / m_base_dist));
  }

  // Returns the reach of the modulation that uses ncu units, while
  // the modulation level m = M uses ncu_M units.
  static
  Weight
  reach(int ncu_M, int ncu)
  {
    assert(("Please call set_reach_1.", m_base_dist != 0));
    double m = static_cast<double>(ncu) / ncu_M;
    return m_base_dist * std::pow(2.0, m);
  }

  // Produces a list of ncu's for all modulations.
  static std::set<int>
  ncus(int ncu_M)
  {
    std::set<int> s;

    for (int units = ncu_M; units <= m_M * ncu_M; ++units)
      s.insert(units);

    return s;
  }
};

template <typename Weight>
Weight adaptive_units<Weight>::m_reach_1;

template <typename Weight>
Weight adaptive_units<Weight>::m_reach_M;

template <typename Weight>
Weight adaptive_units<Weight>::m_base_dist;

#endif // ADAPTIVE_UNITS_HPP
