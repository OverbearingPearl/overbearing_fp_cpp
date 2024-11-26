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

#include <chrono>
#include <memory>
#include <unordered_map>

#include "src/side_effects/memoization/cache/policy.h"

namespace side_effects {
namespace memoization {
namespace cache {

template <typename KeyType, typename ValueType>
class TTLCachePolicy : public CachePolicy<KeyType, ValueType> {
 public:
  explicit TTLCachePolicy(std::chrono::milliseconds ttl) : ttl_(ttl) {}

  void Insert(Cache<KeyType, ValueType>* cache, const KeyType& key,
              std::shared_ptr<ValueType> value) override {
    auto now = std::chrono::steady_clock::now();
    (*cache)[key] = value;
    timestamps_[key] = now;
    CleanUp(cache);
  }

 private:
  void CleanUp(Cache<KeyType, ValueType>* cache) {
    auto now = std::chrono::steady_clock::now();
    for (auto it = timestamps_.begin(); it != timestamps_.end();) {
      if (now - it->second > ttl_) {
        cache->erase(it->first);
        it = timestamps_.erase(it);
      } else {
        ++it;
      }
    }
  }

  std::chrono::milliseconds ttl_;
  std::unordered_map<KeyType, std::chrono::steady_clock::time_point>
      timestamps_;
};

}  // namespace cache
}  // namespace memoization
}  // namespace side_effects
