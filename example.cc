#include "search.hpp"
#include "utils.hpp"

#include <cassert>

using namespace std;

// Type COST is defined in graph.hpp.  Feel free to change it there.

// In this example we request 1 unit, and set the reach of the least
// spectrum-efficient modulation to 8.

int
main()
{
  // Make sure we've got the modulation model right.
  using au = adaptive_units<COST>;
  // Report the length of the longest shortest path.
  au::longest_reach(8);
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
  graph g(4);
  auto [e1, s1] = add_edge(0, 1, g);
  auto [e2, s2] = add_edge(0, 2, g);
  auto [e3, s3] = add_edge(1, 2, g);
  auto [e4, s4] = add_edge(2, 3, g);
  assert(s1 && s2 && s3 && s4);

  // Set the edge lengths.
  boost::get(boost::edge_weight, g, e1) = 2;
  boost::get(boost::edge_weight, g, e2) = 2;
  boost::get(boost::edge_weight, g, e3) = 2;
  boost::get(boost::edge_weight, g, e4) = 2;

  // Set the edge available units.
  boost::get(boost::edge_su, g, e1) = {CU(1, 5)};
  boost::get(boost::edge_su, g, e2) = {CU(0, 3)};
  boost::get(boost::edge_su, g, e3) = {CU(1, 5)};
  boost::get(boost::edge_su, g, e4) = {CU(1, 5)};

  // We search for a path from 0 to 3, with 1 unit for the most
  // efficient modulation.
  auto r = search(g, CU(0, 5), 0, 3, 1);

  cout << "Solution found: ";
  if (r)
    cout << r.value();
  else
    cout << "none";
  cout << endl;
}
