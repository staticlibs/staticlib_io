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
 * File:   operations_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:23 PM
 */

#include "staticlib/io/operations.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "two_bytes_at_once_source.hpp"
#include "two_bytes_at_once_sink.hpp"

void test_write_not_all() {
    two_bytes_at_once_sink sink{};
    auto written = sink.write({"abc", 3});
    slassert(2 == written);
    slassert(2 == sink.get_data().length());
    slassert("ab" == sink.get_data());
}

void test_write_all_buffer() {
    two_bytes_at_once_sink sink{};
    sl::io::write_all(sink, {"abc", 3});
    slassert(3 == sink.get_data().length());
    slassert("abc" == sink.get_data());
}

void test_write_all_str() {
    two_bytes_at_once_sink sink{};
    std::string data{"abc"};
    sl::io::write_all(sink, data);
    slassert(3 == sink.get_data().length());
    slassert("abc" == sink.get_data());
}

void test_read_not_all() {
    two_bytes_at_once_source src{"abc"};
    std::array<char, 4> buf;
    auto read = src.read({buf.data(), 3});
    slassert(2 == read);
    std::string res{buf.data(), static_cast<size_t>(read)};
    slassert(2 == res.length());
    slassert("ab" == res);
}

void test_read_all() {
    two_bytes_at_once_source src{"abc"};
    std::array<char, 4> buf;
    auto read = sl::io::read_all(src, {buf.data(), 3});
    slassert(3 == read);
    std::string res{buf.data(), read};
    slassert(3 == res.length());
    slassert("abc" == res);
}

void test_read_exact() {
    two_bytes_at_once_source src{"abc"};
    std::array<char, 5> buf;
    bool thrown = false;
    try {
        sl::io::read_exact(src, {buf.data(), 4});
    } catch (const sl::io::io_exception&) {
        thrown = true;
    }
    slassert(thrown);
}

void test_copy() {
    two_bytes_at_once_sink sink{};
    two_bytes_at_once_source src{"abc"};
    std::array<char, 2> buf;
    auto copied = sl::io::copy_all(src, sink, buf);
    slassert(3 == copied);
    slassert(3 == sink.get_data().length());
    slassert("abc" == sink.get_data());
}

void test_copy_stackbuf() {
    two_bytes_at_once_sink sink{};
    two_bytes_at_once_source src{"abc"};
    auto copied = sl::io::copy_all(src, sink);
    slassert(3 == copied);
    slassert(3 == sink.get_data().length());
    slassert("abc" == sink.get_data());
}

void test_skip() {
    two_bytes_at_once_source src{"abc"};
    std::array<char, 1> buf;
    sl::io::skip(src, {buf.data(), 1}, 2);
    auto read = src.read({buf.data(), 1});
    slassert(1 == read);
    slassert('c' == buf[0]);
}

int main() {
    try {
        test_write_not_all();
        test_write_all_buffer();
        test_write_all_str();
        test_read_not_all();
        test_read_all();
        test_read_exact();
        test_copy();
        test_copy_stackbuf();
        test_skip();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

