/*
 * Copyright 2017, alex at staticlibs.net
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
 * File:   limited_source_test.cpp
 * Author: alex
 *
 * Created on January 29, 2017, 4:42 PM
 */

#include "staticlib/io/limited_source.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "two_bytes_at_once_source.hpp"
#include "test_utils.hpp"

namespace io = staticlib::io;

void test_limit() {
    auto src = io::make_limited_source(two_bytes_at_once_source{"bar1"}, 3);
    std::array<char, 2> arr;
    auto read = src.read(arr);
    slassert(throws_exc([&src] { src.read({nullptr, 4}); }));
    slassert(2 == read);
    slassert(2 == src.get_count());
    slassert(throws_exc([&src] { src.read({nullptr, 2}); }));
    slassert(!throws_exc([&src, &arr] { src.read({arr.data(), 1}); }));
    slassert(throws_exc([&src, &arr] { src.read({arr.data(), 1}); }));
}

int main() {
    try {
        test_limit();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

