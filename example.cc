#include "adaptive_units.hpp"
#include "graph_type.hpp"
#include "search.hpp"
#include "units.hpp"
#include "utils.hpp"

#include <cassert>

using namespace std;

using au = adaptive_units<double>;

// Make sure we've got the modulation model right.
void
units()
{
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
}

// In this example we request 1 unit, and set the reach of the least
// spectrum-efficient modulation to 8.
void
test1()
{
  // The graph, where (n) is node number n, and (c, CU) is link cost c
  // and contiguous units CU:
  //
  // (0)-------------(2, [0, 3))-------------(2)---(2, [1, 5))---(3)
  //  \                                       /
  //   \---(2, [1, 5))---(1)---(2, [1, 5))---/
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
  auto r = search(g, v0, v3, 1, CU(0, 5));
  assert(r);
  auto path = r.value();
}

// The failing example.
void
failing_example()
{
  // The graph, where (n) is node number n, and (c, CU) is link cost c
  // and contiguous units CU:
  //
  //                 (t)
  //                /  |
  //               /   |
  //              /    |
  //   e1, (1, [0, 1)) |
  //            /      |
  //          (s)  e3, (0, [0, 2))
  //            \      |
  //   e2, (1, [0, 2)) |
  //              \    |
  //               \   |
  //                \  |
  //                 (u)
  graph_type g(3);
  auto &s = add_vertex(g, "s");
  auto &t = add_vertex(g, "t");
  auto &u = add_vertex(g, "u");

  add_edge(s, t, 1, SU{{0, 1}});
  add_edge(s, u, 1, SU{{0, 2}});
  add_edge(u, t, 0, SU{{0, 2}});

  auto &e1 = get_edges(s)[0];
  auto &e2 = get_edges(s)[1];
  auto &e3 = get_edges(u)[0];

  // We search for a path from 0 to 3, with 1 unit for the most
  // efficient modulation.
  auto r = search(g, s, t, 1, CU(0, 5));
  assert(r);
  auto path = r.value();
  assert(path.size() == 2);
  assert(get_edge(path[0]) == e2);
  assert(get_edge(path[1]) == e3);
}

int
main()
{
  // Set the maximal reach of the spectrally-worse (m = 1) modulation.
  au::set_reach_1(8);

  units();
  test1();
  failing_example();
}
