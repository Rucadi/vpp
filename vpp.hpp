#pragma once

#include <type_traits>
#include <utility>
#include <variant>

namespace vpp {

namespace detail {

template<typename T>
struct is_specialization_of_variant : std::false_type {};

template<typename... Args>
struct is_specialization_of_variant<std::variant<Args...>> : std::true_type {};

template<typename T>
struct is_variant : is_specialization_of_variant<std::remove_cv_t<std::remove_reference_t<T>>> {};

template<typename T>
constexpr bool is_variant_v = is_variant<T>::value;

template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};

// Visitor wrapper
template<class Over>
struct visitor_t {
private:
    Over over;

public:
    explicit visitor_t(Over o) : over(std::move(o)) {}

    // Compose another functor
    template<typename U>
    auto operator&(U&& u) const {
        using UDec = std::decay_t<U>;
        return visitor_t<detail::overload<Over, UDec>>{
            detail::overload<Over, UDec>{ over, std::forward<U>(u) }
        };
    }

    // Single-variant visit
    template<typename V, typename = std::enable_if_t<detail::is_variant_v<V>>>
    decltype(auto) operator()(V&& v) const {
        return std::visit(over, std::forward<V>(v));
    }

    // Multi-variant visit
    template<typename V1, typename V2, typename... Vs,
             typename = std::enable_if_t<
                 detail::is_variant_v<V1> && detail::is_variant_v<V2>
             >>
    decltype(auto) operator()(V1&& v1, V2&& v2, Vs&&... vs) const {
        return std::visit(over, std::forward<V1>(v1), std::forward<V2>(v2), std::forward<Vs>(vs)...);
    }
};


} // namespace detail


constexpr inline struct {
    template<typename U>
    auto operator&(U&& u) const {
        using UDec = std::decay_t<U>;
        return detail::visitor_t<detail::overload<UDec>>{ detail::overload<UDec>{ std::forward<U>(u) } };
    }
} visitor{};

} // namespace vpp

/*
MIT License

Copyright (c) 2025 Ruben Cano Diaz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/