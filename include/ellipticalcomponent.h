#ifndef GAUSS2D_FIT_ELLIPTICALCOMPONENT_H
#define GAUSS2D_FIT_ELLIPTICALCOMPONENT_H

#include "centroidparameters.h"
#include "component.h"
#include "ellipseparameters.h"
#include "param_defs.h"
#include "param_filter.h"

namespace gauss2d
{
namespace fit
{

class EllipticalComponent : public Component
{
protected:
    std::shared_ptr<CentroidParameters> _centroid;
    std::shared_ptr<EllipseParameters> _ellipse;
    std::shared_ptr<IntegralModel> _integralmodel;

public:
    const CentroidParameters & get_centroid() const;
    const EllipseParameters & get_ellipse() const;
    const IntegralModel & get_integralmodel() const;

    ParamRefs & get_parameters(ParamRefs & params, ParamFilter * filter = nullptr) const override;
    ParamCRefs & get_parameters_const(ParamCRefs & params, ParamFilter * filter = nullptr) const override;

    std::string str() const override;

    EllipticalComponent(
        std::shared_ptr<CentroidParameters> centroid = nullptr,
        std::shared_ptr<EllipseParameters> ellipse = nullptr,
        std::shared_ptr<IntegralModel> integralmodel = nullptr
    );
};

} // namespace fit
} // namespace gauss2d

#endif