#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <list>
#include <utility>

/**
 * This is the << operator for a list.
 */
template <typename T>
std::ostream &operator << (std::ostream &os, const std::list<T> &v)
{
  typename std::list<T>::const_iterator i = v.begin();

  os << "list(";

  while(i != v.end())
    {
      os << *i;
      if (++i != v.end())
        os << ", ";
    }

  os << ")";

  return os;
}

/**
 * This is the << operator for a pair.
 */
template <typename A, typename B>
std::ostream &operator << (std::ostream &os, const std::pair<A, B> &p)
{
  os << "pair(" << p.first << ", " << p.second << ")";
  return os;
}

#endif /* UTILS_HPP */
