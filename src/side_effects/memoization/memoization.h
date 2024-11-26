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

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "src/side_effects/io/logging.h"
#include "src/side_effects/memoization/cache/policy.h"

namespace side_effects {
namespace memoization {

template <typename Func>
struct function_traits;

template <typename ReturnType, typename... Args>
struct function_traits<std::function<ReturnType(Args...)>> {
  using result_type = ReturnType;
  using arg_tuple_type = std::tuple<Args...>;
};

template <typename ReturnType, typename ClassType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...)> {
  using result_type = ReturnType;
  using arg_tuple_type = std::tuple<ClassType*, Args...>;
};

template <typename Func>
struct default_cache_policy;

template <typename ReturnType, typename... Args>
struct default_cache_policy<std::function<ReturnType(Args...)>> {
  using type =
      side_effects::memoization::cache::DefaultCachePolicy<std::tuple<Args...>,
                                                           ReturnType>;
};

template <typename ReturnType, typename ClassType, typename... Args>
struct default_cache_policy<std::function<ReturnType(ClassType*, Args...)>> {
  using type = side_effects::memoization::cache::DefaultCachePolicy<
      std::tuple<ClassType*, Args...>, ReturnType>;
};

class Memoization {
  template <typename Func, typename CachePolicy>
  struct MemoizedFunc;

 public:
  template <typename Func,
            typename CachePolicy = typename default_cache_policy<Func>::type>
  MemoizedFunc<Func, CachePolicy> memoize(
      Func func, CachePolicy cache_policy = CachePolicy()) {
    return MemoizedFunc<Func, CachePolicy>(func, cache_policy);
  }

  template <typename ReturnType, typename ClassType, typename... Args,
            typename CachePolicy = typename default_cache_policy<
                std::function<ReturnType(ClassType*, Args...)>>::type>
  MemoizedFunc<std::function<ReturnType(ClassType*, Args...)>, CachePolicy>
  memoize(ReturnType (ClassType::*func)(Args...),
          CachePolicy cache_policy = CachePolicy()) {
    return MemoizedFunc<std::function<ReturnType(ClassType*, Args...)>,
                        CachePolicy>(
        [func](ClassType* obj, Args... args) { return (obj->*func)(args...); },
        cache_policy);
  }

 private:
  template <typename Func, typename CachePolicy>
  struct MemoizedFunc {
    using ReturnType = typename function_traits<Func>::result_type;
    using ArgTupleType = typename function_traits<Func>::arg_tuple_type;

    MemoizedFunc(Func func, CachePolicy cache_policy = CachePolicy())
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
        cache_policy_.insert(&cache_, key,
                             std::make_shared<ResultType>(result));
        return result;
      }
      LOG("Cache hit");
      const auto value = it->second;
      cache_policy_.insert(&cache_, key, value);
      return *std::static_pointer_cast<ResultType>(value);
    }

   private:
    Func func_;
    CachePolicy cache_policy_;
    side_effects::memoization::cache::Cache<ArgTupleType, ReturnType> cache_;
    std::shared_ptr<std::mutex> mutex_;
  };
};

}  // namespace memoization
}  // namespace side_effects
