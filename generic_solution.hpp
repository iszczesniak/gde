#ifndef GENERAL_SOLUTION_HPP
#define GENERAL_SOLUTION_HPP

#include "generic_label.hpp"

#include <cassert>
#include <map>
#include <utility>

template <typename Graph, typename Cost, typename Units>
struct generic_solution:
  public std::map<Vertex<Graph>, generic_labels<Graph, Cost, Units>>
{
  using base_type = std::map<Vertex<Graph>,
			     generic_labels<Graph, Cost, Units>>;
  using label_type = generic_label<Graph, Cost, Units>;
  using labels_type = generic_labels<Graph, Cost, Units>;
  using vertex_type = Vertex<Graph>;

  template <typename T>
  void
  push(T &&l)
  {
    // The target of the label.
    vertex_type trg = get_target(l);
    // The target labels.
    labels_type &trg_labels = base_type::operator[](trg);
    // Insert the label.
    bool status = trg_labels.insert(std::forward<T>(l)).second;
    // The insertion should always succeed.
    assert(status);
  }

  auto
  insert(typename labels_type::node_type &&nh)
  {
    // The label of the node handle.
    const auto &l = nh.value();
    // The target of the label.
    vertex_type trg = get_target(l);
    // The target labels.
    labels_type &trg_labels = base_type::operator[](trg);
    // The insert return value.
    auto irv = trg_labels.insert(std::move(nh));
    // Make sure the insertion was successful.
    assert(irv.inserted);
    // Return the result of the insertion.
    return irv;
  }

  auto
  pop()
  {
    // Find the set with the label of the lowest cost.
    auto qi =
      std::min_element(base_type::begin(), base_type::end(),
		       [](const auto &a, const auto &b)
		       {return *a.second.begin() < *b.second.begin();});
    // Make sure we found the one.
    assert(qi != base_type::end());
    // The source labels we move from.
    labels_type &src_labels = qi->second;
    // The iterator to the label we move.
    auto li = src_labels.cbegin();

    // Extract the node.
    auto nh = src_labels.extract(li);
    // Make sure we extracted the node.
    assert(nh);
    
    // Remove the src_labels if it is empty.
    if (src_labels.empty())
      base_type::erase(qi);

    return nh;
  }
};

#endif // GENERAL_SOLUTION_HPP
