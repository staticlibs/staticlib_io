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
 * File:   streambuf_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:29 PM
 */

#include "staticlib/io/streambuf_sink.hpp"

#include <iostream>
#include <sstream>

#include "staticlib/config/assert.hpp"

namespace io = staticlib::io;

void test_write() {
    std::ostringstream stream{};
    io::streambuf_sink sink{stream.rdbuf()};
    auto written = sink.write({"foo", 3});
    slassert(3 == written);
    slassert("foo" == stream.str());
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
