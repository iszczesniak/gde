#ifndef LABEL_CREATOR_HPP
#define LABEL_CREATOR_HPP

#include "adaptive_units.hpp"
#include "generic_label.hpp"
#include "generic_label_creator.hpp"
#include "units.hpp"

#include <list>
#include <optional>

template <typename Weight>
class label_creator: generic_label_creator
{
  using label_type = generic_label<Weight, Units>;

  // The number of contiguous units initially requested for a demand.
  unsigned m_ncu;

public:
  label_creator(int ncu): m_ncu(ncu)
  {
  }

  template <typename Label, typename Edge>
  std::list<label_type>
  operator()(const Label &l, const Edge &e) const
  {
    // Candidate weight.
    Weight c_c = get_weight(l) + get_weight(e);

    // Constriction: consider that path when there is no maximal
    // length given or when the new lenght is not greater than the
    // limit.
    if (!m_ml || c_c <= m_ml.value())
      {
        // The label units.
        const Units &l_units = get_resources(l);
        // The units available on the edge.
        const auto &e_su = get_resources(e);
        // The candidate SU: the su of label l that can be carried by
        // edge e, and that has at least ncu contiguous units.
        auto c_su = intersection(SU{l_units}, e_su);
        c_su.remove(adaptive_units<Weight>::units(m_ncu, c_c));

	std::list<label_type> l;

	for (auto &cu: c_su)
	  // The candidate label.
	  l.push_back(label_type(c_c, std::move(cu)));

	return l;
      }

    // There is no label.
    throw true;
  }
};

#endif // LABEL_CREATOR_HPP
