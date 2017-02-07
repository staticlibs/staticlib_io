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
 * File:   two_bytes_at_once_sink.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:43 AM
 */

#ifndef STATICLIB_IO_TEST_TWO_BYTES_AT_ONCE_SINK_HPP
#define	STATICLIB_IO_TEST_TWO_BYTES_AT_ONCE_SINK_HPP

class two_bytes_at_once_sink {
    std::string data;

public:

    two_bytes_at_once_sink() { }
    
    two_bytes_at_once_sink(const two_bytes_at_once_sink&) = delete;
    
    two_bytes_at_once_sink& operator=(const two_bytes_at_once_sink&) = delete;
    
    two_bytes_at_once_sink(two_bytes_at_once_sink&& other) :
    data(std::move(other.data)) { }
    
    two_bytes_at_once_sink& operator=(two_bytes_at_once_sink&& other) {
        data = std::move(other.data);
        return *this;
    }

    std::streamsize write(staticlib::config::span<const char> span) {
        const char* s = span.data();
        std::streamsize n = span.size_signed();
        std::streamsize len = n >= 2 ? 2 : n;
        for (std::streamsize i = 0; i < len; i++) {
            data.push_back(s[i]);
        }
        return len;
    }
    
    std::streamsize flush() { 
        return 0;
    }
    
    std::string& get_data() {
        return data;
    }
};

#endif	/* STATICLIB_IO_TEST_TWO_BYTES_AT_ONCE_SINK_HPP */

