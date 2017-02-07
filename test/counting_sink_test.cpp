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
 * File:   counting_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:07 PM
 */

#include "staticlib/io/counting_sink.hpp"

#include <array>
#include <iostream>
#include <string>

#include "staticlib/config/assert.hpp"

#include "two_bytes_at_once_sink.hpp"

namespace io = staticlib::io;

void test_count() {
    auto sink = io::make_counting_sink(two_bytes_at_once_sink{});
    auto written = sink.write({"42", 2});
    slassert(2 == written);
    slassert(2 == sink.get_count());
}

void test_count_overflow() {
    two_bytes_at_once_sink delegate{};
    auto sink = io::make_counting_sink(delegate);
    auto written = sink.write({"foo", 3});
    slassert(2 == written);
    slassert(2 == sink.get_count());
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

