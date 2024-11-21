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

#include "src/side_effects/memoization/memoization.h"

int main() {
  side_effects::memoization::Memoization memoization;
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
      }));
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
      }));
  int result = fib(10);
  fib(10);
  fib(11);
  fib1(10);
  fib1(10);
  fib1(11);
  std::cout << "Fibonacci of 10 is " << result << std::endl;

  return 0;
}
