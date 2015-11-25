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
 * File:   TwoBytesAtOnceSource.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:44 AM
 */

#ifndef STATICLIB_IO_TEST_TWOBYTESATONCESOURCE_HPP
#define	STATICLIB_IO_TEST_TWOBYTESATONCESOURCE_HPP

class TwoBytesAtOnceSource {
    std::string data;
    size_t ind = 0;

public:
    TwoBytesAtOnceSource(std::string data) : 
    data(std::move(data)) { }
    
    TwoBytesAtOnceSource(const TwoBytesAtOnceSource&) = delete;

    TwoBytesAtOnceSource& operator=(const TwoBytesAtOnceSource&) = delete;

    TwoBytesAtOnceSource(TwoBytesAtOnceSource&& other) :
    data(std::move(other.data)) { }

    TwoBytesAtOnceSource& operator=(TwoBytesAtOnceSource&& other) {
        data = std::move(other.data);
        return *this;
    }

    std::streamsize read(char* s, std::streamsize n) {
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

#endif	/* STATICLIB_IO_TEST_TWOBYTESATONCESOURCE_HPP */

