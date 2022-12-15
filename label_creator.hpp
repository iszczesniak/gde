#ifndef LABEL_CREATOR_HPP
#define LABEL_CREATOR_HPP

#include "adaptive_units.hpp"
#include "generic_label.hpp"
#include "units.hpp"

#include <list>

// Some of the functionality here is similar to generic_label_creator,
// but it would be hard to use here.  I could call operator() of
// generic_label_creator, and then remove from the list some labels,
// but that would be inefficient.  I could pass to operator() a
// functor that would remove the unwanted slots, but such
// customization would be too specific, rather unfit to other uses.
// So it's best just to implement this struct.
struct label_creator
{
  // The number of contiguous units initially requested for a demand.
  unsigned m_ncu;

  label_creator(unsigned ncu): m_ncu(ncu)
  {
  }

  template <typename Label, typename Edge>
  std::list<Label>
  operator()(const Label &l, const Edge &e) const
  {
    // Candidate weight.
    auto c_w = get_weight(l) + get_weight(e);

    // The label units.
    const auto &l_units = get_resources(l);
    // The units available on the edge.
    const auto &e_su = get_resources(e);
    // The candidate SU: the su of label l that can be carried by
    // edge e, and that has at least ncu contiguous units.
    auto c_su = intersection(SU{l_units}, e_su);
    // Remove the slots that have too few units.
    c_su.remove(adaptive_units::units(m_ncu, c_w));

    std::list<Label> l;

    for (auto &cu: c_su)
      // The candidate label.
      l.push_back(label_type(c_w, std::move(cu)));
  }
};

#endif // LABEL_CREATOR_HPP
