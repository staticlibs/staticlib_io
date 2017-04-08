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
 * File:   reference_sink_test.cpp
 * Author: alex
 *
 * Created on December 13, 2015, 2:32 PM
 */

#include "staticlib/io/reference_sink.hpp"

#include <iostream>

#include "staticlib/config/assert.hpp"

#include "staticlib/io/counting_sink.hpp"

#include "non_copyable_sink.hpp"

void test_ref() {
    non_copyable_sink nc_sink{};
    auto sink = sl::io::make_reference_sink(nc_sink);
    auto wrapped = sl::io::make_counting_sink(std::move(sink));
    auto written = wrapped.write({"foo", 3});
    slassert(3 == written);
    slassert(3 == wrapped.get_count());
    slassert(3 == wrapped.get_sink().get_sink().get_count());
}

int main() {
    try {
        test_ref();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}


