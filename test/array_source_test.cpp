/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   array_source_test.cpp
 * Author: alex
 *
 * Created on November 6, 2015, 8:56 PM
 */

#include "staticlib/io/array_source.hpp"

#include <array>
#include <iostream>

#include "staticlib/config/assert.hpp"

#include "test_utils.hpp"

namespace io = staticlib::io;

void test_read() {
    std::array<char, 3> arr = {{'b', 'a', 'r'}};
    io::array_source src(arr.data(), arr.size());
    std::array<char, 4> out;
    auto res = src.read(out.data(), out.size());
    slassert(3 == res);
    slassert('b' == out[0]);
    slassert('a' == out[1]);
    slassert('r' == out[2]);
    slassert(throws_exc([&src] { src.read(nullptr, -1); }))
}

int main() {
    try {
        test_read();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

