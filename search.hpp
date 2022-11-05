#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "adaptive_units.hpp"
#include "dijkstra.hpp"
#include "generic_label_creator.hpp"
#include "generic_label.hpp"
#include "generic_solution.hpp"
#include "generic_tracer.hpp"
#include "graph.hpp"
#include "units.hpp"

#include <optional>

template <typename G>
auto
search(const G &g, CU cu, int src, int dst, int ncu)
{
  assert(ncu > 0);
  assert(src != dst);

  // The maximal length of a path.  We don't set it, so it's
  // unlimited.
  static std::optional<COST> ml;

  // The solution type.
  typedef generic_solution<graph, COST, CU> sol_type;

  // The permanent and tentative solutions.
  sol_type S, Q;
  // The label we start the search with.
  generic_label<graph, COST, CU> l(0, std::move(cu), edge(), src);
  // The creator of the labels.
  generic_label_creator<graph, COST, CU> c(g, ncu, ml);
  // Run the search.
  dijkstra(g, S, Q, l, c, dst);
  // The tracer.
  generic_tracer<graph, cupath, sol_type, CU> t(g, ncu);
  // Get and return the path.
  return trace(S, dst, l, t);
}

#endif // SEARCH_HPP
