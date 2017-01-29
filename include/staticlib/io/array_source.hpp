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
 * File:   array_source.hpp
 * Author: alex
 *
 * Created on November 6, 2015, 3:59 PM
 */

#ifndef STATICLIB_IO_ARRAY_SOURCE_HPP
#define	STATICLIB_IO_ARRAY_SOURCE_HPP

#include <cstdint>
#include <cstring>
#include <ios>

#include "staticlib/config.hpp"

#include "staticlib/io/IOException.hpp"

namespace staticlib {
namespace io {

/**
 * Source implementation that reads data from the specified char buffer.
 * Buffer is not owned by this source.
 */
class array_source {
    /**
     * Source buffer
     */
    const char* src_buf;
    /**
     * Source buffer length
     */
    size_t src_buf_len;
    /**
     * Current buffer index
     */
    size_t idx = 0;

public:
    /**
     * Constructor
     * 
     * @param src_buf source buffer
     * @param src_buf_len source buffer length
     */
    array_source(const char* src_buf, size_t src_buf_len) :
    src_buf(src_buf),
    src_buf_len(src_buf_len) { }
    
    /**
     * Copy constructor
     * 
     * @param other instance
     */
    array_source(const array_source& other) :
    src_buf(other.src_buf),
    src_buf_len(other.src_buf_len),
    idx(other.idx) { }
    
    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    array_source& operator=(const array_source& other) {
        src_buf = other.src_buf;
        src_buf_len = other.src_buf_len;
        idx = other.idx;
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    array_source(array_source&& other) :
    src_buf(other.src_buf),
    src_buf_len(other.src_buf_len),
    idx(other.idx) {
        other.src_buf = nullptr;
        other.src_buf_len = 0;
        other.idx = 0;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    array_source& operator=(array_source&& other) {
        src_buf = other.src_buf;
        other.src_buf = nullptr;
        src_buf_len = other.src_buf_len;
        other.src_buf_len = 0;
        idx = other.idx;
        other.idx = 0;
        return *this;
    }
    
    /**
     * Buffered read implementation
     * 
     * @param buf output buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize read(char* buf, std::streamsize length) {
        namespace sc = staticlib::config;
        if (!sc::is_sizet(length)) throw IOException(TRACEMSG(
                "Invalid 'read' parameter specified, length: [" + sc::to_string(length) + "]"));
        size_t ulen = static_cast<size_t> (length);
        size_t avail = src_buf_len - idx;
        if (avail > 0) {
            size_t len = ulen <= avail ? ulen : avail;
            std::memcpy(buf, src_buf + idx, len);
            this->idx += len;
            return static_cast<std::streamsize>(len);
        } else return std::char_traits<char>::eof();
    }
    
    /**
     * Source buffer accessor
     * 
     * @return source buffer
     */
    const char* get_array() {
        return src_buf;
    }
    
};

} // namespace
}

#endif	/* STATICLIB_IO_ARRAY_SOURCE_HPP */

