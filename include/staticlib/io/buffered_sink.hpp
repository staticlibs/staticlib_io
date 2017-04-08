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
 * File:   buffered_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:15 PM
 */

#ifndef STATICLIB_IO_BUFFERED_SINK_HPP
#define	STATICLIB_IO_BUFFERED_SINK_HPP

#include <cstring>
#include <array>
#include <ios>
#include <utility>

#include "staticlib/config.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/reference_sink.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Sink wrapper that buffers the output
 */
template <typename Sink, std::size_t buf_size = 4096>
class buffered_sink {
    /**
     * Destination sink
     */
    Sink sink;

    /**
     * Buffer
     */
    std::array<char, buf_size> buffer;
    /**
     * Current position in buffer
     */
    size_t pos = 0;
    /**
     * Number of bytes available in buffer
     */
    size_t avail = 0;

public:
    /**
     * Constructor,
     * created sink wrapper will own specified sink
     * 
     * @param sink destination sink to wrap
     */
    explicit buffered_sink(Sink&& sink) :
    sink(std::move(sink)) { }

    /**
     * Destructor, flushes the buffer before destroy
     */
    ~buffered_sink() STATICLIB_NOEXCEPT {
        try {
            flush();
        } catch(...) {
            // ignore
        }
    }
    
    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    buffered_sink(const buffered_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    buffered_sink& operator=(const buffered_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    buffered_sink(buffered_sink&& other) STATICLIB_NOEXCEPT :
    sink(std::move(other.sink)),
    buffer(std::move(other.buffer)),
    pos(other.pos),
    avail(other.avail) {
        other.pos = 0;
        other.avail = 0;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    buffered_sink& operator=(buffered_sink&& other) STATICLIB_NOEXCEPT {
        sink = std::move(other.sink);
        buffer = std::move(other.buffer);
        pos = other.pos;
        other.pos = 0;
        avail = other.avail;
        other.avail = 0;
        return *this;
    }

    /**
     * Buffered write implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize write(span<const char> span) {
        size_t ulen = span.size();
        if (ulen >= buffer.size()) {
            write_to_sink(buffer.data(), pos);
            pos = 0;
            avail = buffer.size();
            write_to_sink(span.data(), ulen);
        } else if (ulen <= avail) {
            std::memcpy(buffer.data() + pos, span.data(), ulen);
            pos += ulen;
            if (avail > ulen) {
                avail -= ulen;
                return span.size_signed();
            } else {
                write_to_sink(buffer.data(), buffer.size());
                pos = 0;
                avail = buffer.size();
            }
        } else {
            write_to_sink(buffer.data(), pos);
            std::memcpy(buffer.data(), span.data(), ulen);
            pos = ulen;
            avail = buffer.size() - ulen;
        }
        return span.size_signed();
    }

    /**
     * Flushes the buffer to the destination sink
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        std::streamsize flushed = 0;
        if (pos > 0) {
            write_to_sink(buffer.data(), pos);
            flushed += pos;
            pos = 0;
            avail = buffer.size();
        }
        flushed += sink.flush();
        return flushed;
    }
    
    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */
    Sink& get_sink() {
        return sink;
    }

    /**
     * Buffer accessor
     * 
     * @return buffer reference
     */
    std::array<char, buf_size>& get_buffer() {
        return buffer;
    }

private:
    void write_to_sink(const char* buf, size_t length) {
        namespace sc = staticlib::config;
        size_t result = 0;
        while (result < length) {
            size_t ulen = length - result;
            if (!sl::support::is_streamsize(ulen)) {
                ulen = static_cast<size_t> (std::numeric_limits<std::streamsize>::max());
            }
            std::streamsize amt = sink.write({buf + result, static_cast<std::streamsize>(ulen)});
            if (!sl::support::is_sizet(amt)) throw io_exception(TRACEMSG(
                    "Invalid result returned by underlying 'write' operation: [" + sl::support::to_string(amt) + "]"));
            result += static_cast<size_t> (amt);
        }
    }
    
};

/**
 * Factory function for creating buffered sinks,
 * created sink wrapper will own specified sink
 * 
 * @param sink destination sink
 * @return buffered sink
 */
template <typename Sink,
        class = typename std::enable_if<!std::is_lvalue_reference<Sink>::value>::type>
buffered_sink<Sink> make_buffered_sink(Sink&& sink) {
    return buffered_sink<Sink>(std::move(sink));
}

/**
 * Factory function for creating buffered sinks,
 * created sink wrapper will NOT own specified sink
 * 
 * @param sink destination sink
 * @return buffered sink
 */
template <typename Sink>
buffered_sink<reference_sink<Sink>> make_buffered_sink(Sink& sink) {
    return buffered_sink<reference_sink<Sink>>(make_reference_sink(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_BUFFERED_SINK_HPP */

