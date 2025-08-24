# vpp::visitor

A lightweight C++17 utility for composing and applying visitors to `std::variant`.  
It provides an elegant syntax for building overload sets and visiting one or multiple variants.

## Features

- **Compose visitors** easily with `&` operator.
- **Supports single and multi-variant visitation**.
- **Header-only**, no external dependencies.
- **C++17 compatible**.

## Example Usage

```cpp


void example_unary_visitor() {
    std::variant<int, double, std::string> v1 = 42;
    std::variant<int, double, std::string> v2 = 3.14;
    std::variant<int, double, std::string> v3 = "Hello";

    auto to_string_visitor = vpp::visitor
        & [](int x) { return std::to_string(x); }
        & [](double d) { return std::to_string(d); }
        & [](const std::string& s) { return s; };

    std::println("[Unary visitor example]");
    std::println("v1: {}", to_string_visitor(v1));
    std::println("v2: {}", to_string_visitor(v2));
    std::println("v3: {}", to_string_visitor(v3));
}

void example_binary_visitor() {
    std::variant<int, std::string> a = 10;
    std::variant<int, std::string> b = "world";

    auto combine_visitor = vpp::visitor
        & [](int f, int s) { return std::to_string(f + s); }
        & [](int f, const std::string& s) { return std::to_string(f) + s; }
        & [](const std::string& f, int s) { return f + std::to_string(s); }
        & [](const std::string& f, const std::string& s) { return f + s; };

    std::println("\n[Binary visitor example]");
    std::println("a + b: {}", combine_visitor(a, b));
    std::println("b + a: {}", combine_visitor(b, a));

    a = 5;
    b = 7;
    std::println("a + b (ints): {}", combine_visitor(a, b));
}

```

Play with it in godbolt: https://cpp2.godbolt.org/z/7x3sf9KoW
