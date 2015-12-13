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
 * File:   reference_sink.hpp
 * Author: alex
 *
 * Created on December 13, 2015, 2:18 PM
 */

#ifndef STATICLIB_IO_REFERENCE_SINK_HPP
#define	STATICLIB_IO_REFERENCE_SINK_HPP

#include <ios>
#include <functional>

namespace staticlib {
namespace io {

/**
 * Sink wrapper that holds a "std::reference_wrapper" to the underlying sink
 */
template <typename Sink>
class reference_sink {
    /**
     * Destination sink
     */
    std::reference_wrapper<Sink> sink;

public:

    /**
     * Constructor
     * 
     * @param sink destination sink
     */
    reference_sink(Sink& sink) :
    sink(sink) { }

    /**
     * Copy constructor
     * 
     * @param other instance
     */
    reference_sink(const reference_sink& other) :
    sink(other.sink) { }

    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    reference_sink& operator=(const reference_sink& other) {
        this->sink = other.sink;
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    reference_sink(reference_sink&& other) :
    sink(std::move(other.sink)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    reference_sink& operator=(reference_sink&& other) {
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
        return sink.get().write(buffer, length);
    }

    /**
     * Flushes destination sink
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        return sink.get().flush();
    }

    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */
    Sink& get_sink() {
        return sink.get();
    }

};

/**
 * Factory function for creating reference sinks
 * 
 * @param sink destination sink
 * @return reference sink
 */
template <typename Sink>
reference_sink<Sink> make_reference_sink(Sink& sink) {
    return reference_sink<Sink>(sink);
}

} // namespace
}

#endif	/* STATICLIB_IO_REFERENCE_SINK_HPP */

