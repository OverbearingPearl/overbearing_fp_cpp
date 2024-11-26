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

#include <functional>
#include <iostream>
#include <tuple>

#include "src/side_effects/memoization/cache/policy_lfu.h"
#include "src/side_effects/memoization/memoization.h"

class Fibonacci {
 public:
  int compute(int n) {
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
  }
};

int main() {
  side_effects::memoization::Memoization memoization;
  auto lfu_policy =
      side_effects::memoization::cache::LFUCachePolicy<std::tuple<int>, int>(4);
  auto fib =
      memoization.memoize<int, int>(std::function<int(int)>([](int n) -> int {
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
  auto fib1 =
      memoization.memoize<int, int>(std::function<int(int)>([](int n) -> int {
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
  Fibonacci fibObj;
  auto memoizedFib = memoization.memoize(
      &Fibonacci::compute, side_effects::memoization::cache::LFUCachePolicy<
                               std::tuple<Fibonacci*, int>, int>(4));
  int result = memoizedFib(&fibObj, 10);
  std::cout << "Memoized Fibonacci of 10 is " << result << std::endl;
  result = fib(10);
  fib(10);
  fib(11);
  std::cout << "Fibonacci of 10 is " << result << std::endl;

  std::cout << "Begin:" << std::endl;
  std::cout << "fib(10)" << std::endl;
  fib1(10);
  std::cout << "fib(11)" << std::endl;
  fib1(11);
  std::cout << "fib(12)" << std::endl;
  fib1(12);
  std::cout << "fib(13)" << std::endl;
  fib1(13);
  std::cout << "fib(10)" << std::endl;
  fib1(10);
  std::cout << "fib(12)" << std::endl;
  fib1(12);
  std::cout << "fib(13)" << std::endl;
  fib1(13);
  std::cout << "fib(14)" << std::endl;
  fib1(14);
  std::cout << "fib(13)" << std::endl;
  fib1(13);
  return 0;
}
