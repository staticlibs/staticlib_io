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
 * File:   buffered_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:15 PM
 */

#ifndef STATICLIB_IO_BUFFERED_SOURCE_HPP
#define	STATICLIB_IO_BUFFERED_SOURCE_HPP

#include <array>
#include <ios>
#include <utility>
#include <cstring>

namespace staticlib {
namespace io {

/**
 * Source wrapper that buffers the input
 */
template <typename Source, std::size_t buf_size = 8192>
class buffered_source {
    /**
     * Input source
     */
    Source src;
    /**
     * Whether input source was exhausted
     */
    bool exhausted = false;

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
     * Constructor
     * 
     * @param src input source
     */
    buffered_source(Source src) :
    src(std::move(src)) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    buffered_source(const buffered_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    buffered_source& operator=(const buffered_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    buffered_source(buffered_source&& other) :
    src(std::move(other.src)),
    exhausted(other.exhausted),
    buffer(std::move(other.buffer)),
    pos(other.pos),
    avail(other.avail) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    buffered_source& operator=(buffered_source&& other) {
        src = std::move(other.src);
        exhausted = other.exhausted;
        buffer = std::move(other.buffer);
        pos = other.pos;
        avail = other.avail;
        return *this;
    }

    /**
     * Buffered read implementation
     * 
     * @param buf output buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize read(char* buf, std::streamsize length) {
        size_t ulen = static_cast<size_t>(length);
        // return from buffer
        if (ulen <= avail) {
            std::memcpy(buf, buffer.data() + pos, ulen);
            pos += ulen;
            avail -= ulen;
            return length;
        }
        // copy all data already available
        size_t uhead = avail;
        if (uhead > 0) {
            std::memcpy(buf, buffer.data() + pos, uhead);
        }
        pos = 0;
        avail = 0;
        std::streamsize head = static_cast<std::streamsize> (uhead);
        // try to guess whether to do direct read, or fill buffer first
        if (ulen > buffer.size()) {
            // read directly into the destination
            size_t result = read_into_buffer(buf, head, ulen - uhead);
            size_t out = result + uhead;
            return out > 0 ? out : std::char_traits<char>::eof();
        }
        // fill buffer
        avail = read_into_buffer(buffer.data(), 0, buffer.size());
        if (avail > 0) {
            // copy tail from buffer
            size_t to_read = std::min(ulen - uhead, avail);
            std::memcpy(buf + head, buffer.data(), to_read);
            pos = to_read;
            avail -= to_read;
            // note that head >= 0, and to_read > 0
            return to_read + head;
        }
        return head == 0 ? std::char_traits<char>::eof() : head;
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
     * Buffer accessor
     * 
     * @return buffer reference
     */
    std::array<char, buf_size>& get_buffer() {
        return buffer;
    }
    
private:
    // repeatable source read logic
    size_t read_into_buffer(char* buf, std::streamsize offset, size_t length) {
        if (!exhausted) {
            size_t result = 0;
            while (result < length) {
                auto amt = src.read(buf + offset + result, length - result);
                if (std::char_traits<char>::eof() != amt) {
                    result += static_cast<size_t> (amt);
                } else {
                    exhausted = true;
                    break;
                }
            }
            return result;
        }
        return 0;
    }
};

/**
 * Factory function for creating buffered sources
 * 
 * @param source input source
 * @return buffered source
 */
template <typename Source>
buffered_source<Source> make_buffered_source(Source&& source) {
    return buffered_source<Source>(std::move(source));
}

} // namespace
}

#endif	/* STATICLIB_IO_BUFFERED_SOURCE_HPP */

