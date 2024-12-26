/*
 * Copyright (C) 2024  OverbearingPearl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "src/side_effects/cache/cache.h"
#include "src/side_effects/io/logging.h"
#include "src/utils/traits/func_traits.h"

namespace side_effects {
namespace memoization {

template <typename Func>
struct CacheWithNoPolicy;

template <typename ReturnType, typename... Args>
struct CacheWithNoPolicy<std::function<ReturnType(Args...)>> {
  using type =
      side_effects::cache::CacheWithNoPolicy<std::tuple<Args...>, ReturnType>;
};

template <typename ReturnType, typename ClassType, typename... Args>
struct CacheWithNoPolicy<std::function<ReturnType(ClassType*, Args...)>> {
  using type =
      side_effects::cache::CacheWithNoPolicy<std::tuple<ClassType*, Args...>,
                                             ReturnType>;
};

class Memoization {
  template <typename Func, typename Insertable>
  struct MemoizedFunc;

 public:
  template <typename Func,
            typename Insertable = typename CacheWithNoPolicy<Func>::type>
  MemoizedFunc<Func, Insertable> Memoize(
      Func func, Insertable cache_policy = Insertable()) {
    return MemoizedFunc<Func, Insertable>(func, cache_policy);
  }

  template <typename ReturnType, typename ClassType, typename... Args,
            typename Insertable = typename CacheWithNoPolicy<
                std::function<ReturnType(ClassType*, Args...)>>::type>
  MemoizedFunc<std::function<ReturnType(ClassType*, Args...)>, Insertable>
  Memoize(ReturnType (ClassType::*func)(Args...),
          Insertable cache_policy = Insertable()) {
    return MemoizedFunc<std::function<ReturnType(ClassType*, Args...)>,
                        Insertable>(
        [func](ClassType* obj, Args... args) { return (obj->*func)(args...); },
        cache_policy);
  }

 private:
  template <typename Func, typename Insertable>
  struct MemoizedFunc {
    using ReturnType =
        typename utils::traits::FunctionTraits<Func>::result_type;
    using ArgTupleType =
        typename utils::traits::FunctionTraits<Func>::arg_tuple_type;

    explicit MemoizedFunc(Func func, Insertable cache_policy = Insertable())
        : func_(func),
          cache_policy_(cache_policy),
          mutex_(std::make_shared<std::mutex>()) {}

    template <typename... Args>
    ReturnType operator()(Args... args) {
      std::lock_guard<std::mutex> lock(*mutex_);

      using KeyType = std::tuple<Args...>;
      using ResultType = ReturnType;

      KeyType key = std::make_tuple(args...);
      auto it = cache_.find(key);
      if (it == cache_.end()) {
        LOG("Cache miss");
        ResultType result = func_(args...);
        cache_policy_.Insert(&cache_, key,
                             std::make_shared<ResultType>(result));
        return result;
      }
      LOG("Cache hit");
      const auto value = it->second;
      cache_policy_.Insert(&cache_, key, value);
      return *std::static_pointer_cast<ResultType>(value);
    }

   private:
    Func func_;
    Insertable cache_policy_;
    side_effects::cache::Cache<ArgTupleType, ReturnType> cache_;
    std::shared_ptr<std::mutex> mutex_;
  };
};

}  // namespace memoization
}  // namespace side_effects
