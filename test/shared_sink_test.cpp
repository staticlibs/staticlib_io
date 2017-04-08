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
 * File:   shared_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:02 PM
 */

#include "staticlib/io/shared_sink.hpp"

#include <iostream>
#include <memory>

#include "staticlib/config/assert.hpp"

#include "non_copyable_sink.hpp"

void test_copyable() {
    std::shared_ptr<non_copyable_sink> sink{new non_copyable_sink{}};
    sl::io::shared_sink<non_copyable_sink> shared1{sink};
    sl::io::shared_sink<non_copyable_sink> shared2{sink};
    auto written1 = shared1.write({"foo", 3});
    slassert(3 == written1);
    slassert(3 == sink->get_count());
    slassert(3 == shared1.get_sink().get_count());
    slassert(3 == shared2.get_sink().get_count());
    auto written2 = shared2.write({"42", 2});
    slassert(2 == written2);
    slassert(5 == sink->get_count());
    slassert(5 == shared1.get_sink().get_count());
    slassert(5 == shared2.get_sink().get_count());
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
