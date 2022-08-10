#ifndef GAUSS2D_FIT_SERSICMIXCOMPONENT_H
#define GAUSS2D_FIT_SERSICMIXCOMPONENT_H

#include <parameters/parameter.h>

#include "channel.h"
#include "ellipticalcomponent.h"
#include "integralmodel.h"
#include "param_defs.h"
#include "param_filter.h"
#include "sersicmix.h"
#include "sersicparametricellipse.h"

namespace gauss2d
{
namespace fit
{

class SersicMixComponentIndexParameter : public SersicIndexParameter {
private:
    std::vector<IntegralSize> _integralsizes;
    const std::shared_ptr<const SersicMixInterpolator> _interpolator;

    void _set_ratios(double sersicindex);

public:
    double get_integralratio(unsigned short index) const;
    const parameters::Limits<double> & get_limits_maximal() const override;
    double get_min() const override { return 0.5; }
    double get_max() const override { return 8.0; }
    double get_sizeratio(unsigned short index) const;

    unsigned short order;

    void set_value(double value) override;
    void set_value_transformed(double value_transformed) override;

    SersicMixComponentIndexParameter(
        double value = _get_default(),
        std::shared_ptr<const parameters::Limits<double>> limits = nullptr,
        const std::shared_ptr<const parameters::Transform<double>> transform = nullptr,
        std::shared_ptr<const parameters::Unit> unit = nullptr,
        bool fixed = false,
        std::string label = "",
        const SetC & inheritors = {},
        const SetC & modifiers = {},
        const std::shared_ptr<const SersicMixInterpolator> interpolator = nullptr
    );
};

// TODO: Revisit the necessity of this class
class SersicParametricEllipseHolder {
public:
    std::shared_ptr<SersicParametricEllipse> _ellipsedata;

    SersicParametricEllipseHolder(std::shared_ptr<SersicParametricEllipse> ellipse = nullptr)
    : _ellipsedata(std::move(ellipse)) {
        if(_ellipsedata == nullptr) _ellipsedata = std::make_shared<SersicParametricEllipse>();
    }
};

class SersicMixComponent : private SersicParametricEllipseHolder, public EllipticalComponent {
private:
    std::shared_ptr<SersicMixComponentIndexParameter> _sersicindex;
    std::map<std::reference_wrapper<const Channel>, gauss2d::Gaussians::Data> _gaussians;

public:
    std::unique_ptr<const gauss2d::Gaussians> get_gaussians(const Channel & channel) const override;

    ParamRefs & get_parameters(ParamRefs & params, ParamFilter * filter = nullptr) const override;
    ParamCRefs & get_parameters_const(ParamCRefs & params, ParamFilter * filter = nullptr) const override;

    std::string str() const override;
    
    SersicMixComponent(
        std::shared_ptr<SersicParametricEllipse> ellipse = nullptr,
        std::shared_ptr<CentroidParameters> centroid = nullptr,
        std::shared_ptr<IntegralModel> integralmodel = nullptr,
        std::shared_ptr<SersicMixComponentIndexParameter> sersicindex = nullptr
    );
};
} // namespace fit
} // namespace gauss2d

#endif