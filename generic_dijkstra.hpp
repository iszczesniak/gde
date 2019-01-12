#ifndef GENERAL_DIJKSTRA_HPP
#define GENERAL_DIJKSTRA_HPP

#include "dijkstra.hpp"
#include "generic_solution.hpp"

#include <utility>

// Move the best label from Q to L, and return a reference to the
// label in the new place.
template <typename Graph, typename Cost, typename Units>
auto
move_label(generic_solution<Graph, Cost, Units> &S,
           generic_solution<Graph, Cost, Units> &Q)
{
  auto p = S.insert(Q.pop());

  // Make sure we moved the label.
  assert(p.inserted);

  // That's the label.
  const auto &l = *(p.position);

  return l;
}

/**
 * Is there in C a label that is better than or equal to label j?
 */
template <typename Graph, typename Cost, typename Units>
bool
has_better_or_equal(const generic_solution<Graph, Cost, Units> &C,
		    const generic_label<Graph, Cost, Units> &j)
{
  if (const auto iter = C.find(get_target(j)); iter != C.end())
    {
      // We could go for the easy implementation where we iterate for
      // each label i, and compare it to label j.  But we take
      // advantage of the fact that the elements in the set are sorted
      // by cost first.
      for (auto const &i: iter->second)
        {
          // Stop searching when we reach a label with a higher cost.
          // If the cost of label i is higher than the cost of label
          // j, then label i (and the labels in the set that follow)
          // cannot be better or equal (they can be incomparable or
          // worse).
          if (get_cost(i) > get_cost(j))
            break;

          // Is label i better than or equal to label j?
          if (i <= j)
            return true;
        }
    }

  return false;
}

/**
 * Purge from queue Q those labels which are worse than label j.
 */
template <typename Graph, typename Cost, typename Units>
void
purge_worse(generic_solution<Graph, Cost, Units> &Q,
	    const generic_label<Graph, Cost, Units> &j)
{
  if (const auto iter = Q.find(get_target(j)); iter != Q.end())
    {
      auto &Qt = iter->second;

      // We could go for the easy implementation where we iterate for
      // each label i and compare it to j.  But we take advantage of
      // the fact that the elements in the set are sorted by cost
      // first.  We iterate in the reverse order!
      for(auto r = Qt.rbegin(); r != Qt.rend();)
        {
          const auto &i = *r;

          // Stop searching when we reach label i with the cost lower
          // than the cost of label j.  If the cost of label i is
          // lower than the cost of label j, then label i (and the
          // labels in the set that follow) cannot be worse (they can
          // be better or incomparable).
          if (get_cost(i) < get_cost(j))
            break;

          // Make sure labels i and j are not equal.  We can make this
          // assertion here, because we are not inserting equal labels
          // into the priority queue.  We need this assertion here, so
          // that we can sefely use the <= operator below.
          assert(!(get_cost(i) == get_cost(j) &&
                   get_units(i) == get_units(j)));

          // To check whether label i is worse then j, we use the <=
          // operator, because we made sure the labels are not equal.
          if (j <= i)
            // We want to remove label i, and we're going to use
            // iterator r.  We can safely remove the element pointed
            // to by r, because the base iterator points to the
            // element next to r.  This erasure does not invalidate
            // the base iterator of r.  Note that we do not increment
            // r, because after the erasure, r will already point to
            // the next element.
            Qt.erase(--(r.base()));
          else
            ++r;
        }
    }
}

#endif // GENERAL_DIJKSTRA_HPP
