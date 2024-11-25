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

class Memoization {
  template <typename Func, typename CachePolicy>
  struct MemoizedFunc;

 public:
  template <typename ReturnType, typename... Args,
            typename CachePolicy = side_effects::memoization::cache::
                DefaultCachePolicy<std::tuple<Args...>, ReturnType>>
  MemoizedFunc<ReturnType(Args...), CachePolicy> memoize(
      std::function<ReturnType(Args...)> func,
      CachePolicy cache_policy = CachePolicy()) {
    return MemoizedFunc<ReturnType(Args...), CachePolicy>(func, cache_policy);
  }

 private:
  template <typename ReturnType, typename... Args, typename CachePolicy>
  struct MemoizedFunc<ReturnType(Args...), CachePolicy> {
    MemoizedFunc(std::function<ReturnType(Args...)> func,
                 CachePolicy cache_policy = CachePolicy())
        : func_(func), cache_policy_(cache_policy) {}

    ReturnType operator()(Args... args) {
      using KeyType = std::tuple<Args...>;
      using ResultType = ReturnType;

      KeyType key = std::make_tuple(args...);
      auto it = cache_.find(key);
      if (it == cache_.end()) {
        LOG("Cache miss");
        ResultType result = func_(args...);
        cache_policy_.insert(cache_, key, std::make_shared<ResultType>(result));
        return result;
      }
      LOG("Cache hit");
      return *std::static_pointer_cast<ResultType>(it->second);
    }

   private:
    std::function<ReturnType(Args...)> func_;
    CachePolicy cache_policy_;
    side_effects::memoization::cache::Cache<std::tuple<Args...>, ReturnType>
        cache_;
  };
};

}  // namespace memoization
}  // namespace side_effects

#endif  // SRC_SIDE_EFFECTS_MEMOIZATION_MEMOIZATION_H_
