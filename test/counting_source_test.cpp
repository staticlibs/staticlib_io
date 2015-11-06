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

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/counting_source.hpp"

#include "TwoBytesAtOnceSource.hpp"

namespace io = staticlib::io;

void test_count() {
    io::counting_source<TwoBytesAtOnceSource> src{TwoBytesAtOnceSource{"42"}};
    std::array<char, 2> arr;
    auto read = src.read(arr.data(), arr.size());
    (void) read;
    assert(2 == read);
    assert(2 == src.get_count());
}

void test_count_overflow() {
    io::counting_source<TwoBytesAtOnceSource> src{TwoBytesAtOnceSource{"foo42"}};
    std::array<char, 5> arr;
    auto read = src.read(arr.data(), arr.size());
    (void) read;
    assert(2 == read);
    assert(2 == src.get_count());
}

int main() {
    test_count();
    test_count_overflow();

    return 0;
}

