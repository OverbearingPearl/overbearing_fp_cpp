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

#include <algorithm>
#include <list>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "src/side_effects/io/logging.h"
#include "src/side_effects/memoization/cache/policy.h"

namespace side_effects {
namespace memoization {
namespace cache {

template <typename KeyType, typename ValueType>
class LFUCachePolicy : public CachePolicy<KeyType, ValueType> {
 public:
  explicit LFUCachePolicy(size_t capacity) : capacity_(capacity) {
    LOG("LFUCachePolicy capacity: ", capacity_);
  }

  void insert(Cache<KeyType, ValueType>* cache, const KeyType& key,
              std::shared_ptr<ValueType> value) override {
    for (const auto& pair : frequency_list_) {
      LOG("insert()", " capacity: ", capacity_, ", size: ", cache->size(),
          ", Key: ", std::get<0>(pair.first), ", Frequency: ", pair.second);
    }
    if (cache->size() >= capacity_) {
      evict(cache);
    }
    if (frequency_list_.find(key) != frequency_list_.end()) {
      touch(key);
    } else {
      (*cache)[key] = value;
      frequency_list_[key] = 1;
      frequency_order_.push_back(key);
    }
    for (const auto& pair : frequency_list_) {
      LOG("insert()", " capacity: ", capacity_, ", size: ", cache->size(),
          ", Key: ", std::get<0>(pair.first), ", Frequency: ", pair.second);
    }
  }

 private:
  void touch(const KeyType& key) {
    if (frequency_list_.find(key) != frequency_list_.end()) {
      frequency_list_[key]++;
      frequency_order_.remove(key);
      frequency_order_.push_back(key);
    }
  }

  void evict(Cache<KeyType, ValueType>* cache) {
    auto min_freq_it =
        std::min_element(frequency_list_.begin(), frequency_list_.end(),
                         [](const std::pair<KeyType, size_t>& a,
                            const std::pair<KeyType, size_t>& b) {
                           return a.second < b.second;
                         });
    KeyType key_to_evict = min_freq_it->first;
    cache->erase(key_to_evict);
    frequency_list_.erase(key_to_evict);
    frequency_order_.remove(key_to_evict);
    LOG("evict()", " Key: ", std::get<0>(key_to_evict));
  }

  size_t capacity_;
  std::unordered_map<KeyType, size_t, TupleHash, TupleEqual> frequency_list_;
  std::list<KeyType> frequency_order_;
};

}  // namespace cache
}  // namespace memoization
}  // namespace side_effects
