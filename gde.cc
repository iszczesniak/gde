#include "adaptive_units.hpp"
#include "generic_path_range.hpp"
#include "graph_type.hpp"
#include "search.hpp"
#include "units.hpp"

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
  // In link label (c, CU), c is cost, and CU is contiguous units.
  //
  // (a)-----------e1, (2, [0, 3))------------(b)-e3, (2, [1, 5))-(c)
  //  \                                       /
  //   \-e2, (2, [1, 5))-(d)-e4, (2, [1, 5))-/

  graph_type g(4);
  auto &a = add_vertex(g, "a");
  auto &b = add_vertex(g, "b");
  auto &c = add_vertex(g, "c");
  auto &d = add_vertex(g, "d");

  add_edge(a, b, 2, SU{{0, 3}});
  add_edge(a, d, 2, SU{{1, 5}});
  add_edge(b, c, 2, SU{{1, 5}});
  add_edge(d, b, 2, SU{{1, 5}});

  auto &e1 = get_edges(a)[0];
  auto &e2 = get_edges(a)[1];
  auto &e3 = get_edges(b)[0];
  auto &e4 = get_edges(d)[0];

  // We search for a path from 0 to 3, with 1 unit for the most
  // efficient modulation.
  auto r = search(g, a, c, 1, CU(0, 5));
  assert(r);
  auto path = r.value();
  assert(path.size() == 3);
  assert(get_edge(path.front()) == e2);
  assert(get_edge(*++path.begin()) == e4);
  assert(get_edge(*++++path.begin()) == e3);
}

// The failing example.
void
failing_example()
{
  // In link label (c, CU), c is cost, and CU is contiguous units.
  //
  //               (t)
  //              /  |
  //             /   |
  //            /    |
  // e1, (1, [0, 1)) |
  //          /      |
  //        (s)  e3, (0, [0, 2))
  //          \      |
  // e2, (1, [0, 2)) |
  //            \    |
  //             \   |
  //              \  |
  //               (u)

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
  assert(get_edge(path.front()) == e2);
  assert(get_edge(*++path.begin()) == e3);
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
