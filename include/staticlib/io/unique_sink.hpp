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
 * File:   unique_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 1:02 PM
 */

#ifndef STATICLIB_IO_UNIQUE_SINK_HPP
#define	STATICLIB_IO_UNIQUE_SINK_HPP

#include <ios>
#include <memory>

namespace staticlib {
namespace io {

/**
 * Sink wrapper that holds a "std::unique_ptr" to the underlying sink
 */
template <typename Sink>
class unique_sink {
    /**
     * Destination sink
     */
    std::unique_ptr<Sink> sink;

public:
    /**
     * Constructor
     * 
     * @param sink destination sink
     */
    unique_sink(Sink* sink) :
    sink(sink) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */    
    unique_sink(const unique_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */    
    unique_sink& operator=(const unique_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */    
    unique_sink(unique_sink&& other) :
    sink(std::move(other.sink)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */    
    unique_sink& operator=(unique_sink&& other) {
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
        return sink->write(buffer, length);
    }

    /**
     * Flushes destination sink
     * 
     * @return number of bytes flushed
     */    
    std::streamsize flush() {
        return sink->flush();
    }

    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */    
    Sink& get_sink() {
        return *sink;
    }

};

/**
 * Factory function for creating unique sinks
 * 
 * @param sink destination sink
 * @return unique sink
 */
template <typename Sink>
unique_sink<Sink> make_unique_sink(Sink* sink) {
    return unique_sink<Sink>(sink);
}

} // namespace
}

#endif	/* STATICLIB_IO_UNIQUE_SINK_HPP */

