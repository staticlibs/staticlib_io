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

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/copying_source.hpp"

#include "TwoBytesAtOnceSource.hpp"
#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_copy() {
    io::copying_source<TwoBytesAtOnceSource, TwoBytesAtOnceSink> src{TwoBytesAtOnceSource{"42"}, TwoBytesAtOnceSink{}};
    std::string dest{};
    dest.resize(2);
    auto read = src.read(std::addressof(dest.front()), 2);
    (void) read;
    assert(2 == read);
    assert(2 == dest.size());
    assert("42" == dest);
    assert(2 == src.get_sink().get_data().size());
    assert("42" == src.get_sink().get_data());
}

void test_omit_tail() {
    io::copying_source<TwoBytesAtOnceSource, TwoBytesAtOnceSink> src{TwoBytesAtOnceSource{"42foo"}, TwoBytesAtOnceSink{}};
    std::string dest{};
    dest.resize(2);
    auto read = src.read(std::addressof(dest.front()), 2);
    (void) read;
    assert(2 == read);
    assert(2 == dest.size());
    assert("42" == dest);
    assert(2 == src.get_sink().get_data().size());
    assert("42" == src.get_sink().get_data());
}

int main() {
    test_copy();
    test_omit_tail();

    return 0;
}

