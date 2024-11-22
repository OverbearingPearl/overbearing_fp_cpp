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

#ifndef SRC_SIDE_EFFECTS_MEMOIZATION_TUPLE_HASH_H_
#define SRC_SIDE_EFFECTS_MEMOIZATION_TUPLE_HASH_H_

#include <functional>
#include <tuple>

struct TupleHash {
  template <typename... Args>
  std::size_t operator()(const std::tuple<Args...>& t) const {
    return hashTuple(t);
  }

 private:
  template <typename Tuple,
            std::size_t Index = std::tuple_size<Tuple>::value - 1>
  struct Hasher {
    std::size_t operator()(const Tuple& t) const {
      std::size_t hash = Hasher<Tuple, Index - 1>()(t);
      TupleHash::hashCombine(&hash, std::get<Index>(t));
      return hash;
    }
  };

  template <typename Tuple>
  struct Hasher<Tuple, 0> {
    std::size_t operator()(const Tuple& t) const {
      std::size_t hash = 0;
      TupleHash::hashCombine(&hash, std::get<0>(t));
      return hash;
    }
  };

  template <typename Tuple>
  std::size_t hashTuple(const Tuple& t) const {
    return Hasher<Tuple>()(t);
  }

  template <typename T>
  static void hashCombine(std::size_t* seed, const T& value) {
    std::hash<T> hasher;
    *seed ^= hasher(value) + 0x9e3779b9 + (*seed << 6) + (*seed >> 2);
  }
};

struct TupleEqual {
  template <typename... Args>
  bool operator()(const std::tuple<Args...>& t1,
                  const std::tuple<Args...>& t2) const {
    return t1 == t2;
  }
};

#endif  // SRC_SIDE_EFFECTS_MEMOIZATION_TUPLE_HASH_H_
