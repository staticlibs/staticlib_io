/*
 * Copyright 2018, alex at staticlibs.net
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
 * File:   memory_sink.hpp
 * Author: alex
 *
 * Created on June 8, 2018, 8:40 PM
 */

#ifndef STATICLIB_IO_MEMORY_SINK_HPP
#define STATICLIB_IO_MEMORY_SINK_HPP

#include <cstring>
#include <ios>

#include "staticlib/config.hpp"
#include "staticlib/support.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Sink implementation that writes into fixed-size
 * memory area. Exception is thrown on overwrite.
 */
class memory_sink {

    /**
     * Destination memory area
     */
    span<char> dest;
    /**
     * Dest index
     */
    size_t idx = 0;
public:
    /**
     * Constructor
     */
    memory_sink(span<char> dest):
    dest(dest) { }

    /**
     * Copy constructor
     * 
     * @param other instance
     */
    memory_sink(const memory_sink& other) :
    dest(other.dest),
    idx(other.idx) { }

    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance
     */
    memory_sink& operator=(const memory_sink& other) {
        dest = other.dest;
        idx = other.idx;
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    memory_sink(memory_sink&& other) STATICLIB_NOEXCEPT :
    dest(std::move(other.dest)),
    idx(other.idx) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    memory_sink& operator=(memory_sink&& other) STATICLIB_NOEXCEPT {
        dest = std::move(other.dest);
        idx = other.idx;
        return *this;
    }

    /**
     * Write implementation
     * 
     * @param span buffer span
     * @return specified length
     */
    std::streamsize write(span<const char> span) {
        if (span.size() <= dest.size() - idx) {
            std::memcpy(dest.data() + idx, span.data(), span.size());
            idx += span.size();
            return span.size_signed();
        }
        throw io_exception(TRACEMSG("Write overflow," + 
                " req: [" + sl::support::to_string(span.size()) + "]," +
                " avail: [" + sl::support::to_string(dest.size() - idx) + "]"));
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


#endif /* STATICLIB_IO_MEMORY_SINK_HPP */

