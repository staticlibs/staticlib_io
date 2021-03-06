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
 * File:   hex_source_test.cpp
 * Author: alex
 *
 * Created on June 3, 2018, 2:41 PM
 */

#include "staticlib/io/hex_source.hpp"
#include "staticlib/io/operations.hpp"

#include <iostream>

#include "staticlib/config/assert.hpp"

void test_source() {
    // hello in russian
    auto src = sl::io::make_hex_source(sl::io::string_source("d0bfd180d0b8d0b2d0b5d182"));
    auto sink = sl::io::string_sink();
    sl::io::copy_all(src, sink);
    slassert("\xd0\xbf\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82" == sink.get_string());
}

int main() {
    try {
        test_source();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
