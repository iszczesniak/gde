#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "dijkstra.hpp"
#include "generic_label.hpp"
#include "generic_path_range.hpp"
#include "generic_permanent.hpp"
#include "generic_tentative.hpp"
#include "graph_type.hpp"
#include "label_creator.hpp"
#include "label_robe.hpp"
#include "units.hpp"

template <typename Graph, typename Vertex>
auto
search(const Graph &g, const Vertex &src, const Vertex &dst,
       unsigned ncu, CU cu)
{
  assert(ncu > 0);

  // The null edge that is not part of the graph.
  edge_type null_edge(src, src, 0, SU{});
  // The label type.
  using robe_type = label_robe<edge_type,
                               generic_label<unsigned, CU>>;
  // The initial label.
  robe_type init(null_edge, 0, cu);

  // The permanent and tentative solutions.
  generic_permanent<robe_type> P(num_vertexes(g));
  generic_tentative<robe_type> T(num_vertexes(g));
  // Label creator.
  label_creator<double> f(ncu);
  // Run the search.
  dijkstra(init, P, T, f, dst);

  // The type of the path range.
  using range_type = generic_path_range<decltype(P), decltype(f)>;
  // Return the path labels.
  return trace<range_type>(P, f, dst, init);
}

#endif // SEARCH_HPP
