/*
 * Copyright 2016, alex at staticlibs.net
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
 * File:   replacer_source_test.cpp
 * Author: alex
 *
 * Created on November 5, 2016, 8:25 PM
 */

#include "staticlib/io/replacer_source.hpp"

#include <array>
#include <iostream>
#include <map>
#include <string>

#include "staticlib/config/assert.hpp"

#include "staticlib/io/operations.hpp"
#include "staticlib/io/string_sink.hpp"

#include "two_bytes_at_once_source.hpp"

void test_replace() {
    auto src = two_bytes_at_once_source("fox{{abc}}42");
    std::map<std::string, std::string> values = {{"abc", "bar"}};
    std::string res;
    res.resize(4);
    auto replacer = sl::io::make_replacer_source(src, values, [](const std::string& msg) {slassert(msg.empty()); });
    replacer.read(res);
    slassert("foxb" == res);
    replacer.read({std::addressof(res.front()), 2});
    slassert("arxb" == res);
    replacer.read({std::addressof(res.front()), 2});
    slassert("42xb" == res);
}

void test_multiple() {
    auto src = two_bytes_at_once_source("{{foo}}{{bar}}{{baz}}");
    std::map<std::string, std::string> values = {
        {"foo", "1"},
        {"bar", ""},
        {"baz", "2345678"}
    };
    auto replacer = sl::io::make_replacer_source(std::move(src), std::move(values), [](const std::string& msg) {slassert(msg.empty()); });
    auto sink = sl::io::string_sink();
    std::array<char, 1024> buf;
    sl::io::copy_all(replacer, sink, buf);
    slassert("12345678" == sink.get_string());
}

int main() {
    try {
        test_replace();
        test_multiple();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

