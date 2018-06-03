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
 * File:   hex_source.hpp
 * Author: alex
 *
 * Created on June 2, 2018, 10:31 PM
 */

#ifndef STATICLIB_IO_HEX_SOURCE_HPP
#define STATICLIB_IO_HEX_SOURCE_HPP

#include <array>
#include <ios>

#include "staticlib/config.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/reference_source.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that decodes data from Hexadecimal,
 * should be used in conjunction with buffered source.
 */
template<typename Source>
class hex_source {
    /**
     * Input source
     */
    Source src;
    /**
     * Decode buffer
     */
    std::array<char, 3> hbuf;

public:
    /**
     * Constructor,
     * created source wrapper will own specified source
     * 
     * @param src input source
     */
    explicit hex_source(Source&& src) :
    src(std::move(src)) {
        hbuf[0] = '\0';
        hbuf[1] = '\0';
        hbuf[2] = '\0';
    }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    hex_source(const hex_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    hex_source& operator=(const hex_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    hex_source(hex_source&& other) STATICLIB_NOEXCEPT :
    src(std::move(other.src)),
    hbuf(std::move(other.hbuf)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    hex_source& operator=(hex_source&& other) STATICLIB_NOEXCEPT {
        src = std::move(other.src);
        hbuf = std::move(other.hbuf);
        return *this;
    }

    /**
     * Hex-decoding read implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize read(span<char> span) {
        size_t idx = 0;
        while (idx < span.size()) {
            auto read = read_all(src, {hbuf.data(), 2});
            if (2 == read) {
                char* end = nullptr;
                errno = 0;
                char byte = static_cast<char> (strtol(hbuf.data(), std::addressof(end), 16));
                if (errno == ERANGE || end != hbuf.data() + 2) {
                    throw io_exception(TRACEMSG("Error parsing byte from HEX-pair: [" + std::string(hbuf.data(), 2) + "]"));
                }
                span.data()[idx] = byte;
                idx += 1;
            } else if (1 == read) {
                throw io_exception(TRACEMSG("Invalid non-even number of bytes available in HEX source"));
            } else if (0 == read) {
                break;
            } else {
                throw io_exception(TRACEMSG("Invalid hex decoder state"));
            }
        }
        if (idx > 0) {
            return static_cast<std::streamsize>(idx);
        }
        return std::char_traits<char>::eof();
    }

    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return src;
    }
    
};

/**
 * Factory function for creating hex sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return counting source
 */
template <typename Source,
        class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
hex_source<Source> make_hex_source(Source&& source) {
    return hex_source<Source>(std::move(source));
}

/**
 * Factory function for creating hex sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return counting source
 */
template <typename Source>
hex_source<reference_source<Source>> make_hex_source(Source& source) {
    return hex_source<reference_source<Source>>(make_reference_source(source));
}

} // namespace
}


#endif /* STATICLIB_IO_HEX_SOURCE_HPP */

