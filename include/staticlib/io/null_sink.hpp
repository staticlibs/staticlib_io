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
 * File:   null_sink.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:14 PM
 */

#ifndef STATICLIB_IO_NULL_SINK_HPP
#define	STATICLIB_IO_NULL_SINK_HPP

#include <ios>

#include "staticlib/config/span.hpp"

namespace staticlib {
namespace io {

/**
 * Sink implementation with no-op write method
 */
class null_sink {
public:
    /**
     * Constructor
     */
    null_sink() { }

    /**
     * Copy constructor
     * 
     * @param other instance
     */
    null_sink(const null_sink&) { }

    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance
     */
    null_sink& operator=(const null_sink&) {
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    null_sink(null_sink&&) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    null_sink& operator=(null_sink&&) {
        return *this;
    }

    /**
     * No-op write implementation
     * 
     * @param span buffer span
     * @return specified length
     */
    std::streamsize write(staticlib::config::span<const char> span) {
        // no-op
        return span.size_signed();
    }

    /**
     * No-op flush implementation
     * 
     * @return 0
     */
    std::streamsize flush() {
        // no-op
        return 0;
    }

};

} // namespace
}

#endif	/* STATICLIB_IO_NULL_SINK_HPP */

