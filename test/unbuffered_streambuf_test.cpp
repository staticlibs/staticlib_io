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
 * File:   unbuffered_streambuf_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:47 PM
 */

#include "staticlib/io/unbuffered_streambuf.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "two_bytes_at_once_source.hpp"
#include "two_bytes_at_once_sink.hpp"

namespace io = staticlib::io;

void test_unbuffered_source() {
    two_bytes_at_once_source src{"abc"};
    auto istreambuf = io::make_unbuffered_istreambuf(std::move(src));
    std::istream stream{std::addressof(istreambuf)};
    // pass it somewhere through std api
    std::array<char, 3> buf;
    auto read = stream.rdbuf()->sgetn(buf.data(), 3);
    slassert(2 == read);
    std::string res{buf.data(), static_cast<size_t>(read)};
    slassert("ab" == res);
}

void test_unbuffered_source_lvalue() {
    two_bytes_at_once_source src{"abc"};
    auto istreambuf = io::make_unbuffered_istreambuf(src);
    std::istream stream{std::addressof(istreambuf)};
    // pass it somewhere through std api
    std::array<char, 3> buf;
    auto read = stream.rdbuf()->sgetn(buf.data(), 3);
    slassert(2 == read);
    std::string res{buf.data(), static_cast<size_t> (read)};
    slassert("ab" == res);
}

void test_unbuffered_sink() {
    two_bytes_at_once_sink sink{};
    auto ostreambuf = io::make_unbuffered_ostreambuf(std::move(sink));
    std::ostream stream{std::addressof(ostreambuf)};
    // pass it somewhere through std api
    auto written = stream.rdbuf()->sputn("abc", 3);
    slassert(2 == written);
    slassert("ab" == ostreambuf.get_sink().get_data());
}

void test_unbuffered_sink_lvalue() {
    two_bytes_at_once_sink sink{};
    auto ostreambuf = io::make_unbuffered_ostreambuf(sink);
    std::ostream stream{std::addressof(ostreambuf)};
    // pass it somewhere through std api
    auto written = stream.rdbuf()->sputn("abc", 3);
    slassert(2 == written);
    slassert("ab" == ostreambuf.get_sink().get_sink().get_data());
}

int main() {
    try {
        test_unbuffered_source();
        test_unbuffered_source_lvalue();
        test_unbuffered_sink();
        test_unbuffered_sink_lvalue();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
