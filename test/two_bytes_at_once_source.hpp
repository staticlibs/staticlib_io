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
 * File:   two_bytes_at_once_source.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:44 AM
 */

#ifndef STATICLIB_IO_TEST_TWO_BYTES_AT_ONCE_SOURCE_HPP
#define	STATICLIB_IO_TEST_TWO_BYTES_AT_ONCE_SOURCE_HPP

class two_bytes_at_once_source {
    std::string data;
    size_t ind = 0;

public:
    two_bytes_at_once_source(std::string data) : 
    data(std::move(data)) { }
    
    two_bytes_at_once_source(const two_bytes_at_once_source&) = delete;

    two_bytes_at_once_source& operator=(const two_bytes_at_once_source&) = delete;

    two_bytes_at_once_source(two_bytes_at_once_source&& other) :
    data(std::move(other.data)) { }

    two_bytes_at_once_source& operator=(two_bytes_at_once_source&& other) {
        data = std::move(other.data);
        return *this;
    }

    std::streamsize read(staticlib::config::span<char> span) {
        char* s = span.data();
        std::streamsize n = span.size_signed();
        if (ind >= data.size()) return std::char_traits<char>::eof();
        std::streamsize len = 0;
        if (1 == data.size() - ind)  {
            len = 1;
        } else len = n > 2 ? 2 : n;
        for (std::streamsize i = 0; i < len; i++) {
            s[i] = data[ind++];
        }
        return len;
    }
    
};

#endif	/* STATICLIB_IO_TEST_TWO_BYTES_AT_ONCE_SOURCE_HPP */

