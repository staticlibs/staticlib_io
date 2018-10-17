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
 * File:   limited_source.hpp
 * Author: alex
 *
 * Created on January 29, 2017, 1:33 PM
 */

#ifndef STATICLIB_IO_LIMITED_SOURCE_HPP
#define STATICLIB_IO_LIMITED_SOURCE_HPP

#include <ios>

#include "staticlib/config.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/counting_source.hpp"
#include "staticlib/io/reference_source.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that limits the number of bytes read through it
 * and returns EOF on threshold exceed
 */
template<typename Source>
class limited_source {
    /**
     * Input source
     */
    staticlib::io::counting_source<Source> src;

    /**
     * Limit in bytes
     */
    size_t limit_bytes;

public:

    /**
     * Constructor,
     * created source wrapper will own specified source
     * 
     * @param src input source
     * @param limit_bytes max number of bytes allowed to be read from this source
     */
    limited_source(Source&& src, size_t limit_bytes) :
    src(staticlib::io::make_counting_source(std::move(src))),
    limit_bytes(limit_bytes) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    limited_source(const limited_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    limited_source& operator=(const limited_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    limited_source(limited_source&& other) STATICLIB_NOEXCEPT :
    src(std::move(other.src)),
    limit_bytes(other.limit_bytes) {
        other.limit_bytes = 0;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    limited_source& operator=(limited_source&& other) STATICLIB_NOEXCEPT {
        src = std::move(other.src);
        limit_bytes = other.limit_bytes;
        other.limit_bytes = 0;
        return *this;
    }

    /**
     * Counting read implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize read(span<char> origin_span) {
        if (src.get_count() < limit_bytes) {
            size_t ulen = origin_span.size();
            auto remaining_bytes = limit_bytes - src.get_count();
            auto read_length = (ulen < remaining_bytes) ? ulen : remaining_bytes;
            return src.read(span<char> {origin_span.data(), static_cast<uint32_t>(read_length)});
        } else {
            return std::char_traits<char>::eof();
        }
    }

    /**
     * Returns number of bytes read through this instance
     * 
     * @return number of bytes read through this instance
     */
    size_t get_count() {
        return src.get_count();
    }

    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return src.get_source();
    }

};

/**
 * Factory function for creating limited sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @param limit_bytes max number of bytes allowed to be read from this source
 * @return limited source
 */
template <typename Source,
class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
limited_source<Source> make_limited_source(Source&& source, size_t limit_bytes) {
    return limited_source<Source>(std::move(source), limit_bytes);
}

/**
 * Factory function for creating limited sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @param limit_bytes max number of bytes allowed to be read from this source
 * @return limited source
 */
template <typename Source>
limited_source<reference_source<Source>> make_limited_source(Source& source, size_t limit_bytes) {
    return limited_source<reference_source<Source>> (make_reference_source(source), limit_bytes);
}

} // namespace
}

#endif /* STATICLIB_IO_LIMITED_SOURCE_HPP */

