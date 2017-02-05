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
 * File:   string_sink_test.cpp
 * Author: alex
 *
 * Created on November 6, 2015, 9:11 PM
 */

#include "staticlib/io/string_sink.hpp"

#include <array>
#include <iostream>

#include "staticlib/config/assert.hpp"

#include "test_utils.hpp"

namespace io = staticlib::io;

void test_write() {
    io::string_sink sink{};
    auto res = sink.write({"bar", 3});
    slassert(3 == res);
    slassert(sink.get_string() == "bar");
    slassert(throws_exc([&sink] { sink.write({nullptr, -1}); }))
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
