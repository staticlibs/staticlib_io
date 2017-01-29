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
 * File:   counting_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:23 PM
 */

#ifndef STATICLIB_IO_COUNTING_SINK_HPP
#define	STATICLIB_IO_COUNTING_SINK_HPP

#include <ios>

#include "staticlib/config.hpp"

#include "staticlib/io/reference_sink.hpp"

namespace staticlib {
namespace io {

/**
 * Sink wrapper that counts the number of bytes written through it
 */
template<typename Sink>
class counting_sink {
    /**
     * Destination sink
     */
    Sink sink;
    /**
     * Number of bytes written
     */
    size_t count = 0;

public:
    /**
     * Constructor,
     * created sink wrapper will own specified sink
     * 
     * @param sink destination sink
     */
    counting_sink(Sink&& sink) :
    sink(std::move(sink)) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    counting_sink(const counting_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    counting_sink& operator=(const counting_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    counting_sink(counting_sink&& other) :
    sink(std::move(other.sink)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    counting_sink& operator=(counting_sink&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    /**
     * Counting write implementation
     * 
     * @param buffer source buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize write(const char* buffer, std::streamsize length) {
        namespace sc = staticlib::config;
        std::streamsize res = sink.write(buffer, length);
        if (sc::is_sizet(res)) {
            count += static_cast<size_t>(res);
        }
        return res;
    }

    /**
     * Flushes destination sink
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        return sink.flush();
    }

    /**
     * Returns number of bytes written through this instance
     * 
     * @return number of bytes written through this instance
     */
    size_t get_count() {
        return count;
    }

    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */
    Sink& get_sink() {
        return sink;
    }

};

/**
 * Factory function for creating counting sinks,
 * created sink wrapper will own specified sink
 * 
 * @param sink destination sink
 * @return counting sink
 */
template <typename Sink,
        class = typename std::enable_if<!std::is_lvalue_reference<Sink>::value>::type>
counting_sink<Sink> make_counting_sink(Sink&& sink) {
    return counting_sink<Sink>(std::move(sink));
}

/**
 * Factory function for creating counting sinks,
 * created sink wrapper will NOT own specified sink
 * 
 * @param sink destination sink
 * @return counting sink
 */
template <typename Sink>
counting_sink<reference_sink<Sink>> make_counting_sink(Sink& sink) {
    return counting_sink<reference_sink<Sink>>(make_reference_sink(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_COUNTING_SINK_HPP */

