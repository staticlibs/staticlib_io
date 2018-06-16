/*
 * Copyright 2018, alex at staticlibs.net
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
 * File:   memory_sink_test.cpp
 * Author: alex
 *
 * Created on June 8, 2018, 9:18 PM
 */

#include "staticlib/io/memory_sink.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "staticlib/io/operations.hpp"
#include "staticlib/io/string_source.hpp"

void test_write() {
    // write
    auto dest = std::array<char, 3>();
    auto span = sl::io::make_span(dest);
    auto sink = sl::io::memory_sink(span);
    auto src = sl::io::string_source(std::string("foo"));
    auto buf = std::array<char, 2>();
    auto written = sl::io::copy_all(src, sink, buf);
    slassert(3 == written);
    slassert("foo" == std::string(dest.data(), dest.size()));

    // overflow
    bool thrown = false;
    try {
        char ch = 'a';
        sink.write({std::addressof(ch), 1});
    } catch(const sl::io::io_exception& e) {
        thrown = true;
    }
    slassert(thrown);
}

int main() {
    try {
        test_write();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
