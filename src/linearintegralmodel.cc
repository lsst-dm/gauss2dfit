#include "linearintegralmodel.h"

#include "channel.h"
#include "param_filter.h"
#include "parameters.h"
#include "util.h"

namespace gauss2d::fit {

std::vector<std::reference_wrapper<const Channel>> LinearIntegralModel::get_channels() const {
    std::vector<std::reference_wrapper<const Channel>> rval = {};
    for (auto& datum : _data) rval.emplace_back(datum.first);
    return rval;
}

double LinearIntegralModel::get_integral(const Channel& channel) const {
    return _map.at(channel)->get_value();
}

std::vector<std::pair<ParamBaseCRef, ExtraParamFactorValues>>
LinearIntegralModel::get_integral_derivative_factors(const Channel& channel) const {
    return {};
}

std::shared_ptr<IntegralParameterD> LinearIntegralModel::at(const Channel& channel) {
    return _map.at(channel);
}

std::shared_ptr<const IntegralParameterD> LinearIntegralModel::at(const Channel& channel) const {
    return _map.at(channel);
}

typename LinearIntegralModel::Data::iterator LinearIntegralModel::begin() noexcept { return _data.begin(); }
typename LinearIntegralModel::Data::const_iterator LinearIntegralModel::cbegin() const noexcept {
    return _data.begin();
}

typename LinearIntegralModel::Data::iterator LinearIntegralModel::end() noexcept { return _data.end(); }
typename LinearIntegralModel::Data::const_iterator LinearIntegralModel::cend() const noexcept {
    return _data.cend();
}

ParamRefs& LinearIntegralModel::get_parameters(ParamRefs& params, ParamFilter* filter) const {
    for (const auto& p : _data) insert_param_channel(p.first, *p.second, params, filter);
    return params;
}
ParamCRefs& LinearIntegralModel::get_parameters_const(ParamCRefs& params, ParamFilter* filter) const {
    for (const auto& p : _data) insert_param_channel(p.first, *p.second, params, filter);
    return params;
}

size_t LinearIntegralModel::size() const { return _data.size(); }

std::string LinearIntegralModel::repr(bool name_keywords) const {
    std::string s = std::string("LinearIntegralModel(") + (name_keywords ? "data=" : "") + "{";
    for (const auto& datum : _data) {
        s += datum.first.get().repr(name_keywords) + ": " + datum.second->repr(name_keywords) + ",";
    }
    return s + "})";
}

std::string LinearIntegralModel::str() const {
    std::string s = "LinearIntegralModel(data={";
    for (const auto& datum : _data) {
        s += datum.first.get().str() + ": " + datum.second->str() + ",";
    }
    return s + "})";
}

// not giving a nullptr default because users should explicitly use the null Channel
LinearIntegralModel::LinearIntegralModel(const Data* data_in) {
    if (data_in != nullptr) {
        size_t idx = 0;
        for (const auto& datum : *data_in) {
            if (datum.second == nullptr) {
                throw std::runtime_error("LinearIntegralModel data[" + std::to_string(idx)
                                         + "] can't be null");
            }
            if (_map.find(datum.first) != _map.end()) {
                throw std::runtime_error("LinearIntegralModel data[" + std::to_string(idx)
                                         + "] channel=" + datum.first.get().str() + " duplicated");
            }
            _data.emplace_back(datum.first, datum.second);
            _map.insert(_data.back());
            idx++;
        }
    } else {
        _data.emplace_back(Channel::NONE(), std::make_shared<IntegralParameterD>(1));
        _map.insert(_data.back());
    }
}
LinearIntegralModel::~LinearIntegralModel(){};

}  // namespace gauss2d::fit
