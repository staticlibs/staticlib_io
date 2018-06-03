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
 * File:   hex_sink.hpp
 * Author: alex
 *
 * Created on June 2, 2018, 10:53 PM
 */

#ifndef STATICLIB_IO_HEX_SINK_HPP
#define STATICLIB_IO_HEX_SINK_HPP

#include <ios>

#include "staticlib/config.hpp"

#include "staticlib/io/reference_sink.hpp"
#include "staticlib/io/operations.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

namespace hex_sink_detail {

const std::array<char, 16> symbols = {{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}};

} // namespace

/**
 * Sink wrapper that encodes data into Hexadecimal,
 * should be used in conjunction with buffered sink.
 */
template<typename Sink>
class hex_sink {
    /**
     * Destination sink
     */
    Sink sink;
    /**
     * Encode buffer
     */
    std::array<char, 2> hbuf;

public:
    /**
     * Constructor,
     * created sink wrapper will own specified sink
     * 
     * @param sink destination sink
     */
    explicit hex_sink(Sink&& sink) :
    sink(std::move(sink)) {
        hbuf[0] = '\0';
        hbuf[1] = '\0';
    }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    hex_sink(const hex_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    hex_sink& operator=(const hex_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    hex_sink(hex_sink&& other) STATICLIB_NOEXCEPT :
    sink(std::move(other.sink)),
    hbuf(std::move(other.hbuf)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    hex_sink& operator=(hex_sink&& other) STATICLIB_NOEXCEPT {
        sink = std::move(other.sink);
        hbuf = std::move(other.hbuf);
        return *this;
    }

    /**
     * Hex encoding write implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize write(span<const char> span) {
        for (size_t i = 0; i < span.size(); i++) {
            char ch = span.data()[i];
            // http://stackoverflow.com/a/18025541/314015
            unsigned char uch = static_cast<unsigned char>(ch);
            hbuf[0] = hex_sink_detail::symbols[static_cast<size_t>(uch >> 4)];
            hbuf[1] = hex_sink_detail::symbols[static_cast<size_t>(uch & 0x0f)];
            sl::io::write_all(sink, {hbuf.data(), hbuf.size()});
        }
        return span.size_signed();
    }

    /**
     * Flushes destination sink
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        return sink.flush();
    }

    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */
    Sink& get_sink() {
        return sink;
    }

};

/**
 * Factory function for creating hex sinks,
 * created sink wrapper will own specified sink
 * 
 * @param sink destination sink
 * @return counting sink
 */
template <typename Sink,
        class = typename std::enable_if<!std::is_lvalue_reference<Sink>::value>::type>
hex_sink<Sink> make_hex_sink(Sink&& sink) {
    return hex_sink<Sink>(std::move(sink));
}

/**
 * Factory function for creating hex sinks,
 * created sink wrapper will NOT own specified sink
 * 
 * @param sink destination sink
 * @return counting sink
 */
template <typename Sink>
hex_sink<reference_sink<Sink>> make_hex_sink(Sink& sink) {
    return hex_sink<reference_sink<Sink>>(make_reference_sink(sink));
}

} // namespace
}

#endif /* STATICLIB_IO_HEX_SINK_HPP */

