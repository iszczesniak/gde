#include "adaptive_units.hpp"
#include "graph.hpp"
//#include "search.hpp"
#include "units.hpp"
#include "utils.hpp"

#include <cassert>

using namespace std;

template <typename Edge>
using vertex_type_tmp = vertex<Edge, index<unsigned>,
                               name<std::string>>;

using edge_type = edge<vertex_type_tmp, weight<unsigned>,
                       resources<SU>>;
using vertex_type = vertex_type_tmp<edge_type>;
using graph_type = graph<vertex_type>;

// In this example we request 1 unit, and set the reach of the least
// spectrum-efficient modulation to 8.

int
main()
{
  // Make sure we've got the modulation model right.
  using au = adaptive_units<unsigned>;
  // Set the maximal reach of the spectrally-worse (m = 1) modulation.
  au::set_reach_1(8);
  assert(au::units(1, 0) == 1);
  assert(au::units(1, 1) == 1);
  assert(au::units(1, 2) == 2);
  assert(au::units(1, 3) == 3);
  assert(au::units(1, 4) == 3);
  assert(au::units(1, 5) == 4);
  assert(au::units(1, 6) == 4);
  assert(au::units(1, 7) == 4);
  assert(au::units(1, 8) == 4);
  assert(au::units(1, 9) == std::numeric_limits<int>::max());

  // The graph, where (n) is node number n, and [c, CU] is link cost c
  // and contiguous units CU:
  //
  // (0)-------------[2, (0, 3)]-------------(2)---[2, (1, 5)]---(3)
  //  \                                       /
  //   \---[2, (1, 5)]---(1)---[2, (1, 5)]---/
  //
  graph_type g(4);
  auto &v0 = add_vertex(g, "v0");
  auto &v1 = add_vertex(g, "v1");
  auto &v2 = add_vertex(g, "v2");
  auto &v3 = add_vertex(g, "v3");

  add_edge(v0, v1, 2, SU{{1, 5}});
  add_edge(v0, v2, 2, SU{{0, 3}});
  add_edge(v1, v2, 2, SU{{1, 5}});
  add_edge(v2, v3, 2, SU{{1, 5}});

  // We search for a path from 0 to 3, with 1 unit for the most
  // efficient modulation.

  // auto r = search(g, CU(0, 5), 0, 3, 1);

  // cout << "Solution found: ";
  // if (r)
  //   cout << r.value();
  // else
  //   cout << "none";
  // cout << endl;
}
