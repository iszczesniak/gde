#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "dijkstra.hpp"
#include "generic_label.hpp"
#include "generic_permanent.hpp"
#include "generic_tentative.hpp"
#include "generic_tracer.hpp"
#include "graph_type.hpp"
#include "label_creator.hpp"
#include "label_robe.hpp"
#include "units.hpp"

template <typename Graph, typename Vertex>
auto
search(const Graph &g, CU cu, unsigned ncu,
       const Vertex &src, const Vertex &dst)
{
  assert(ncu > 0);

  // The null edge that is not part of the graph.
  edge_type null_edge(src, src, 0, SU{});
  // The label type, and the initial label.
  using robe_type = label_robe<edge_type,
                               generic_label<unsigned, CU>>;
  robe_type initial(null_edge, 0, cu);

  // The permanent and tentative solutions.
  generic_permanent<robe_type> P(num_vertexes(g));
  generic_tentative<robe_type> T(num_vertexes(g));
  // Run the search.
  dijkstra(initial, P, T, label_creator(ncu));
  // Get and return the path.
  return trace(initial, dst, generic_tracer(P));
}

#endif // SEARCH_HPP
