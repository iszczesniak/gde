#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <boost/range.hpp>

#include <list>
#include <optional>

template <typename Graph>
using Vertex = typename Graph::vertex_descriptor;

template <typename Graph>
using Edge = typename Graph::edge_descriptor;

/**
 * Find the shortest path in graph g to t.
 */
template <typename Graph, typename Solution, typename Queue,
          typename Label, typename Functor>
void
dijkstra(const Graph &g, Solution &S, Queue &Q, const Label &l,
         const Functor &f, Vertex<Graph> t)
{
  // Boot the search.
  Q.push(l);

  while(!Q.empty())
    {
      const Label &l = move_label(S, Q);
      Vertex<Graph> v = get_target(l);

      // Stop searching when we reach the destination node.
      if (v == t)
        break;

      // Itereate over the out edges of vertex v.
      for(const auto &e: boost::make_iterator_range(out_edges(v, g)))
        relax(g, S, Q, e, l, f);
    }
}

/**
 * Try to relax edge e, given label l.
 */
template <typename Graph, typename Solution, typename Queue,
          typename Label, typename Functor>
void
relax(const Graph &g, Solution &S, Queue &Q, const Edge<Graph> &e,
      const Label &l, const Functor &f)
{
  try
    {
      // Candidate labels.
      auto cls = f(e, l);

      for (auto &cl: cls)
	if (!has_better_or_equal(S, cl) && !has_better_or_equal(Q, cl))
	  {
	    purge_worse(Q, cl);
	    // We push the new label after purging, so that purging
	    // has less work, i.e., a smaller Q.  Furthermore, in
	    // purge_worse we are using the <= operator, which would
	    // remove the label we push below.
	    Q.push(std::move(cl));
	  }
    } catch (bool no_label)
    {
      assert(no_label);
    }
}

/**
 * Build the path by tracing labels.
 */
template <typename Solution, typename Vertex, typename Label,
          typename Tracer>
std::optional<typename Tracer::path_type>
trace(const Solution &S, Vertex dst, const Label &sl, Tracer &t)
{
  // Find the solution for the dst node, the destination iterator.
  // This mundane work should be done here, not in the user functor.
  // We want to reuse the iterator i, and so the user functor should
  // accept this iterator.
  auto i = S.find(dst);

  // Check whether there is a solution for node dst in S.
  if (i != S.end())
    {
      // This is the path we're building.
      typename Tracer::path_type result;

      // Get the initial label, i.e. the label for the destination.
      for(auto li = t.init(result, i); *li != sl; li = t.advance(S, li))
        t.push(result, li);

      // Move the result to the optional object we return.
      return std::move(result);
    }

  // We return an empty optional, becase no path was found.
  return std::optional<typename Tracer::path_type>();
}

#endif // DIJKSTRA_HPP
