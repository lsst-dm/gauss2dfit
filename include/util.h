#ifndef GAUSS2D_FIT_UTIL_H
#define GAUSS2D_FIT_UTIL_H

#include <algorithm>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gauss2d::fit {

/**
 * The result of an isclose() check.
 *
 * @tparam T The type of value being checked (usually float/double).
 */
template <typename T>
struct IsCloseResult {
    const bool isclose;
    const double diff_abs;
    const T margin;

    std::string str() const {
        std::stringstream ss;
        ss.precision(5);
        ss << "isclose=" << isclose << " from diff=" << diff_abs << " <= margin=" << margin;
        return ss.str();
    }
};

/**
 * @brief Check if two values are close, within some tolerances.
 *
 * This function mimics the behaviour of numpy.isclose as of v1.7.0
 *
 * @note See https://numpy.org/doc/stable/reference/generated/numpy.isclose.html
 *
 * @tparam T The type of value being checked (usually float/double).
 * @param a The (measured) value to check.
 * @param b The (reference) value to check against.
 * @param rtol The relative tolerance.
 * @param atol The absolute tolerance.
 * @return A IsCloseResult object with the result and some metadata.
 */
template <typename T>
IsCloseResult<T> isclose(T a, T b, T rtol = 1e-5, T atol = 1e-8) {
    const T diff_abs = std::abs(a - b);
    const T margin = atol + rtol * std::abs(b);
    return IsCloseResult<T>{diff_abs <= margin, diff_abs, margin};
}

// The rest of these functions are mainly intended to make printing container members easier
template <template <typename...> class Map, class Key, class Value>
std::set<Key> map_keys(const Map<Key, Value>& map) {
    std::set<Key> result;
    for (const auto& it : map) {
        result.insert(it.first);
    }
    return result;
}

template <template <typename...> class Map, class Key, class Value>
std::set<std::reference_wrapper<const Key>> map_keys_ref_const(
        const Map<std::reference_wrapper<const Key>, Value, std::less<const Key>>& map) {
    std::set<std::reference_wrapper<const Key>> result;
    for (const auto& it : map) result.insert(it.first.get());
    return result;
}

template <template <typename...> class Map, class Key, class Value>
std::set<std::shared_ptr<const Value>> map_values_shared_ptr_const(
        const Map<Key, std::shared_ptr<const Value>>& map) {
    std::set<std::shared_ptr<const Value>> result;
    for (const auto& it : map) result.insert(it.second);
    return result;
}

template <template <typename...> class Map, class Key, class Value>
std::set<std::reference_wrapper<const Value>> map_values_ref_const(
        const Map<Key, std::reference_wrapper<const Value>>& map) {
    std::set<std::reference_wrapper<const Value>> result;
    for (const auto& it : map) result.insert(it.second);
    return result;
}

// Builds a new vector with only unique elements, preserving order
// Note: std::unique only removes consecutive identical elements
// Also, this returns a new vector, potentially copying elements
template <typename T>
std::vector<T> nonconsecutive_unique(const std::vector<T>& vec) {
    std::set<T> set{};
    std::vector<T> rval{};
    rval.reserve(vec.size());
    for (const auto& elem : vec) {
        auto result = set.insert(elem);
        if (result.second) rval.push_back(elem);
    }
    return rval;
}

template <typename T>
std::string repr_ptr(const T* obj, bool name_keywords = false) {
    return (obj == nullptr) ? "None" : obj->repr(name_keywords);
}

template <typename T>
std::string repr_iter_ref(const T& container, bool name_keywords = false) {
    std::string str = "[";
    for (const auto& obj : container) str += obj.repr(name_keywords) + ",";
    return str.substr(0, str.size() - 1) + "]";
}

template <typename T>
std::string str_ptr(const T* obj) {
    return (obj == nullptr) ? "None" : obj->str();
}

template <typename T>
std::string str_iter_ptr(const T& container) {
    std::string str = "[";
    for (const auto& obj : container) str += str_ptr(obj) + ",";
    return str.substr(0, str.size() - 1) + "]";
}

template <typename T>
std::string str_iter_ref(const T& container) {
    std::string str = "[";
    for (const auto& obj : container) str += obj.str() + ",";
    return str.substr(0, str.size() - 1) + "]";
}

template <typename T>
std::string str_iter_refw(const T& container) {
    std::string str = "[";
    for (const auto& obj : container) str += obj.get().str() + ",";
    return str.substr(0, str.size() - 1) + "]";
}

template <typename T>
std::string str_map_refw(const T& container) {
    std::string str = "{";
    for (const auto& [obj, value] : container) {
        str += obj.get().str() + ": " + std::to_string(value) + ", ";
    }
    return str.substr(0, str.size() - 2 * (container.size() > 0)) + "}";
}

template <typename T>
void stream_iter_ref(const T& container, std::ostream& stream) {
    stream << "[";
    for (const auto& obj : container) stream << obj << ",";
    stream << "]";
}

template <template <typename...> class Container, class Value>
Value sum_iter(const Container<Value>& container) {
    Value sum = 0;
    for (const auto& value : container) sum += value;
    return sum;
}

template <template <typename...> class Container, class Value>
Container<Value> head_iter(const Container<Value>& container, size_t n) {
    return Container<Value>(container.begin(), container.begin() + n);
}

template <template <typename...> class Container, class Value>
Container<Value> tail_iter(const Container<Value>& container, size_t n) {
    return Container<Value>(container.end() - n, container.end());
}

template <typename T>
std::string to_string_float(const T value, const int precision = 6, const bool scientific = true) {
    std::ostringstream out;
    out.precision(precision);
    if (scientific) {
        out << std::scientific;
    } else {
        out << std::fixed;
    }
    out << value;
    return std::move(out).str();
}

template <template <typename...> class Container, class Value>
std::string to_string_float_iter(const Container<Value>& container, const int precision = 6,
                                 const bool scientific = true) {
    std::string str = "[";
    for (const auto& value : container) {
        str += to_string_float(value, precision, scientific) + ", ";
    }
    return str.substr(0, str.size() - 2 * (container.size() > 0)) + "]";
}

}  // namespace gauss2d::fit

#endif
