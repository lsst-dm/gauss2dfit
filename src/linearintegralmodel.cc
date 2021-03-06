#include "linearintegralmodel.h"

#include "channel.h"
#include "parameters.h"
#include "util.h"


namespace gauss2d
{
namespace fit
{

std::set<std::reference_wrapper<const Channel>> LinearIntegralModel::get_channels() const {
    return map_keys_ref_const(_data);
}

double LinearIntegralModel::get_integral(const Channel & channel) const {
    return _data.at(channel)->get_value();
}

std::shared_ptr<IntegralParameter> LinearIntegralModel::at(const Channel & channel) {
    return _data.at(channel);
}

std::shared_ptr<const IntegralParameter> LinearIntegralModel::at(const Channel & channel) const {
    return _data.at(channel);
}

typename LinearIntegralModel::Data::iterator LinearIntegralModel::begin() noexcept {
    return _data.begin();
}
typename LinearIntegralModel::Data::const_iterator LinearIntegralModel::cbegin() const noexcept {
    return _data.begin();
}

typename LinearIntegralModel::Data::iterator LinearIntegralModel::end() noexcept {
    return _data.end();
}
typename LinearIntegralModel::Data::const_iterator LinearIntegralModel::cend() const noexcept {
    return _data.cend();
}

ParamRefs & LinearIntegralModel::get_parameters(ParamRefs & params, ParamFilter * filter) const {
    for(auto & p: _data) params.push_back(*p.second);
    return params;
}
ParamCRefs & LinearIntegralModel::get_parameters_const(ParamCRefs & params, ParamFilter * filter) const {
    for(const auto & p: _data) params.push_back(*p.second);
    return params;
}

size_t LinearIntegralModel::size() const { return _data.size(); }

std::string LinearIntegralModel::str() const {
    std::string s = "LinearIntegralModel({";
    for(const auto & [channel, integral] : _data) s += channel.get().str() + ": " + integral->str() + ",";
    return s + "})";
}

// not giving a nullptr default because users should explicitly use the null Channel
LinearIntegralModel::LinearIntegralModel(const Data * data_in)
{
    if(data_in != nullptr)
    {
        for(const auto & [channel, datum] : *data_in)
        {
            if(datum == nullptr) throw std::runtime_error("GaussianIntegrals data["
                + channel.get().str() + "] can't be null");
            _data[channel] = datum;
        }
    } else {
        _data[Channel::NONE()] = std::make_shared<IntegralParameter>(1);
    }
}
LinearIntegralModel::~LinearIntegralModel() {};

} // namespace fit
} // namespace gauss2d
