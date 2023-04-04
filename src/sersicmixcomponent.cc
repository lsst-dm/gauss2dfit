#include "sersicmixcomponent.h"

#include <stdexcept>
#include <string>

#include "gauss2d/centroid.h"
#include "gauss2d/ellipse.h"
#include "gauss2d/evaluate.h"
#include "gauss2d/gaussian.h"

#include "channel.h"
#include "component.h"
#include "gaussianmodelintegral.h"
#include "integralmodel.h"
#include "linearsersicmixinterpolator.h"
#include "param_defs.h"
#include "param_filter.h"
#include "parametricellipse.h"
#include "sersicmix.h"

namespace gauss2d::fit
{

class SersicEllipseData : public EllipseData, public QuasiEllipse
{
private:
    const std::shared_ptr<const ReffXParameter> _size_x;
    const std::shared_ptr<const ReffYParameter> _size_y;
    const std::shared_ptr<const RhoParameter> _rho;
    const std::shared_ptr<const SersicMixComponentIndexParameter> _sersicindex;
    unsigned short _index;

public:
    double get_sizeratio() const { return _sersicindex->get_sizeratio(_index); }

    double get_sigma_x() const override { return get_sizeratio()*_size_x->get_value(); }
    double get_sigma_y() const override { return get_sizeratio()*_size_y->get_value(); }
    double get_rho() const override { return _rho->get_value(); }

    double get_size_x() const override { return get_sigma_x(); };
    double get_size_y() const override { return get_sigma_y(); };
    std::array<double, 3> get_xyr() const override { return {get_size_x(), get_size_y(), get_rho()}; };

    void set(double sigma_x, double sigma_y, double rho) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }
    void set_h(double hwhm_x, double hwhm_y, double rho) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }
    void set_sigma_x(double sigma_x) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }
    void set_sigma_y(double sigma_y) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }
    void set_rho(double rho) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }
    void set_hxyr(const std::array<double, 3> & hxyr) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }
    void set_xyr(const std::array<double, 3> & xyr) override {
        throw std::runtime_error("Can't set on SersicEllipseData");
    }

    std::string repr(bool name_keywords) const override {
        return std::string("SersicEllipseData(")
            + (name_keywords ? "size_x=" : "") + _size_x->repr(name_keywords) + ", "
            + (name_keywords ? "size_y=" : "") + _size_y->repr(name_keywords) + ", "
            + (name_keywords ? "rho=" : "") + _rho->repr(name_keywords) + ")";
    }

    std::string str() const override {
        return "SersicEllipseData(size_x=" + _size_x->str() + ", size_y=" + _size_y->str() + ", rho=" + _rho->str()
            +  + ")";
    }

    SersicEllipseData(
        const std::shared_ptr<const ReffXParameter> size_x,
        const std::shared_ptr<const ReffYParameter> size_y,
        const std::shared_ptr<const RhoParameter> rho,
        const std::shared_ptr<const SersicMixComponentIndexParameter> sersicindex,
        unsigned short index
    ) :
        _size_x(std::move(size_x)),
        _size_y(std::move(size_y)),
        _rho(std::move(rho)),
        _sersicindex(std::move(sersicindex)),
        _index(index)
    {
        if((_size_x == nullptr) || (_size_y == nullptr) || (_rho == nullptr) || (_sersicindex == nullptr)) {
            throw std::invalid_argument("SersicEllipseData args must not be nullptr");
        }
        if(!(_index < _sersicindex->order)) throw std::invalid_argument("index=" + std::to_string(_index)
            + "!< sersicindex->order=" + std::to_string(_sersicindex->order));
    }
};

class SersicModelIntegral : public GaussianModelIntegral, public IntegralModel
{
private:
    const std::shared_ptr<const SersicMixComponentIndexParameter> _sersicindex;
    unsigned short _index;

public:
    std::set<std::reference_wrapper<const Channel>> get_channels() const override {
        return {_channel};
    }

    double get_integral(const Channel & channel) const override {
        if(channel != _channel) {
            throw std::invalid_argument("channel=" + channel.str() + " != this->_channel=" + _channel.str());
        }
        return get_value();
    }
    std::vector<std::pair<ParamBaseCRef, extra_param_factor_values>> get_integral_derivative_factors(
        const Channel & channel) const override {
            // TODO: Refactor this?
        return {};
    }

    double get_integralratio() const  { return _sersicindex->get_integralratio(_index); }

    ParamRefs & get_parameters(ParamRefs & params, ParamFilter * filter = nullptr) const override {
        return _integralmodel->get_parameters(params, filter);
    }

    ParamCRefs & get_parameters_const(ParamCRefs & params, ParamFilter * filter = nullptr) const override {
        return _integralmodel->get_parameters_const(params, filter);
    }

    double get_value() const override {
        return get_integralratio()*_integralmodel->get_integral(_channel);
    }
    void set_value(double value) override { throw std::runtime_error("Can't set on SersicModelIntegral"); }

    std::string repr(bool name_keywords) const override {
        return std::string("SersicModelIntegral(")
            + (name_keywords ? "channel=" : "") + _channel.repr(name_keywords) + ", "
            + (name_keywords ? "integralmodel=" : "") + _integralmodel->repr(name_keywords) + ", "
            + (name_keywords ? "sersicindex=" : "") + _sersicindex->repr(name_keywords) + ", "
            + (name_keywords ? "index=" : "") + std::to_string(_index) + ")";
    }

    std::string str() const override {
        return "SersicModelIntegral(channel=" + _channel.str() + ", integralmodel=" + _integralmodel->str()
            + ", sersicindex=" + _sersicindex->str() + ", index=" + std::to_string(_index) + ")";
    }

    SersicModelIntegral(
        const Channel & channel, const std::shared_ptr<const IntegralModel> integralmodel,
        const std::shared_ptr<const SersicMixComponentIndexParameter> sersicindex, unsigned short index
    ) : GaussianModelIntegral(channel, integralmodel), _sersicindex(std::move(sersicindex)), _index(index)
    {
        if(_sersicindex == nullptr) throw std::invalid_argument("sersicindex must not be null");
        if(!(_index < _sersicindex->order)) throw std::invalid_argument("index=" + std::to_string(_index)
            + "!< sersicindex->order=" + std::to_string(_sersicindex->order));
    }
    
    ~SersicModelIntegral() {};
};

// This is the gauss2d convention; see evaluator.h
static const std::array<size_t, N_PARAMS_GAUSS2D> IDX_ORDER = {0, 1, 3, 4, 5, 2};

// TODO: This could derive from gauss2d::Gaussian, but would that serve any purpose?
// TODO: This was intended to derive from QuasiEllipticalComponent, but there also
// seems to be no need to implement all of its functions
class SersicMixComponent::SersicMixGaussianComponent
{
private:
    std::shared_ptr<SersicEllipseData> _ellipsedata;
    std::shared_ptr<CentroidParameters> _centroid;
    std::shared_ptr<SersicModelIntegral> _integralmodel;

public:
    const SersicEllipseData & get_ellipse() const { return *_ellipsedata; }
    const SersicModelIntegral & get_integralmodel() const { return *_integralmodel; }

    std::unique_ptr<const gauss2d::Gaussians> get_gaussians(const Channel & channel) const {
        gauss2d::Gaussians::Data gaussians = {
            std::make_shared<Gaussian>(
                std::make_shared<Centroid>(this->_centroid),
                std::make_shared<Ellipse>(this->_ellipsedata),
                std::make_shared<GaussianModelIntegral>(channel, this->_integralmodel)
            )
        };
        return std::make_unique<const gauss2d::Gaussians>(gaussians);
    }

    SersicMixGaussianComponent(
        std::shared_ptr<SersicEllipseData> ellipsedata = nullptr,
        std::shared_ptr<CentroidParameters> centroid = nullptr,
        std::shared_ptr<SersicModelIntegral> integralmodel = nullptr
    ) : _ellipsedata(std::move(ellipsedata)), _centroid(std::move(centroid)),
     _integralmodel(std::move(integralmodel))
    { }
};

static const std::shared_ptr<const SersicMixInterpolator> INTERPOLATOR_DEFAULT = std::make_shared<
    const LinearSersicMixInterpolator>(SERSICMIX_ORDER_DEFAULT);

void SersicMixComponentIndexParameter::_set_ratios(double sersicindex) {
    _integralsizes = _interpolator->get_integralsizes(sersicindex);
    _integralsizes_derivs = _interpolator->get_integralsizes_derivs(sersicindex);
}

double SersicMixComponentIndexParameter::get_integralratio(unsigned short index) const {
    if(index >= order) {
        throw std::invalid_argument(this->str() + ".get_integralratio(index=" + std::to_string(index)
            + " >= max(order=" + std::to_string(order) + "))");
    }
    return _integralsizes[index].integral;
}

double SersicMixComponentIndexParameter::get_integralratio_deriv(unsigned short index) const {
    if(index >= order) {
        throw std::invalid_argument(this->str() + ".get_integralratio_deriv(index="
            + std::to_string(index) + " >= max(order=" + std::to_string(order) + "))");
    }
    return _integralsizes_derivs[index].integral;
}

double SersicMixComponentIndexParameter::get_sizeratio(unsigned short index) const {
    if(index >= order) {
        throw std::invalid_argument(this->str() + ".get_integralratio(index=" + std::to_string(index)
        + " >= max(order=" + std::to_string(order) + "))");
    }
    return _integralsizes[index].sigma;
}

double SersicMixComponentIndexParameter::get_sizeratio_deriv(unsigned short index) const {
    if(index >= order) {
        throw std::invalid_argument(this->str() + ".get_integralratio(index=" + std::to_string(index)
        + " >= max(order=" + std::to_string(order) + "))");
    }
    return _integralsizes_derivs[index].sigma;
}

static const std::string limits_sersic_name = std::string(parameters::type_name<
    SersicMixComponentIndexParameter>()) + ".limits_maximal";

static const auto limits_sersic = std::make_shared<const parameters::Limits<double>>(
    0.5, 8.0, std::string(parameters::type_name<SersicMixComponentIndexParameter>()) + ".limits_maximal"
);

const parameters::Limits<double> & SersicMixComponentIndexParameter::get_limits_maximal() const {
    return *limits_sersic;
}

SersicMixComponentIndexParameter::SersicMixComponentIndexParameter(
    double value,
    std::shared_ptr<const parameters::Limits<double>> limits,
    const std::shared_ptr<const parameters::Transform<double>> transform,
    std::shared_ptr<const parameters::Unit> unit,
    bool fixed,
    std::string label,
    const std::shared_ptr<const SersicMixInterpolator> interpolator
) : SersicIndexParameter(value, nullptr, transform, unit, fixed, label),
    _interpolator(std::move(interpolator == nullptr ? INTERPOLATOR_DEFAULT : interpolator)),
    order(_interpolator->get_order())
{
    // TODO: determine if this can be avoided
    set_limits(std::move(limits));
    _set_ratios(value);
}

void SersicMixComponentIndexParameter::set_value(double value) {
    SersicIndexParameter::set_value(value);
    _set_ratios(value);
}

void SersicMixComponentIndexParameter::set_value_transformed(double value) {
    SersicIndexParameter::set_value_transformed(value);
    _set_ratios(get_value());
}

void SersicMixComponent::add_extra_param_map(
    const Channel & channel, extra_param_map & map_extra, const grad_param_map & map_grad, ParameterMap & offsets
) const
{
    if(_sersicindex->get_free())
    {
        auto found = offsets.find(*_sersicindex);
        if(found == offsets.end()) {
            throw std::runtime_error(
                "_sersicindex=" + _sersicindex->str() + " not found in offsets; was add_grad_param_map called?"
            );
        }
        const auto & offset = (*found).second;
        const size_t order = _sersicindex->order;
        const size_t size_map_grad = map_grad.size();
        if(!(size_map_grad >= order)) {
            throw std::invalid_argument(
                "map_grad.size()=" + std::to_string(map_grad.size()) + "!>=order=" + std::to_string(order)
            );
        }
        for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
        {
            map_extra.push_back({size_map_grad - order +  idx_g, offset});
        }
    } else {
        for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
        {
            map_extra.push_back({0, 0});
        }
    }
}

void SersicMixComponent::add_extra_param_factors(const Channel & channel, extra_param_factors & factors) const
{
    for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
    {
        factors.push_back({0, 0, 0});
    }
}

void SersicMixComponent::add_grad_param_map(const Channel & channel, grad_param_map & map, ParameterMap & offsets
    ) const
{
    ParamCRefs params;
    ParamFilter filter{};
    filter.channel = channel;
    this->get_parameters_const(params, &filter);

    if(params.size() != N_PARAMS) {
        throw std::runtime_error(
            this->str() + "get_parameters_const with channel=" + channel.str() + " size="
                + std::to_string(params.size()) + "!=N_PARAMS=" + std::to_string(N_PARAMS)
        );
    }

    std::array<size_t, N_PARAMS_GAUSS2D> values{0, 0, 0, 0, 0, 0};

    size_t size_map = offsets.size();
    size_t index_param_map;
    for(size_t idx_param = 0; idx_param < N_PARAMS_GAUSS2D; ++idx_param) {
        const size_t & order_param = IDX_ORDER[idx_param];
        // The parameters must be in the same order as returned by get_parameters(_const)
        const auto & param = params.at(idx_param).get();
        if(!param.get_fixed()) {
            if(offsets.find(param) == offsets.end()) {
                index_param_map = ++size_map;
                offsets[param] = index_param_map;
            } else {
                index_param_map = offsets[param];
            }
            values[order_param] = index_param_map;
        }
    }
    if(_sersicindex->get_free() && (offsets.find(*_sersicindex) == offsets.end())) {
        offsets[params[N_PARAMS_GAUSS2D]] = offsets.size() + 1;
    }
    for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
    {
        map.push_back(values);
    }
}

void SersicMixComponent::add_grad_param_factors(const Channel & channel, grad_param_factors & factors) const
{
    ParamCRefs params;
    ParamFilter filter{};
    filter.channel = channel;
    this->get_parameters_const(params, &filter);

    if(params.size() != N_PARAMS) {
        throw std::runtime_error(
            this->str() + "get_parameters_const with channel=" + channel.str() + " size="
                + std::to_string(params.size()) + "!=N_PARAMS=" + std::to_string(N_PARAMS)
        );
    }
    
    for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
    {
        std::array<double, N_PARAMS_GAUSS2D> values{1., 1., 1., 1., 1., 1.};
        factors.push_back(values);
    }
}

std::unique_ptr<const gauss2d::Gaussians> SersicMixComponent::get_gaussians(const Channel & channel) const {
    std::vector<std::optional<const gauss2d::Gaussians::Data>> in;
    // TODO: This isn't sufficient; need to implement get_n_components
    const auto & components = _gaussians.at(channel);
    in.reserve(components.size());
    for(auto & component : components) {
        in.push_back(component->get_gaussians(channel)->get_data());
    }
    return std::make_unique<gauss2d::Gaussians>(in);
}

size_t SersicMixComponent::get_n_gaussians(const Channel & channel) const {
    return _gaussians.at(channel).size();
};

ParamRefs & SersicMixComponent::get_parameters(ParamRefs & params, ParamFilter * filter) const {
    EllipticalComponent::get_parameters(params, filter);
    insert_param(*_sersicindex, params, filter);
    return params;
}

ParamCRefs & SersicMixComponent::get_parameters_const(ParamCRefs & params, ParamFilter * filter) const {
    EllipticalComponent::get_parameters_const(params, filter);
    insert_param(*_sersicindex, params, filter);
    return params;
}

void SersicMixComponent::set_extra_param_factors(
    const Channel & channel, extra_param_factors & factors, size_t index
) const {
    if(_sersicindex->get_free())
    {
        const auto & integralmodel = this->get_integralmodel();
        double dx = _sersicindex->get_transform_derivative();
        const double dintegral = dx*integralmodel.get_integral(channel);
        const double dreff_x = dx*_ellipse->get_size_x_param().get_size();
        const double dreff_y = dx*_ellipse->get_size_y_param().get_size();
        for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
        {
            const double dsizeratio = _sersicindex->get_sizeratio_deriv(idx_g);
            auto & values = factors.at(index + idx_g);
            values[0] = dintegral*_sersicindex->get_integralratio_deriv(idx_g);
            values[1] = dreff_x*dsizeratio;
            values[2] = dreff_y*dsizeratio;
        }
    } else {
        for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
        {
            auto & values = factors[index + idx_g];
            values[0] = 0;
            values[1] = 0;
            values[2] = 0;
        }
    }
}

void SersicMixComponent::set_grad_param_factors(
    const Channel & channel, grad_param_factors & factors, size_t index
) const {
    ParamCRefs params;
    ParamFilter filter{};
    filter.channel = channel;
    this->get_parameters_const(params, &filter);

    std::array<double, N_PARAMS_GAUSS2D> values_base{1., 1., 1., 1., 1., 1.};

    for(size_t idx_param = 0; idx_param < N_PARAMS_GAUSS2D; ++idx_param) {
        const size_t & order_param = IDX_ORDER[idx_param];
        // The parameters must be in the same order as returned by get_parameters(_const)
        const auto & param = params.at(idx_param).get();
        if(param.get_fixed()) {
            values_base[order_param] = 0;
        } else {
            const auto deriv = param.get_transform_derivative();
            if(deriv == 0) throw std::runtime_error("Param[idx=" + std::to_string(idx_param) + "="
                + param.str() + " get_transform_derivative=0 (will result in divide by 0)");
            values_base[order_param] /= deriv;
        }
    }

    const auto & subcomps = _gaussians.at(channel);
    for(size_t idx_g = 0; idx_g < _sersicindex->order; ++idx_g)
    {
        const auto & subcomp = subcomps[idx_g];
        auto & values = factors[index + idx_g];
        values[0] = values_base[0];
        values[1] = values_base[1];
        values[2] = values_base[2]*subcomp->get_integralmodel().get_integralratio();
        const double sizeratio = subcomp->get_ellipse().get_sizeratio();
        values[3] = values_base[3]*sizeratio;
        values[4] = values_base[4]*sizeratio;
        values[5] = values_base[5];
    }
}

std::string SersicMixComponent::repr(bool name_keywords) const {
    return "SersicMixComponent(" + EllipticalComponent::repr(name_keywords) + ", "
        + (name_keywords ? "photo=" : "") + _integralmodel->repr(name_keywords) + ")";
}

std::string SersicMixComponent::str() const {
    return "SersicMixComponent(" + EllipticalComponent::str() + ", photo=" + _integralmodel->str() + ")";
}

SersicMixComponent::SersicMixComponent(
    std::shared_ptr<SersicParametricEllipse> ellipse,
    std::shared_ptr<CentroidParameters> centroid,
    std::shared_ptr<IntegralModel> integralmodel,
    std::shared_ptr<SersicMixComponentIndexParameter> sersicindex
) : SersicParametricEllipseHolder(std::move(ellipse)),
    EllipticalComponent(_ellipsedata, centroid, integralmodel),
    _sersicindex(sersicindex != nullptr ? std::move(sersicindex)
        : std::make_shared<SersicMixComponentIndexParameter>())
{
    for(const Channel & channel : _integralmodel->get_channels()) {
        auto & gaussians = _gaussians[channel];
        gaussians.reserve(_sersicindex->order);
        for(size_t index = 0; index < _sersicindex->order; ++index) {
            auto ell = std::make_shared<SersicEllipseData>(
                _ellipsedata->get_reff_x_param_ptr(),
                _ellipsedata->get_reff_y_param_ptr(),
                _ellipsedata->get_rho_param_ptr(),
                _sersicindex,
                index
            );
            auto integral = std::make_shared<SersicModelIntegral>(
                channel,
                _integralmodel,
                _sersicindex,
                index
            );
            gaussians.emplace_back(
                std::make_unique<SersicMixGaussianComponent>(ell, this->_centroid, integral)
            );
        }
    };
}

SersicMixComponent::~SersicMixComponent() {};

} // namespace gauss2d::fit
