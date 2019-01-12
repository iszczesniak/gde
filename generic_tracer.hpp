#ifndef GENERAL_TRACER_HPP
#define GENERAL_TRACER_HPP

#include "adaptive_units.hpp"
#include "graph.hpp"

template <typename Label>
auto
get_cost(const Label &);

template <typename Label>
auto
get_units(const Label &);

template <typename Label>
auto
get_edge(const Label &);

CU
select_cu(const CU &cu, int ncu)
{
  assert(cu.count () >= ncu);
  return CU(cu.min(), cu.min() + ncu);
}

template <typename Graph, typename Path, typename Solution,
          typename Units>
class generic_tracer
{
  using Labels = typename Solution::mapped_type;
  using Label = typename Labels::value_type;

  // The graph.
  const Graph &m_g;

  // The number of contigous units.
  int m_ncu;

  // The units of the path.
  Units m_units;

public:
  using path_type = Path;

  generic_tracer(const Graph &g, int ncu):
    m_g(g), m_ncu(ncu), m_units()
  {
  }

  /**
   * Initialize the tracing.
   */
  typename Labels::const_iterator
  init(Path &p, typename Solution::const_iterator i)
  {
    // Make sure there is at least one label.
    assert(!i->second.empty());
    // This is the label.
    const Label &l = *i->second.begin();
    // Get the number of units required.
    int units = adaptive_units<COST>::units(m_ncu, get_cost(l));
    // Get the CU for the path.
    p.first = m_units = {select_cu(get_units(l), units)};
    return i->second.begin();
  }

  /**
   * Push the label to the path.
   */
  void
  push(Path &p, typename Labels::const_iterator &li)
  {
    // This is the label we process.
    const Label &l = *li;
    // The edge of the label.
    const Edge<Graph> &e = get_edge(l);
    // Add the label's edge to the path.
    p.second.push_front(e);
  }

  /**
   * We are given the iterator to the label that we process here.  We
   * require this is not the starting label.  The objective is to find
   * the previous label on the path.
   */
  typename Labels::const_iterator
  advance(const Solution &S, typename Labels::const_iterator li)
  {
    // This is the label we process.
    const Label &l = *li;
    // The cost of the label.
    const auto &c = get_cost(l);
    // This is the edge of the label.
    const Edge<Graph> &e = get_edge(l);
    // The edge cost.
    auto ec = boost::get(boost::edge_weight, m_g, e);
    // The edge source.
    Vertex<Graph> s = boost::source(e, m_g);

    // Find the labels at node s.
    auto i = S.find(s);
    assert(i != S.end());
    const auto &ls = i->second;

    // We look for a preceeding label at node s.  The loop condition
    // is true, because the loop should always be terminated by the
    // return statement.  Nonetheless, we make sure with the assertion
    // that we never reach the end.
    for(auto j = ls.begin(); assert(j != ls.end()), true; ++j)
      // Note that we are not subtracting (which is more natural,
      // since we are tracing back the path, and decreasing the cost),
      // but adding the costs, because during the search we use the
      // addition only.  Since we might be dealing with floating point
      // numbers (i.e., cost may be float or double), this could fail:
      //
      //  ec == c - get_cost(*j)
      //
      // And this will be fine:
      //
      //  get_cost(*j) + ec == c
      if (get_cost(*j) + ec == c && get_units(*j).includes(m_units))
        // This is the next label iterator.
        return j;

    // We should never get here!
    assert(false);
  }
};

#endif // GENERAL_TRACER_HPP
