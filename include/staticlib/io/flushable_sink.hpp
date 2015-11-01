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
 * File:   flushable_sink.hpp
 * Author: alex
 *
 * Created on October 13, 2015, 7:53 AM
 */

#ifndef STATICLIB_IO_FLUSHABLE_SINK_HPP
#define	STATICLIB_IO_FLUSHABLE_SINK_HPP

#include <ios>

namespace staticlib {
namespace io {

/**
 * Sink wrapper that implements no-op flush
 */
template<typename NonFlushableSink>
class flushable_sink {
    /**
     * Destination sink
     */
    NonFlushableSink sink;

public:
    /**
     * Constructor
     * 
     * @param sink destination sink
     */
    flushable_sink(NonFlushableSink sink) :
    sink(std::move(sink)) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    flushable_sink(const flushable_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    flushable_sink& operator=(const flushable_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    flushable_sink(flushable_sink&& other) :
    sink(std::move(other.sink)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    flushable_sink& operator=(flushable_sink&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    /**
     * Write implementation delegated to the underlying sink
     * 
     * @param buffer source buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize write(const char* buffer, std::streamsize length) {
        return sink.write(buffer, length);
    }

    /**
     * No-op flush implementation
     * 
     * @return 0
     */
    std::streamsize flush() {
        return 0;
    }

    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */
    NonFlushableSink& get_sink() {
        return sink;
    }

};

/**
 * Factory function for creating flushable sinks
 * 
 * @param sink destination sink
 * @return flushable sink
 */
template <typename NonFlushableSink>
flushable_sink<NonFlushableSink> make_flushable_sink(NonFlushableSink&& sink) {
    return flushable_sink<NonFlushableSink>(std::move(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_FLUSHABLE_SINK_HPP */

