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

#include <gtest/gtest.h>

#include <functional>

#include "src/side_effects/memoization/cache/policy_lfu.h"
#include "src/side_effects/memoization/memoization.h"

TEST(Memoization, LfuCache_CaclulateFibonacciOnce_Normally) {
  side_effects::memoization::Memoization memoization;
  auto lfu_policy =
      side_effects::memoization::cache::LFUCachePolicy<std::tuple<int>, int>(4);
  auto fib = memoization.Memoize(std::function<int(int)>([](int n) -> int {
                                   if (n < 2) {
                                     return n;
                                   }
                                   int a = 0, b = 1, c;
                                   for (int i = 2; i <= n; ++i) {
                                     c = a + b;
                                     a = b;
                                     b = c;
                                   }
                                   return b;
                                 }),
                                 lfu_policy);

  SCOPED_TRACE("Testing fib(10)");
  EXPECT_EQ(fib(10), 55);
}

TEST(Memoization, LfuCache_CalculateFibonacciMultiply_Normally) {
  side_effects::memoization::Memoization memoization;
  auto lfu_policy =
      side_effects::memoization::cache::LFUCachePolicy<std::tuple<int>, int>(4);
  auto fib = memoization.Memoize(std::function<int(int)>([](int n) -> int {
                                   if (n < 2) {
                                     return n;
                                   }
                                   int a = 0, b = 1, c;
                                   for (int i = 2; i <= n; ++i) {
                                     c = a + b;
                                     a = b;
                                     b = c;
                                   }
                                   return b;
                                 }),
                                 lfu_policy);

  SCOPED_TRACE("Testing fib(10)");
  EXPECT_EQ(fib(10), 55);
  EXPECT_EQ(fib(10), 55);  // Should hit cache

  SCOPED_TRACE("Testing fib(11)");
  EXPECT_EQ(fib(11), 89);

  SCOPED_TRACE("Testing fib(12)");
  EXPECT_EQ(fib(12), 144);

  SCOPED_TRACE("Testing fib(13)");
  EXPECT_EQ(fib(13), 233);

  SCOPED_TRACE("Testing fib(10) again");
  EXPECT_EQ(fib(10), 55);  // Should hit cache

  SCOPED_TRACE("Testing fib(12) again");
  EXPECT_EQ(fib(12), 144);  // Should hit cache

  SCOPED_TRACE("Testing fib(13) again");
  EXPECT_EQ(fib(13), 233);  // Should hit cache

  SCOPED_TRACE("Testing fib(14)");
  EXPECT_EQ(fib(14), 377);

  SCOPED_TRACE("Testing fib(13) again");
  EXPECT_EQ(fib(13), 233);  // Should hit cache
}
