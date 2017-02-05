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
 * File:   shared_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:22 PM
 */

#include "staticlib/io/shared_source.hpp"

#include <array>
#include <iostream>
#include <memory>

#include "staticlib/config/assert.hpp"

#include "NonCopyableSource.hpp"

namespace io = staticlib::io;

void test_copyable() {
    std::shared_ptr<NonCopyableSource> source{new NonCopyableSource{}};
    io::shared_source<NonCopyableSource> shared1{source};
    io::shared_source<NonCopyableSource> shared2{source};
    std::array<char, 3> buf;
    auto read1 = shared1.read({buf.data(), 3});
    slassert(3 == read1);
    slassert(3 == source->get_count());
    slassert(3 == shared1.get_source().get_count());
    slassert(3 == shared2.get_source().get_count());
    auto read2 = shared2.read({buf.data(), 2});
    slassert(2 == read2);
    slassert(5 == source->get_count());
    slassert(5 == shared1.get_source().get_count());
    slassert(5 == shared2.get_source().get_count());
}

int main() {
    try {
        test_copyable();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

