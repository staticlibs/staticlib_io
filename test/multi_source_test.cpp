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
 * File:   multi_source_test.cpp
 * Author: alex
 *
 * Created on June 18, 2018, 11:55 AM
 */

#include "staticlib/io/multi_source.hpp"

#include <iostream>
#include <list>
#include <string>

#include "staticlib/config/assert.hpp"

#include "staticlib/io/operations.hpp"
#include "staticlib/io/string_source.hpp"

#include "two_bytes_at_once_source.hpp"

void test_multi() {

    // empty instances for factory function selection checks
    auto vec_empty = std::vector<sl::io::reference_source<two_bytes_at_once_source>>();
    auto multi_ref = sl::io::multi_source<std::vector<sl::io::reference_source<two_bytes_at_once_source>>>(
            std::move(vec_empty));
    auto li_empty = std::list<two_bytes_at_once_source>();
    auto multi_val = sl::io::multi_source<std::list<two_bytes_at_once_source>>(std::move(li_empty));

    // empty reads checks
    auto span_empty = sl::io::span<char>(nullptr, 0);
    slassert(std::char_traits<char>::eof() == multi_ref.read(span_empty));
    slassert(std::char_traits<char>::eof() == multi_val.read(span_empty));
    {
        // sources for ref case
        auto list_ref = std::list<two_bytes_at_once_source>();
        list_ref.push_back(two_bytes_at_once_source("foo"));
        list_ref.push_back(two_bytes_at_once_source("bar1"));
        list_ref.push_back(two_bytes_at_once_source("baz"));

        // move assignment
        multi_ref = sl::io::make_multi_source(list_ref);
        auto sink1 = sl::io::string_sink();
        sl::io::copy_all(multi_ref, sink1);
        slassert("foobar1baz" == sink1.get_string());

        // sources for val case
        auto list_val = std::list<two_bytes_at_once_source>();
        list_val.push_back(two_bytes_at_once_source("foo"));
        list_val.push_back(two_bytes_at_once_source("bar1"));
        list_val.push_back(two_bytes_at_once_source("baz"));

        multi_val = sl::io::make_multi_source(std::move(list_val));
    }
    auto sink2 = sl::io::string_sink();
    sl::io::copy_all(multi_val, sink2);
    slassert("foobar1baz" == sink2.get_string());
}

int main() {
    try {
        test_multi();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
