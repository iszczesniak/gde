#ifndef GENERAL_LABEL_HPP
#define GENERAL_LABEL_HPP

#include <ostream>
#include <set>
#include <tuple>

template <typename Graph>
using Vertex = typename Graph::vertex_descriptor;

template <typename Graph>
using Edge = typename Graph::edge_descriptor;

// The label is a tuple (c, S, e, v) of cost c, SU S, edge descriptor
// e, and vertex descriptor v.  The label stores cost c and SU S of
// using edge e.
//
// In the tuple we store an edge descriptor, not a vertex descriptor,
// so that we can allow for multigraphs (i.e. graphs with parallel
// edges).
//
// Even though we could figure out the target vertex from the edge, we
// have to store it in the tuple to make a tuple unique, because in
// Boost the descriptors of a directed edge and its reverse are the
// same.

template <typename Graph, typename Cost, typename Units>
struct generic_label
{
  using self = generic_label<Graph, Cost, Units>;

  Cost cost;
  Units units;
  Edge<Graph> edge;
  Vertex<Graph> target;

  generic_label(): cost(), units(), edge(), target()
  {
  }

  generic_label(const Cost &c, Units &&u, const Edge<Graph> &e,
                const Vertex<Graph> &t):
    cost(c), units(std::move(u)), edge(e), target(t)
  {
  }

  generic_label(const self &) = default;

  generic_label(self &&) = default;

  bool
  operator == (const self &j) const
  {
    return cost == j.cost && target == j.target &&
           edge == j.edge && units == j.units;
  }

  bool
  operator != (const self &j) const
  {
    // We compare first the cost and target, since they are the most
    // likely to differ.  We compare the units at the very end,
    // because that comparison is time-consuming.
    return cost != j.cost || target != j.target ||
           edge != j.edge || units != j.units;
  }

  // This operator is used by containers to establish the order
  // between labels.  All we really care about is the cost, and the
  // units.  The order of units matters, because it influences the
  // spectrum allocation policy.  Here the units are sorted by their
  // increasing min() numbers (the < operator of cunits), and so the
  // specturm allocation policy is the first-fit.
  //
  // To distinguish different labels, we need to take into account the
  // other label properties, and to this end we use the operator < of
  // a tuple.  Tuple should be optimized out, so there is no overhead
  // in using the tuple here.
  bool
  operator < (const self &j) const
  {
    using tpl = std::tuple<const Cost &, const Units &,
			   const Edge<Graph> &, const Vertex<Graph> &>;
    return tpl(cost, units, edge, target) <
      tpl(j.cost, j.units, j.edge, j.target);
  }

  // This operator is used by our algorithm.
  bool
  operator <= (const self &j) const
  {
    return cost <= j.cost && units.includes(j.units);
  }
};

template <typename Graph, typename Cost, typename Units>
std::ostream &
operator << (std::ostream &os,
             const generic_label<Graph, Cost, Units> &l)
{
  os << "generic_label(" << l.cost << ", " << l.units
     << ", " << l.edge << ", " << l.target << ')';
  return os;
}

template <typename Graph, typename Cost, typename Units>
using generic_labels = std::set<generic_label<Graph, Cost, Units>>;

template <typename Graph, typename Cost, typename Units>
auto
get_cost(const generic_label<Graph, Cost, Units> &l)
{
  return l.cost;
}

template <typename Graph, typename Cost, typename Units>
auto
get_units(const generic_label<Graph, Cost, Units> &l)
{
  return l.units;
}

template <typename Graph, typename Cost, typename Units>
auto
get_edge(const generic_label<Graph, Cost, Units> &l)
{
  return l.edge;
}

template <typename Graph, typename Cost, typename Units>
auto
get_target(const generic_label<Graph, Cost, Units> &l)
{
  return l.target;
}

#endif // GENERAL_LABEL_HPP
