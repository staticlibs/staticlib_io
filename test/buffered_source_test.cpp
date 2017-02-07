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
 * File:   buffered_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 9:20 AM
 */

#include "staticlib/io/buffered_source.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "negative_read_source.hpp"
#include "two_bytes_at_once_source.hpp"
#include "test_utils.hpp"

namespace io = staticlib::io;

void test_buffered() {
    io::buffered_source<two_bytes_at_once_source, 3> src{two_bytes_at_once_source{"foo42"}};
    std::string dest{};
    dest.resize(3);
    auto read = src.read({std::addressof(dest.front()), 3});
    slassert(3 == read);
    slassert(3 == dest.size());
    slassert("foo" == dest);
}

void test_overread() {
    io::buffered_source<two_bytes_at_once_source, 3> src{two_bytes_at_once_source{"foo42"}};
    std::string dest{};
    dest.resize(5);
    auto read = src.read({std::addressof(dest.front()), 5});
    slassert(5 == read);
    slassert(5 == dest.size());
    slassert("foo42" == dest);
}

void test_make_rvalue() {
    auto src = io::make_buffered_source(two_bytes_at_once_source{"foo42"});
    (void) src;
}

void test_make_lvalue() {
    two_bytes_at_once_source delegate{"foo42"};
    auto src = io::make_buffered_source(delegate);
    (void) src;
}

void test_throw() {
    auto src = io::make_buffered_source(two_bytes_at_once_source{"foo42"});
    slassert(throws_exc([&src] { src.read({nullptr, -1}); }));
    slassert(throws_exc([] {
        auto ns = io::make_buffered_source(negative_read_source());
        std::array<char, 16> buf;
        std::memset(buf.data(), '\0', buf.size());
        ns.read(buf);
    }));
}

int main() {
    try {
        test_buffered();
        test_overread();
        test_make_rvalue();
        test_make_lvalue();
        test_throw();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

