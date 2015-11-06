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

#include <iostream>
#include <sstream>
#include <cassert>

#include "staticlib/io/streambuf_sink.hpp"

namespace io = staticlib::io;

void test_write() {
    std::ostringstream stream{};
    io::streambuf_sink sink{stream.rdbuf()};
    auto written = sink.write("foo", 3);
    (void) written;
    assert(3 == written);
    assert("foo" == stream.str());
}

int main() {
    test_write();

    return 0;
}

