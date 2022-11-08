#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "adaptive_units.hpp"
#include "dijkstra.hpp"
#include "generic_label_creator.hpp"
#include "generic_label.hpp"
#include "generic_permanent.hpp"
#include "generic_tentative.hpp"
#include "generic_tracer.hpp"
#include "graph.hpp"
#include "units.hpp"

#include <optional>

template <typename Graph>
auto
search(const Graph &g, CU cu, int ncu,
       const Vertex<Graph> &src, const Vertex<Graph> &dst)
{
  assert(ncu > 0);
  assert(src != dst);

  using weight_type = Weight<Edge<Graph>>;
  using label_type = generic_label<weight_type, CU>;

  // The permanent and tentative solutions.
  generic_permanent<weight_type, CU> P(num_vertexes(g));
  generic_tentative<weight_type, CU> T(num_vertexes(g));

  // That's a null edge that is not a part of the graph.
  Edge<Graph> null_edge(src, src);

  // The initial label we start the search with.
  label l(0, std::move(cu), null_edge);
  // The creator of the labels.
  generic_label_creator<graph, COST, CU> c(g, ncu);
  // Run the search.
  dijkstra(g, S, Q, l, c, dst);
  // The tracer.
  generic_tracer<graph, cupath, sol_type, CU> t(g, ncu);
  // Get and return the path.
  return trace(S, dst, l, t);
}

#endif // SEARCH_HPP
