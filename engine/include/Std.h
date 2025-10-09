#pragma once

#define USE_STD_HEADERS

#if defined(USE_STD_HEADERS)

#include <cassert>
#include <algorithm>
#include <memory>
#include <stdexcept>

#include <queue>
#include <stack>
#include <iterator>
#include <random>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <array>
#include <map>

using String = std::string;
using StringView = std::string_view;
template <typename T> using Hash = std::hash<T>;

//Trying to use heterogeneous lookup for unordered_map to avoid string allocations
//https://www.cppstories.com/2021/heterogeneous-access-cpp20/
struct StringHash {
	using is_transparent = void;
	[[nodiscard]] size_t operator()(const char* txt) const {
		return Hash<StringView>{}(txt);
	}
	[[nodiscard]] size_t operator()(StringView txt) const {
		return Hash<StringView>{}(txt);
	}
	[[nodiscard]] size_t operator()(const String& txt) const {
		return Hash<String>{}(txt);
	}
};

template <typename T> using SharedPtr = std::shared_ptr<T>;
template <typename T> using UniquePtr = std::unique_ptr<T>;
template <typename T> using Vector = std::vector<T>;
template <typename T, size_t S> using Array = std::array<T, S>;
template <typename V> using UnorderedMapStringKey = std::unordered_map<String, V, StringHash, std::equal_to<>>;
template <typename K, typename V> using UnorderedMap = std::unordered_map < K, V >;
template <typename K, typename V> using MultiMap = std::multimap < K, V >;
template <typename T> using Set = std::set<T>;
template <typename K, typename V, typename C> using PriorityQueue = std::priority_queue<K, V, C>;

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
	T* ptr = new T(std::forward<Args>(args)...);
	return SharedPtr<T>(ptr);
}

template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args) {
	T* ptr = new T(std::forward<Args>(args)...);
	return UniquePtr<T>(ptr);
}

#endif