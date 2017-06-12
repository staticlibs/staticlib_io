/*
 * Copyright 2017, alex at staticlibs.net
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
 * File:   array_sink_test.cpp
 * Author: alex
 *
 * Created on June 11, 2017, 11:14 PM
 */

#include "staticlib/io/array_sink.hpp"

#include <cstdlib>
#include <iostream>

#include "staticlib/config/assert.hpp"

void test_sink() {
    auto sink = sl::io::make_array_sink();
    sink.write({"42"});
    sink.write({"foo"});
    sink.write({"1"});
    auto span = sink.release();
    slassert(6 == span.size());
    slassert('\0' ==  span.data()[span.size()]);
    slassert("42foo1" ==  std::string(span.data()));
    std::free(span.data());
}

int main() {
    try {
        test_sink();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

