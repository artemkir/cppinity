#pragma once

#include <algorithm>
#include <queue>
#include <memory>
#include <stack>
#include <iterator>
#include <random>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <array>
#include <map>
#include <cassert>

using String = std::string;

//Trying to use heterogeneous lookup for unordered_map to avoid string allocations
//https://www.cppstories.com/2021/heterogeneous-access-cpp20/
struct StringHash {
    using is_transparent = void;
    [[nodiscard]] size_t operator()(const char* txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(std::string_view txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(const String& txt) const {
        return std::hash<String>{}(txt);
    }
};

template <typename T> using SharedPtr = std::shared_ptr<T>;
template <typename T> using UniquePtr = std::unique_ptr<T>;
template <typename T> using Vector = std::vector<T>;
template <typename T, size_t S> using Array = std::array<T,S>;
template <typename V> using UnorderedMapStringKey = std::unordered_map<String, V, StringHash, std::equal_to<>>;
template <typename K, typename V> using UnorderedMap = std::unordered_map < K, V >;
template <typename K, typename V> using MultiMap = std::multimap < K, V >;



