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
 * File:   copying_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:24 PM
 */

#ifndef STATICLIB_IO_COPYING_SOURCE_HPP
#define	STATICLIB_IO_COPYING_SOURCE_HPP

#include <ios>
#include <utility>

#include "staticlib/io/operations.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that copies all the data read from it into
 * the sink specified on construction
 */
template<typename Source, typename Sink>
class copying_source {
    /**
     * Input source
     */
    Source src;
    /**
     * Sink for data copy
     */
    Sink sink;

public:
    /**
     * Constructor
     * 
     * @param src input source
     * @param sink sink for data copy
     */
    copying_source(Source src, Sink sink) :
    src(std::move(src)),
    sink(std::move(sink)) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    copying_source(const copying_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    copying_source& operator=(const copying_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    copying_source(copying_source&& other) :
    src(std::move(other.src)),
    sink(std::move(other.sink))  { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    copying_source& operator=(copying_source&& other) {
        src = std::move(other.src);
        sink = std::move(other.sink);
        sink = std::move(other.sink);
        return *this;
    }

    /**
     * Read implementation with additional copy to 
     * the sink specified on construction
     * 
     * @param buffer output buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize read(char* buffer, std::streamsize length) {
        std::streamsize res = src.read(buffer, length);
        if (std::char_traits<char>::eof() != res) {
            write_all(sink, buffer, res);
        }
        return res;
    }

    /**
     * Flushes copy sink
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        return sink.flush();
    }

    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return src;
    }

    /**
     * Underlying copy sink accessor
     * 
     * @return underlying copy sink reference
     */
    Sink& get_sink() {
        return sink;
    }
    
};

/**
 * Factory function for creating copying sources
 * 
 * @param source input source
 * @param sink copy sink
 * @return copying source
 */
template<typename Source, typename Sink>
copying_source<Source, Sink> make_copying_source(Source&& source, Sink&& sink) {
    return copying_source<Source, Sink>(std::move(source), std::move(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_COPYING_SOURCE_HPP */

