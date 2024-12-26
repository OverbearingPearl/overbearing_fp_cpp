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

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <tuple>

#include "src/side_effects/cache/cache_lfu.h"

TEST(Cache, PolicyLfu_InsertSameKeyTwice_CacheSizeIsOne) {
  side_effects::cache::CacheWithLfuPolicy<std::tuple<std::string>, int>
      lfu_policy(1);
  auto cache = std::make_shared<
      side_effects::cache::Cache<std::tuple<std::string>, int>>();
  lfu_policy.Insert(cache.get(), std::make_tuple("key"),
                    std::make_shared<int>(1));
  lfu_policy.Insert(cache.get(), std::make_tuple("key"),
                    std::make_shared<int>(2));
  EXPECT_EQ(cache->size(), 1);
}
