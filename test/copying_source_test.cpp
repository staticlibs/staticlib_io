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
 * File:   copying_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 12:48 PM
 */

#include "staticlib/io/copying_source.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "TwoBytesAtOnceSource.hpp"
#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_copy() {
    auto src = io::make_copying_source(TwoBytesAtOnceSource{"42"}, TwoBytesAtOnceSink{});
    std::string dest{};
    dest.resize(2);
    auto read = src.read(dest);
    slassert(2 == read);
    slassert(2 == dest.size());
    slassert("42" == dest);
    slassert(2 == src.get_sink().get_data().size());
    slassert("42" == src.get_sink().get_data());
}

void test_omit_tail() {
    auto src = io::make_copying_source(TwoBytesAtOnceSource{"42foo"}, TwoBytesAtOnceSink{});
    std::string dest{};
    dest.resize(2);
    auto read = src.read(dest);
    slassert(2 == read);
    slassert(2 == dest.size());
    slassert("42" == dest);
    slassert(2 == src.get_sink().get_data().size());
    slassert("42" == src.get_sink().get_data());
}

void test_lvalue() {
    TwoBytesAtOnceSource s1{"42foo"};
    TwoBytesAtOnceSink s2{};
    auto src = io::make_copying_source(s1, s2);
    (void) src;
}

int main() {
    try {
        test_copy();
        test_omit_tail();
        test_lvalue();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

