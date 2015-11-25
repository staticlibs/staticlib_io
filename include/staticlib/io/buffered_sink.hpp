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

#include <array>
#include <ios>
#include <utility>
#include <cstring>

namespace staticlib {
namespace io {

/**
 * Sink wrapper that buffers the output
 */
template <typename Sink, std::size_t buf_size = 8192>
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
     * Constructor, wraps specified sink
     * 
     * @param sink destination sink to wrap
     */
    buffered_sink(Sink sink) :
    sink(std::move(sink)) { }

    /**
     * Destructor, flushes the buffer before destroy
     */
    ~buffered_sink() {
        flush();
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
    buffered_sink(buffered_sink&& other) :
    sink(std::move(other.sink)),
    buffer(std::move(other.buffer)),
    pos(other.pos),
    avail(other.avail) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    buffered_sink& operator=(buffered_sink&& other) {
        sink = std::move(other.sink);
        buffer = std::move(other.buffer);
        pos = other.pos;
        avail = other.avail;
        return *this;
    }

    /**
     * Buffered write implementation
     * 
     * @param b source buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize write(const char* b, std::streamsize length) {
        size_t ulen = static_cast<size_t>(length);
        if (ulen >= buffer.size()) {
            write_to_sink(buffer.data(), 0, pos);
            pos = 0;
            avail = buffer.size();
            write_to_sink(b, 0, ulen);
        } else if (ulen <= avail) {
            std::memcpy(buffer.data() + pos, b, ulen);
            pos += ulen;
            if (avail > ulen) {
                avail -= ulen;
                return length;
            } else {
                write_to_sink(buffer.data(), 0, buffer.size());
                pos = 0;
                avail = buffer.size();
            }
        } else {
            write_to_sink(buffer.data(), 0, pos);
            std::memcpy(buffer.data(), b, ulen);
            pos = ulen;
            avail = buffer.size() - ulen;
        }
        return length;
    }

    /**
     * Flushes the buffer to the destination sink
     * 
     * @return number of bytes flushed
     */
    std::streamsize flush() {
        std::streamsize flushed = 0;
        if (pos > 0) {
            write_to_sink(buffer.data(), 0, pos);
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
    void write_to_sink(const char* buf, std::streamsize offset, size_t length) {
        size_t result = 0;
        while (result < length) {
            auto amt = sink.write(buf + offset + result, length - result);
            result += static_cast<size_t> (amt);
        }
    }
    
};

/**
 * Factory function for creating buffered sinks
 * 
 * @param sink destination sink
 * @return buffered sink
 */
template <typename Sink>
buffered_sink<Sink> make_buffered_sink(Sink&& sink) {
    return buffered_sink<Sink>(std::move(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_BUFFERED_SINK_HPP */

