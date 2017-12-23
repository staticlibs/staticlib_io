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
 * File:   shared_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:41 PM
 */

#ifndef STATICLIB_IO_SHARED_SINK_HPP
#define STATICLIB_IO_SHARED_SINK_HPP

#include <ios>
#include <memory>

#include "staticlib/config.hpp"

#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Sink wrapper that holds a "std::shared_ptr" to the underlying sink
 */
template <typename Sink>
class shared_sink {
    /**
     * Destination sink
     */
    std::shared_ptr<Sink> sink;

public:

    /**
     * Constructor
     * @param sink destination sink
     */
    shared_sink(std::shared_ptr<Sink> sink) :
    sink(std::move(sink)) { }

    /**
     * Copy constructor
     * 
     * @param other instance
     */
    shared_sink(const shared_sink& other) :
    sink(other.sink) { }

    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance
     */
    shared_sink& operator=(const shared_sink& other) {
        sink = other.sink;
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    shared_sink(shared_sink&& other) STATICLIB_NOEXCEPT :
    sink(std::move(other.sink)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    shared_sink& operator=(shared_sink&& other) STATICLIB_NOEXCEPT {
        sink = std::move(other.sink);
        return *this;
    }

    /**
     * Write implementation delegated to the underlying sink
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize write(span<const char> span) {
        return sink->write(span);
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
 * Factory function for creating shared sinks
 * 
 * @param sink destination sink
 * @return shared sink
 */
template <typename Sink>
shared_sink<Sink> make_shared_sink(std::shared_ptr<Sink> sink) {
    return shared_sink<Sink>(std::move(sink));
}

} // namespace
}

#endif /* STATICLIB_IO_SHARED_SINK_HPP */

