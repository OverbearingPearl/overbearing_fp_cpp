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

#include <list>
#include <memory>
#include <unordered_map>

#include "src/side_effects/memoization/cache/policy.h"

namespace side_effects {
namespace memoization {
namespace cache {

template <typename KeyType, typename ValueType>
class LRUCachePolicy : public CachePolicy<KeyType, ValueType> {
 public:
  explicit LRUCachePolicy(size_t capacity) : capacity_(capacity) {}

  void insert(Cache<KeyType, ValueType>* cache, const KeyType& key,
              std::shared_ptr<ValueType> value) override {
    if (cache->size() >= capacity_) {
      evict(cache);
    }
    if (key_iterator_map_.find(key) != key_iterator_map_.end()) {
      access_order_.erase(key_iterator_map_[key]);
    }
    (*cache)[key] = value;
    access_order_.push_front(key);
    key_iterator_map_[key] = access_order_.begin();
  }

 private:
  void evict(Cache<KeyType, ValueType>* cache) {
    KeyType key_to_evict = access_order_.back();
    cache->erase(key_to_evict);
    key_iterator_map_.erase(key_to_evict);
    access_order_.pop_back();
  }

  size_t capacity_;
  std::list<KeyType> access_order_;
  std::unordered_map<KeyType, typename std::list<KeyType>::iterator>
      key_iterator_map_;
};

}  // namespace cache
}  // namespace memoization
}  // namespace side_effects
