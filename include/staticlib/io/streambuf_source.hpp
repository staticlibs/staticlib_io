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
 * File:   streambuf_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:32 PM
 */

#ifndef STATICLIB_IO_STREAMBUF_SOURCE_HPP
#define	STATICLIB_IO_STREAMBUF_SOURCE_HPP

#include <ios>
#include <streambuf>

namespace staticlib {
namespace io {

/**
 * Source implementation that wraps a pointer to the "std::streambuf"
 */
class streambuf_source {
    /**
     * Input streambuf
     */
    std::streambuf* streambuf;
    
public:
    /**
     * Constructor
     * 
     * @param streambuf input streambuf
     */
    streambuf_source(std::streambuf* streambuf) :
    streambuf(streambuf) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */

    streambuf_source(const streambuf_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    streambuf_source& operator=(const streambuf_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */    
    streambuf_source(streambuf_source&& other) :
    streambuf(other.streambuf) {
        other.streambuf = nullptr;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */    
    streambuf_source& operator=(streambuf_source&& other) {
        streambuf = other.streambuf;
        other.streambuf = nullptr;
        return *this;
    }

    /**
     * Read implementation delegated to the underlying streambuf
     * 
     * @param buffer output buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */    
    std::streamsize read(char* buffer, std::streamsize length) {
        std::streamsize res = streambuf->sgetn(buffer, length);
        if (res > 0) {
            return res;
        } else if (0 == res && std::char_traits<char>::eof() == streambuf->sbumpc()) {
            return std::char_traits<char>::eof();
        } else {
            streambuf->sungetc();
            return res;
        }
    }
    
};

} // namespace
}

#endif	/* STATICLIB_IO_STREAMBUF_SOURCE_HPP */

