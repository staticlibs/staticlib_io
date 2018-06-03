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
 * File:   hex_sink_test.cpp
 * Author: alex
 *
 * Created on June 3, 2018, 2:28 PM
 */

#include "staticlib/io/hex_sink.hpp"
#include "staticlib/io/operations.hpp"

#include <iostream>

#include "staticlib/config/assert.hpp"

void test_sink() {
    // hello in russian
    auto src = sl::io::string_source("\xd0\xbf\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82");
    auto dest_sink = sl::io::string_sink();
    auto sink = sl::io::make_hex_sink(dest_sink);
    sl::io::copy_all(src, sink);
    slassert("d0bfd180d0b8d0b2d0b5d182" == dest_sink.get_string());
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
