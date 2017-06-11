/*
 * Copyright 2017, alex at staticlibs.net
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
 * File:   array_sink.hpp
 * Author: alex
 *
 * Created on June 11, 2017, 10:51 PM
 */

#ifndef STATICLIB_IO_ARRAY_SINK_HPP
#define	STATICLIB_IO_ARRAY_SINK_HPP

#include <cstdlib>
#include <cstring>
#include <functional>

#include "staticlib/config.hpp"
#include "staticlib/support.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Growing heap array, that can release underlying buffer
 */
class array_sink {
    /**
     * Buffer
     */
    char* buf = nullptr;
    /**
     * Buffer capacity
     */
    size_t capacity = 0;
    /**
     * Buffer size
     */
    size_t bufsize = 0;

public:

    // todo: custom malloc and free support
    /**
     * Constructor,
     * created empty sink (cannot be released, until written into)
     */
    array_sink() { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    array_sink(const array_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    array_sink& operator=(const array_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    array_sink(array_sink&& other) STATICLIB_NOEXCEPT :
    buf(other.buf),
    capacity(other.capacity),
    bufsize(other.bufsize) {
        other.buf = nullptr;
        other.capacity = 0;
        other.bufsize = 0;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    array_sink& operator=(array_sink&& other) STATICLIB_NOEXCEPT {
        buf = other.buf;
        capacity = other.capacity;
        other.capacity = 0;
        bufsize = other.bufsize;
        other.bufsize = 0;        
        return *this;
    }

    /**
     * Destructor
     */
    ~array_sink() STATICLIB_NOEXCEPT {
        if (nullptr != buf) {
            std::free(buf);
        }
    }
    
    /**
     * Write implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize write(span<const char> span) {
        if (capacity - bufsize >= span.size()) {
            std::memcpy(buf + bufsize, span.data(), span.size());
            bufsize += span.size();
            return span.size();
        }
        capacity = capacity * 1.5;
        if (bufsize + span.size() > capacity) {
            capacity = bufsize + span.size();
        }
        char* nbuf = static_cast<char*>(std::malloc(capacity + 1));
        if (nullptr == nbuf) throw io_exception(TRACEMSG(
                "Alloc error for length: [" + sl::support::to_string(capacity) + "]"));
        if (nullptr != buf) {
            std::memcpy(nbuf, buf, bufsize);
            std::free(buf);
        }
        buf = nbuf;
        std::memcpy(nbuf + bufsize, span.data(), span.size());
        bufsize += span.size();
        return span.size_signed();
    }

    /**
     * No-op flush
     * 
     * @return zero
     */
    std::streamsize flush() {
        /* no-op */
        return 0;
    }
    
    /**
     * Releases underlying buffer and returns it inside the span.
     * Returned buffer is always NUL-terminated and must be
     * freed by the client.
     * Sink is left in empty state.
     * 
     * @return span with sink buffer
     */
    span<char> release() {
        if (nullptr != buf) {
            buf[bufsize] = '\0';
            auto res = make_span(buf, bufsize);
            buf = nullptr;
            bufsize = 0;
            capacity = 0;
            return res;
        }
        throw io_exception(TRACEMSG("Cannot release invalid empty 'array_sink'"));
    }
    
    /**
     * Underlying buffer accessor
     * 
     * @return pointer to underlying buffer
     */
    char* data() {
        return buf;
    }
    
    /**
     * Size accessor
     * 
     * @return current size of the buffer
     */
    size_t size() {
        return bufsize;
    }
    
};

} // namespace
}

#endif	/* STATICLIB_IO_ARRAY_SINK_HPP */

