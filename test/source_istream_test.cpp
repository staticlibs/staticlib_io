/*
 * Copyright 2016, alex at staticlibs.net
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
 * File:   source_istream_test.cpp
 * Author: alex
 *
 * Created on April 21, 2016, 4:34 PM
 */

#include "staticlib/io/source_istream.hpp"

#include <array>
#include <iostream>

#include "staticlib/config/assert.hpp"

#include "two_bytes_at_once_source.hpp"

void test_istream() {
    two_bytes_at_once_source src{"abc"};
    auto istream = sl::io::make_source_istream_ptr(src);
    std::streambuf* sb = istream->rdbuf();
    std::array<char, 16> buf;
    auto read = sb->sgetn(buf.data(), buf.size());
    slassert(2 == read);
    slassert('a' == buf[0]);
    slassert('b' == buf[1]);
    auto read2 = sb->sgetn(buf.data(), buf.size());
    slassert(1 == read2);
    slassert('c' == buf[0]);
}

int main() {
    try {
        test_istream();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

