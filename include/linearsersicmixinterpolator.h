#ifndef GAUSS2D_FIT_LINEARSERSICMIXINTERPOLATOR_H
#define GAUSS2D_FIT_LINEARSERSICMIXINTERPOLATOR_H

#include "sersicmix.h"

namespace gauss2d
{
namespace fit
{

class LinearSersicMixInterpolator : public SersicMixInterpolator {
private:
    const unsigned short _order;

public:
    const std::vector<SersicMixValues> & knots;

    std::vector<IntegralSize> get_integralsizes(double sersicindex) const override;
    unsigned short get_order() const override;

    const double sersicindex_min;
    const double sersicindex_max;

    std::string str() const override;

    LinearSersicMixInterpolator(unsigned short order);
    ~LinearSersicMixInterpolator();
};

} // namespace fit
} // namespace gauss2d

#endif