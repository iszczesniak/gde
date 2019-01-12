#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "adaptive_units.hpp"
#include "generic_dijkstra.hpp"
#include "generic_label_creator.hpp"
#include "generic_label.hpp"
#include "generic_solution.hpp"
#include "generic_tracer.hpp"
#include "graph.hpp"

#include <chrono>
#include <list>
#include <optional>
#include <utility>

template <typename G>
auto
search(const G &g, CU cu, int src, int dst, int ncu)
{
  assert(ncu > 0);
  assert(src != dst);

  using tp_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
  tp_t t0 = std::chrono::system_clock::now();

  // The maximal length of a path.
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
  // Get the path.
  auto op = trace(S, dst, l, t);

  tp_t t1 = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = t1 - t0;

  using result_t = std::optional<std::pair<CU, std::list<int>>>;

  result_t result;

  if (op)
    {
      std::list<int> p;
      for (const edge &e: op.value().second)
        p.push_back(e2i[e]);
      result = make_pair(op.value().first, p);
    }

  return std::pair<double, result_t>(dt.count(), std::move(result));
}

#endif // SEARCH_HPP
