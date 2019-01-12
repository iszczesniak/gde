#include "interface.hpp"

#include <cassert>

using namespace std;

// Type COST is defined in graph.hpp.  Feel free to change it there.

// In this example we request 1 unit, and set the reach of the least
// spectrum-efficient modulation to 8.

int
main()
{
  // Report the length of the longest shortest path.
  using au = adaptive_units<COST>;
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

  // The adjacency matrix of this graph, where (n) is node number n,
  // and [c] is link cost c:
  //
  // (0)---------[2]---------(2)---[2]---(3)
  //  \                       /
  //   \---[2]---(1)---[2]---/
  //
  std::vector<std::vector<COST>> G = {{0, 2, 2, 0},
                                      {2, 0, 2, 0},
                                      {2, 2, 0, 2},
                                      {0, 0, 2, 0}};

  // The availability matrix.  0 means the unit is available.
  std::vector<std::vector<int>> M = {
    {1, 0, 0, 0, 0}, // #0: link (0) -> (1)
    {0, 0, 0, 1, 1}, // #1: link (0) -> (2)
    {0, 0, 0, 0, 0}, // #2: link (1) -> (0)
    {1, 0, 0, 0, 0}, // #3: link (1) -> (2)
    {0, 0, 0, 0, 0}, // #4: link (2) -> (0)
    {0, 0, 0, 0, 0}, // #5: link (2) -> (1)
    {1, 0, 0, 0, 0}, // #6: link (2) -> (3)
    {0, 0, 0, 0, 0}, // #7: link (3) -> (2)
  };

  // We search for a path from 0 to 3, with 1 unit for the most
  // efficient modulation.
  auto r = interface(G, M, CU(0, 5), 0, 3, 1);

  cout << "The search took " << r.first << " s\n";
  cout << "Solution found: ";
    
  if (r.second)
      {
        const auto &path = r.second.value();
        cout << "links: ";
        for (int e: path.second)
          cout << e << ", ";
        cout << "units: (" << path.first.min() << ", "
             << path.first.max() << ")\n";
      }
  else
    cout << "none\n";
}
