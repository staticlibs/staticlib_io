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
 * File:   non_copyable_source.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:03 PM
 */

#ifndef STATICLIB_IO_TEST_NON_COPYABLE_SOURCE_HPP
#define STATICLIB_IO_TEST_NON_COPYABLE_SOURCE_HPP

class non_copyable_source {
    size_t count = 0;
    
public:
    non_copyable_source() { }

    non_copyable_source(const non_copyable_source&) = delete;

    non_copyable_source& operator=(const non_copyable_source&) = delete;

    non_copyable_source(non_copyable_source&&) = delete;

    non_copyable_source& operator=(non_copyable_source&&) = delete;

    std::streamsize read(sl::io::span<char> span) {
        count += span.size();
        return span.size_signed();
    }
    
    size_t get_count() {
        return count;
    }

};

#endif /* STATICLIB_IO_TEST_NON_COPYABLE_SOURCE_HPP */

