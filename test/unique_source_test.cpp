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
 * File:   unique_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 3:18 PM
 */

#include "staticlib/io/unique_source.hpp"

#include <array>
#include <iostream>
#include <memory>

#include "staticlib/config/assert.hpp"

#include "staticlib/io/counting_source.hpp"

#include "NonCopyableSource.hpp"

namespace io = staticlib::io;

void test_movable() {
    io::unique_source<NonCopyableSource> source{new NonCopyableSource{}};
    io::counting_source<io::unique_source<NonCopyableSource>> wrapped{std::move(source)};
    std::array<char, 3> buf;
    auto read = wrapped.read(buf.data(), 3);
    slassert(3 == read);
    slassert(3 == wrapped.get_count());
    slassert(3 == wrapped.get_source().get_source().get_count());
}

int main() {
    try {
        test_movable();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

