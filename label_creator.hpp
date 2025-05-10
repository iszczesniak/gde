#ifndef LABEL_CREATOR_HPP
#define LABEL_CREATOR_HPP

#include "adaptive_units.hpp"
#include "generic_label_creator.hpp"

#include <list>

template <typename Weight>
struct label_creator: generic_label_creator
{
  using weight_type = Weight;
  using base_type = generic_label_creator;

  // The number of contiguous units initially requested for a demand.
  unsigned m_ncu;

  label_creator(unsigned ncu): m_ncu(ncu)
  {
  }

  template <typename Label, typename Edge>
  auto
  operator()(const Label &l, const Edge &e) const
  {
    // Candidate weight and resources.
    auto [c_w, c_su] = base_type::operator()(l, e);

    // The number of required contiguous units for c_w.
    auto t = adaptive_units<weight_type>::units(m_ncu, c_w);

    std::list<Label> ret;

    for (auto &cu: c_su)
      if (cu.size() >= t)
	// Add the candidate label.
	ret.push_back(Label(e, c_w, std::move(cu)));

    return ret;
  }
};

#endif // LABEL_CREATOR_HPP
