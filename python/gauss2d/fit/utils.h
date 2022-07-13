#ifndef GAUSS2D_FIT_PYBIND11_UTILS_H
#define GAUSS2D_FIT_PYBIND11_UTILS_H

#include <string>

namespace gauss2d
{
namespace fit
{

// This is so annoying that I'm considering borrowing a solution for static string
template<typename T>
constexpr std::string_view suffix_type();

template<> constexpr std::string_view suffix_type<double>() { return "D";}

template<typename T>
std::string suffix_type_str() { return std::string(suffix_type<T>()); }

} // namespace fit
} // namespace gauss2d

#endif