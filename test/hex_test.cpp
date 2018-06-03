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
#include "staticlib/io/hex_operations.hpp"

#include <iostream>

#include "staticlib/config/assert.hpp"

void test_string_to_hex() {
    slassert("666f6f" == sl::io::string_to_hex("foo"));
    slassert("" == sl::io::string_to_hex(""));
}

void test_string_from_hex() {
    slassert("foo" == sl::io::string_from_hex("666f6f"));
    slassert("" == sl::io::string_from_hex(""));
}

void test_format_hex_and_plain() {
    auto hex = std::string("024e4f00002e002e");
    auto plain = sl::io::string_from_hex(hex);
    auto formatted = sl::io::format_hex_and_plain(hex, plain);
    slassert("02 4e 4f 00 00 2e 00 2e [ NO  . .]" == formatted);
    slassert("" == sl::io::format_hex_and_plain("", ""));
}

void test_format_hex() {
    auto formatted = sl::io::format_hex("024e4f00002e002e");
    slassert("02 4e 4f 00 00 2e 00 2e [ NO  . .]" == formatted);
    slassert("" == sl::io::format_hex(""));
}

void test_format_plain_as_hex() {
    auto hex = std::string("024e4f00002e002e");
    auto plain = sl::io::string_from_hex(hex);
    auto formatted = sl::io::format_plain_as_hex(plain);
    slassert("02 4e 4f 00 00 2e 00 2e [ NO  . .]" == formatted);
    slassert("" == sl::io::format_plain_as_hex(""));
}

int main() {
    try {
        test_string_to_hex();
        test_string_from_hex();
        test_format_hex_and_plain();
        test_format_hex();
        test_format_plain_as_hex();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
