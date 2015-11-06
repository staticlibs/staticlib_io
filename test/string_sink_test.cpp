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

#include <array>
#include <cassert>

#include "staticlib/io/string_sink.hpp"

namespace io = staticlib::io;

void test_write() {
    io::string_sink sink{};
    auto res = sink.write("bar", 3);
    (void) res;
    assert(3 == res);
    assert(sink.get_string() == "bar");
}

int main() {
    test_write();
    
    return 0;
}

