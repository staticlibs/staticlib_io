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
 * File:   counting_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:12 PM
 */

#include "staticlib/io/counting_source.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "two_bytes_at_once_source.hpp"

namespace io = staticlib::io;

void test_count() {
    auto src = io::make_counting_source(two_bytes_at_once_source{"42"});
    std::array<char, 2> arr;
    auto read = src.read(arr);
    slassert(2 == read);
    slassert(2 == src.get_count());
}

void test_count_overflow() {
    two_bytes_at_once_source delegate{"foo42"};
    auto src = io::make_counting_source(delegate);
    std::array<char, 5> arr;
    auto read = src.read(arr);
    slassert(2 == read);
    slassert(2 == src.get_count());
}

int main() {
    try {
        test_count();
        test_count_overflow();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

