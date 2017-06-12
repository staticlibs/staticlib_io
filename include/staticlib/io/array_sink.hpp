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
#include <utility>

#include "staticlib/config.hpp"
#include "staticlib/support.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

namespace detail_array_sink {

inline char* default_alloc(int size_bytes) {
    if (!size_bytes > 0) {
        return nullptr;
    }
    return reinterpret_cast<char*> (std::malloc(static_cast<size_t> (size_bytes)));
};

inline void default_free(char* buffer) {
    std::free(buffer);
}

} // namespace

/**
 * Growing heap array, that can release underlying buffer
 */
template<typename AllocFun, typename FreeFun>
class array_sink {       
    
    /**
     * Allocator function, expected signature "std::function<char*(int)>"
     */
    AllocFun alloc_fun;
    
    /**
     * Deallocator function, expected signature "std::function<void(char*)>"
     */
    FreeFun free_fun;
    
    /**
     * Growth coefficient
     */
    float grow_coef;
    
    /**
     * Buffer
     */
    char* buf;
    /**
     * Buffer capacity
     */
    size_t capacity;
    /**
     * Buffer size
     */
    size_t bufsize = 0;

public:
    
    /**
     * Constructor
     * 
     * @param alloc_fun function to use to allocate memory
     * @param free_fun function to use to free memory 
     * @param initial_capacity initial capacity to allocate* 
     */
    array_sink(AllocFun alloc_fun, FreeFun free_fun,
            size_t initial_capacity, float grow_coef = 1.5):
    alloc_fun(std::move(alloc_fun)),
    free_fun(std::move(free_fun)),
    grow_coef(grow_coef),
    buf(alloc_fun(static_cast<int>(initial_capacity + 1))),
    capacity(initial_capacity) {
        if (nullptr == buf) {
            throw io_exception(TRACEMSG(
                "Alloc error for capacity: [" + sl::support::to_string(capacity) + "]"));
        }
    }

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
    alloc_fun(std::move(other.alloc_fun)),
    free_fun(std::move(other.free_fun)),
    grow_coef(other.grow_coef),
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
        alloc_fun(std::move(other.alloc_fun));
        free_fun(std::move(other.free_fun));
        grow_coef = other.grow_coef;
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
            free_fun(buf);
        }
    }
    
    /**
     * Write implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize write(span<const char> span) {
        while (bufsize + span.size() > capacity) {
            capacity = capacity * grow_coef;
            char* nbuf = alloc_fun(capacity + 1);
            if (nullptr == nbuf) throw io_exception(TRACEMSG(
                    "Alloc error for capacity: [" + sl::support::to_string(capacity) + "]"));
            std::memcpy(nbuf, buf, bufsize);
            free_fun(buf);
            buf = nbuf;
        }
        std::memcpy(buf + bufsize, span.data(), span.size());
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
     * Underlying buffer const accessor
     * 
     * @return const pointer to underlying buffer
     */
    const char* data() const {
        return buf;
    }
    
    /**
     * Size accessor
     * 
     * @return current size of the buffer
     */
    size_t size() const {
        return bufsize;
    }
    
};


/**
 * Factory function for creating array_sink,
 * uses "std::malloc" and "std::free"
 * 
 * @param alloc_fun allocator function
 * @param free_fun deallocator function
 * @param initial_capacity initial buffer capacity to allocate
 * @return array_sink
 */
template <typename AllocFun, typename FreeFun>
array_sink<AllocFun, FreeFun> make_array_sink(AllocFun alloc_fun, FreeFun free_fun,
        size_t initial_capacity = 15) {
    return array_sink<AllocFun, FreeFun>(std::move(alloc_fun), std::move(free_fun), initial_capacity);
}

/**
 * Factory function for creating array_sink,
 * uses "std::malloc" and "std::free"
 * 
 * @param initial_capacity initial buffer capacity to allocate
 * @return array_sink
 */
inline array_sink<char*(*)(int), void(*)(char*)> make_array_sink(size_t initial_capacity = 15) {
    return make_array_sink(detail_array_sink::default_alloc, detail_array_sink::default_free, initial_capacity);
}

} // namespace
}

#endif	/* STATICLIB_IO_ARRAY_SINK_HPP */

