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
 * File:   hex_test.cpp
 * Author: alex
 *
 * Created on March 20, 2018, 5:18 PM
 */
#include "staticlib/io/hex.hpp"

#include <iostream>

#include "staticlib/config/assert.hpp"

void test_copy_hex() {
    // hello in russian
    std::string plain = "\xd0\xbf\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82";
    auto plain_src = sl::io::string_source(plain);
    auto hex_sink = sl::io::string_sink();
    auto count_to = sl::io::copy_to_hex(plain_src, hex_sink);
    auto hex_src = sl::io::string_source(hex_sink.get_string());
    auto plain_sink = sl::io::string_sink();
    auto count_from = sl::io::copy_from_hex(hex_src, plain_sink);
    slassert(count_to == count_from);
    slassert(plain == plain_sink.get_string());
}

void test_format_hex_and_plain() {
    auto formatted = sl::io::format_hex_and_plain("024e4f00002e002e");
    slassert("02 4e 4f 00 00 2e 00 2e [ NO  . .]" == formatted);
}

int main() {
    try {
        test_copy_hex();
        test_format_hex_and_plain();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
