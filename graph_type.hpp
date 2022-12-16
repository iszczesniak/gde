#ifndef GRAPH_TYPE_HPP
#define GRAPH_TYPE_HPP

#include "graph.hpp"
#include "props.hpp"
#include "units.hpp"

template <typename Edge>
using vertex_type_tmp = vertex<Edge, index<unsigned>,
                               name<std::string>>;

using edge_type = edge<vertex_type_tmp, weight<unsigned>,
                       resources<SU>>;
using vertex_type = vertex_type_tmp<edge_type>;
using graph_type = graph<vertex_type>;

#endif // GRAPH_TYPE_HPP
