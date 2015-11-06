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
 * File:   NonFlushableSink.hpp
 * Author: alex
 *
 * Created on October 13, 2015, 8:01 AM
 */

#ifndef NONFLUSHABLESINK_HPP
#define	NONFLUSHABLESINK_HPP

class NonFlushableSink {
    size_t count = 0;

public:

    NonFlushableSink() { }

    NonFlushableSink(const NonFlushableSink&) = delete;

    NonFlushableSink& operator=(const NonFlushableSink&) = delete;

    NonFlushableSink(NonFlushableSink&& other) :
    count(other.count) { }

    NonFlushableSink& operator=(NonFlushableSink&& other) { 
        count = other.count;
        return *this;
    }

    std::streamsize write(const char*, std::streamsize n) {
        count += static_cast<size_t> (n);
        return n;
    }

    size_t get_count() {
        return count;
    }

};

#endif	/* NONFLUSHABLESINK_HPP */

