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

#ifndef SRC_SIDE_EFFECTS_MEMOIZATION_CACHE_CACHING_LFU_H_
#define SRC_SIDE_EFFECTS_MEMOIZATION_CACHE_CACHING_LFU_H_

#include <list>
#include <unordered_map>

#include "src/side_effects/memoization/cache/caching.h"

namespace side_effects {
namespace memoization {
namespace cache {

template <typename KeyType, typename ValueType>
class LFUCachePolicy : public CachePolicy<KeyType, ValueType> {
 public:
  LFUCachePolicy(size_t capacity) : capacity_(capacity) {}

  void insert(std::unordered_map<KeyType, std::shared_ptr<ValueType>>& cache,
              const KeyType& key, std::shared_ptr<ValueType> value) override {
    if (cache.size() >= capacity_) {
      evict(cache);
    }
    cache[key] = value;
    frequency_list_[key] = 1;
    frequency_order_.push_back(key);
  }

 private:
  void evict(std::unordered_map<KeyType, std::shared_ptr<ValueType>>& cache) {
    auto min_freq_it = std::min_element(
        frequency_list_.begin(), frequency_list_.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    KeyType key_to_evict = min_freq_it->first;
    cache.erase(key_to_evict);
    frequency_list_.erase(key_to_evict);
    frequency_order_.remove(key_to_evict);
  }

  size_t capacity_;
  std::unordered_map<KeyType, size_t> frequency_list_;
  std::list<KeyType> frequency_order_;
};

}  // namespace cache
}  // namespace memoization
}  // namespace side_effects

#endif  // SRC_SIDE_EFFECTS_MEMOIZATION_CACHE_CACHING_LFU_H_
