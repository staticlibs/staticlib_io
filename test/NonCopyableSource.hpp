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
 * File:   NonCopyableSource.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:03 PM
 */

#ifndef NONCOPYABLESOURCE_HPP
#define	NONCOPYABLESOURCE_HPP

class NonCopyableSource {
    size_t count = 0;
    
public:
    NonCopyableSource() { }

    NonCopyableSource(const NonCopyableSource&) = delete;

    NonCopyableSource& operator=(const NonCopyableSource&) = delete;

    NonCopyableSource(NonCopyableSource&&) = delete;

    NonCopyableSource& operator=(NonCopyableSource&&) = delete;

    std::streamsize read(char*, std::streamsize n) {
        count += static_cast<size_t>(n);
        return n;
    }
    
    size_t get_count() {
        return count;
    }

};

#endif	/* NONCOPYABLESOURCE_HPP */

