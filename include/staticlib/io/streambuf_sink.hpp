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
 * File:   streambuf_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:32 PM
 */

#ifndef STATICLIB_IO_STREAMBUF_SINK_HPP
#define	STATICLIB_IO_STREAMBUF_SINK_HPP

#include <ios>
#include <streambuf>

#include "staticlib/config/noexcept.hpp"
#include "staticlib/config/span.hpp"

namespace staticlib {
namespace io {

/**
 * Sink implementation that wraps a pointer to the "std::streambuf"
 */
class streambuf_sink {
    /**
     * Destination streambuf
     */
    std::streambuf* streambuf;

public:
    /**
     * Constructor
     * 
     * @param streambuf destination streambuf
     */
    explicit streambuf_sink(std::streambuf* streambuf) :
    streambuf(streambuf) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    streambuf_sink(const streambuf_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    streambuf_sink& operator=(const streambuf_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    streambuf_sink(streambuf_sink&& other) STATICLIB_NOEXCEPT :
    streambuf(other.streambuf) {
        other.streambuf = nullptr;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    streambuf_sink& operator=(streambuf_sink&& other) STATICLIB_NOEXCEPT {
        streambuf = other.streambuf;
        other.streambuf = nullptr;
        return *this;
    }

    /**
     * Write implementation delegated to the underlying streambuf
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize write(staticlib::config::span<const char> span) {
        return streambuf->sputn(span.data(), span.size_signed());
    }

    /**
     * Flushes destination streambuf
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        return streambuf->pubsync();
    }

};

} // namespace
}


#endif	/* STATICLIB_IO_STREAMBUF_SINK_HPP */

