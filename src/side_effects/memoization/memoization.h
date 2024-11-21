/*
 Copyright (c) 2024 OverbearingPearl

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

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

#ifndef SRC_SIDE_EFFECTS_MEMOIZATION_MEMOIZATION_H_
#define SRC_SIDE_EFFECTS_MEMOIZATION_MEMOIZATION_H_

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "src/side_effects/io/logging.h"

namespace side_effects {
namespace memoization {

struct TupleHash {
  template <typename... Args>
  std::size_t operator()(const std::tuple<Args...>& t) const {
    return hashTuple(t);
  }

 private:
  template <typename Tuple,
            std::size_t Index = std::tuple_size<Tuple>::value - 1>
  struct Hasher {
    std::size_t operator()(const Tuple& t) const {
      std::size_t hash = Hasher<Tuple, Index - 1>()(t);
      TupleHash::hashCombine(&hash, std::get<Index>(t));
      return hash;
    }
  };

  template <typename Tuple>
  struct Hasher<Tuple, 0> {
    std::size_t operator()(const Tuple& t) const {
      std::size_t hash = 0;
      TupleHash::hashCombine(&hash, std::get<0>(t));
      return hash;
    }
  };

  template <typename Tuple>
  std::size_t hashTuple(const Tuple& t) const {
    return Hasher<Tuple>()(t);
  }

  template <typename T>
  static void hashCombine(std::size_t* seed, const T& value) {
    std::hash<T> hasher;
    *seed ^= hasher(value) + 0x9e3779b9 + (*seed << 6) + (*seed >> 2);
  }
};

struct TupleEqual {
  template <typename... Args>
  bool operator()(const std::tuple<Args...>& t1,
                  const std::tuple<Args...>& t2) const {
    return t1 == t2;
  }
};

class Memoization {
 public:
  template <typename ResultType, typename... Args>
  auto memoize(std::function<ResultType(Args...)> func)
      -> std::function<ResultType(Args...)> {
    return [this, func](Args... args) -> ResultType {
      using KeyType = std::tuple<Args...>;

      KeyType key = std::make_tuple(args...);
      auto& cache = getCache<KeyType, ResultType>();
      auto it = cache.find(key);
      if (it == cache.end()) {
        LOG("Cache miss");
        ResultType result = func(args...);
        cache[key] = std::make_shared<ResultType>(result);
        return result;
      }
      LOG("Cache hit");
      return *std::static_pointer_cast<ResultType>(it->second);
    };
  }

 private:
  template <typename KeyType, typename ResultType>
  using CacheType = std::unordered_map<KeyType, std::shared_ptr<ResultType>,
                                       TupleHash, TupleEqual>;

  template <typename KeyType, typename ResultType>
  CacheType<KeyType, ResultType>& getCache() const {
    static CacheType<KeyType, ResultType> cache;
    return cache;
  }
};

}  // namespace memoization
}  // namespace side_effects

#endif  // SRC_SIDE_EFFECTS_MEMOIZATION_MEMOIZATION_H_
