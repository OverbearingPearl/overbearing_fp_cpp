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

#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <vector>

#include "src/side_effects/cache/policy.h"

namespace side_effects {
namespace cache {

template <typename KeyType, typename ValueType>
class RRCachePolicy : public CachePolicy<KeyType, ValueType> {
 public:
  explicit RRCachePolicy(size_t capacity) : capacity_(capacity) {}

  void Insert(Cache<KeyType, ValueType>* cache, const KeyType& key,
              std::shared_ptr<ValueType> value) override {
    if (cache->size() >= capacity_) {
      Evict(cache);
    }
    (*cache)[key] = value;
    keys_.push_back(key);
  }

 private:
  void Evict(Cache<KeyType, ValueType>* cache) {
    size_t index = std::rand() % keys_.size();
    KeyType key_to_evict = keys_[index];
    cache->erase(key_to_evict);
    keys_.erase(keys_.begin() + index);
  }

  size_t capacity_;
  std::vector<KeyType> keys_;
};

}  // namespace cache
}  // namespace side_effects
