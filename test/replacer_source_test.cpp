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

#include "TwoBytesAtOnceSource.hpp"

namespace io = staticlib::io;

void test_replace() {
    auto src = TwoBytesAtOnceSource("fox{{abc}}42");
    std::map<std::string, std::string> values = {{"abc", "bar"}};
    std::string res;
    res.resize(4);
    auto replacer = io::make_replacer_source(src, values);
    replacer.read(std::addressof(res.front()), res.length());
    slassert("foxb" == res);
    replacer.read(std::addressof(res.front()), 2);
    slassert("arxb" == res);
    replacer.read(std::addressof(res.front()), 2);
    slassert("42xb" == res);
}

int main() {
    try {
        test_replace();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

