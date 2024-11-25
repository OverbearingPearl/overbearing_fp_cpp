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

#include <iostream>
#include <sstream>
#include <utility>

template <typename T>
void log_helper(std::ostringstream& oss, T&& arg) {
  oss << std::forward<T>(arg);
}

template <typename T, typename... Args>
void log_helper(std::ostringstream& oss, T&& arg, Args&&... args) {
  oss << std::forward<T>(arg);
  log_helper(oss, std::forward<Args>(args)...);
}

template <typename... Args>
void log_impl(Args&&... args) {
  std::ostringstream oss;
  log_helper(oss, std::forward<Args>(args)...);
  std::cout << oss.str() << std::endl;
}

#ifdef LOGGING_ENABLED
#define LOG(...) log_impl(__VA_ARGS__)
#else
#define LOG(...)
#endif
