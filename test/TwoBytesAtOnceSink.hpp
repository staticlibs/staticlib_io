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
 * File:   TwoBytesAtOnceSink.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:43 AM
 */

#ifndef STATICLIB_IO_TEST_TWOBYTESATONCESINK_HPP
#define	STATICLIB_IO_TEST_TWOBYTESATONCESINK_HPP

class TwoBytesAtOnceSink {
    std::string data;

public:

    TwoBytesAtOnceSink() { }
    
    TwoBytesAtOnceSink(const TwoBytesAtOnceSink&) = delete;
    
    TwoBytesAtOnceSink& operator=(const TwoBytesAtOnceSink&) = delete;
    
    TwoBytesAtOnceSink(TwoBytesAtOnceSink&& other) :
    data(std::move(other.data)) { }
    
    TwoBytesAtOnceSink& operator=(TwoBytesAtOnceSink&& other) {
        data = std::move(other.data);
        return *this;
    }

    std::streamsize write(const char* s, std::streamsize n) {
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

#endif	/* STATICLIB_IO_TEST_TWOBYTESATONCESINK_HPP */

