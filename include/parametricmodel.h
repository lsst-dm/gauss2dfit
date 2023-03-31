#ifndef GAUSS2D_FIT_PARAMETRICMODEL_H
#define GAUSS2D_FIT_PARAMETRICMODEL_H

#include "gauss2d/evaluate.h"
#include "gauss2d/gaussian.h"

#include "channel.h"
#include "parametric.h"

namespace gauss2d::fit
{
typedef std::vector<std::array<size_t, gauss2d::N_EXTRA_MAP>> extra_param_map;
typedef std::array<double, gauss2d::N_EXTRA_FACTOR> extra_param_factor_values;
typedef std::vector<extra_param_factor_values> extra_param_factors;
typedef std::vector<std::array<size_t, gauss2d::N_PARAMS_GAUSS2D>> grad_param_map;
typedef std::vector<std::array<double, gauss2d::N_PARAMS_GAUSS2D>> grad_param_factors;

typedef std::map<ParamBaseCRef, size_t> ParameterMap;

class ParametricModel : public Parametric
{
public:
    virtual void add_extra_param_map(
        const Channel & channel, extra_param_map & map_extra, const grad_param_map & map_grad, ParameterMap & offsets
    ) const = 0;
    virtual void add_extra_param_factors(const Channel & channel, extra_param_factors & factors) const = 0;
    virtual void add_grad_param_map(const Channel & channel, grad_param_map & map, ParameterMap & offsets
        ) const = 0;
    virtual void add_grad_param_factors(const Channel & channel, grad_param_factors & factors) const = 0;
    virtual void set_extra_param_factors(const Channel & channel, extra_param_factors & factors, size_t index) const = 0;
    virtual void set_grad_param_factors(const Channel & channel, grad_param_factors & factors, size_t index) const = 0;
    
    virtual std::unique_ptr<const gauss2d::Gaussians> get_gaussians(const Channel & channel) const = 0;
    virtual size_t get_n_gaussians(const Channel & channel) const = 0;
};

} // namespace gauss2d::fit

#endif
