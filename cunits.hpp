#ifndef CUNITS_HPP
#define CUNITS_HPP

#include <cassert>
#include <iostream>
#include <list>

// Describes contiguous units [min, max), i.e., min is included, and
// max is not.

template <typename T>
class cunits
{
  using self = cunits<T>;

  T m_min;
  T m_max;

public:
  cunits(): m_min(0), m_max(0)
  {
  }

  cunits(T min, T max): m_min(min), m_max(max)
  {
    assert(min < max);
  }

  const T &
  min() const
  {
    return m_min;
  }

  const T &
  max() const
  {
    return m_max;
  }

  T
  count() const
  {
    return m_max - m_min;
  }

  bool
  operator < (const self &a) const
  {
    return m_min != a.m_min ? m_min < a.m_min : m_max < a.m_max;
  }

  bool
  operator == (const self &a) const
  {
    return m_min == a.m_min && m_max == a.m_max;
  }

  bool
  operator != (const self &a) const
  {
    return !(*this == a);
  }

  bool
  includes(const self &a) const
  {
    return m_min <= a.m_min && a.m_max <= m_max;
  }
};

template <typename T>
std::ostream &
operator << (std::ostream &out, const cunits<T> &cu)
{
  out << '{' << cu.min() << ", " << cu.max() << '}';
  return out;
}

template <typename T>
std::istream &
operator >> (std::istream &in, cunits<T> &cu)
{
  char c;
  T min, max;

  // Get the initial '{'.
  in >> c;
  if (!in.fail())
    {
      assert(c == '{');
      // Get the min value.
      in >> min;
      assert(!in.fail());
      // Get the ','.
      in >> c;
      assert(c == ',');
      assert(!in.fail());
      // Get the max value.
      in >> max;
      assert(!in.fail());
      // Get the trailing '}'.
      in >> c;
      assert(c == '}');
      assert(!in.fail());

      if (in)
        {
          // Make sure min < max.
          assert(min < max);

          cu = cunits<T>(min, max);
        }
    }

  return in;
}

template <typename T>
auto
get_candidate_slots(const cunits<T> &cu, int ncu)
{
  std::list<cunits<T>> result;

  if (ncu <= cu.count())
    for(auto m = cu.min(); m <= cu.max() - ncu; ++m)
      result.push_back(cunits<T>(m, m + ncu));

  return result;
}

#endif // CUNITS_HPP
