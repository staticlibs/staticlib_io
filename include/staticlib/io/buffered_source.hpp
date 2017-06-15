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

#include <cstring>
#include <array>
#include <ios>
#include <string>
#include <utility>

#include "staticlib/config.hpp"

#include "staticlib/io/io_exception.hpp"
#include "staticlib/io/reference_source.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that buffers the input
 */
template <typename Source, std::size_t buf_size = 4096>
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
     * Constructor,
     * created source wrapper will own specified source
     * 
     * @param src input source
     */
    explicit buffered_source(Source&& src) :
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
    buffered_source(buffered_source&& other) STATICLIB_NOEXCEPT :
    src(std::move(other.src)),
    exhausted(other.exhausted),
    buffer(std::move(other.buffer)),
    pos(other.pos),
    avail(other.avail) { 
        other.exhausted = true;
        other.pos = 0;
        other.avail = 0;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    buffered_source& operator=(buffered_source&& other) STATICLIB_NOEXCEPT {
        src = std::move(other.src);
        exhausted = other.exhausted;
        other.exhausted = true;
        buffer = std::move(other.buffer);
        pos = other.pos;
        other.pos = 0;
        avail = other.avail;
        other.avail = 0;
        return *this;
    }

    /**
     * Buffered read implementation
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize read(span<char> span) {
        size_t ulen = span.size();
        // return from buffer
        if (ulen <= avail) {
            std::memcpy(span.data(), buffer.data() + pos, ulen);
            pos += ulen;
            avail -= ulen;
            return span.size_signed();
        }
        // copy all data already available
        size_t uhead = avail;
        if (!sl::support::is_streamsize(uhead)) {
            uhead = static_cast<size_t> (std::numeric_limits<std::streamsize>::max());
        }
        if (uhead > 0) {
            std::memcpy(span.data(), buffer.data() + pos, uhead);
        }
        pos = 0;
        avail = 0;
        std::streamsize head = static_cast<std::streamsize> (uhead);
        // try to guess whether to do direct read, or fill buffer first
        if (ulen > buffer.size()) {
            // read directly into the destination
            size_t result = read_into_buffer(span.data(), uhead, ulen - uhead);
            size_t out = result + uhead;
            return out > 0 ? out : std::char_traits<char>::eof();
        }
        // fill buffer
        avail = read_into_buffer(buffer.data(), 0, buffer.size());
        if (avail > 0) {
            // copy tail from buffer
            size_t to_read_req = ulen - uhead;
            size_t to_read = to_read_req <= avail ? to_read_req : avail;
            if (!sl::support::is_streamsize(to_read + uhead)) {
                to_read = static_cast<size_t> (std::numeric_limits<std::streamsize>::max()) - uhead;
            }
            std::memcpy(span.data() + uhead, buffer.data(), to_read);
            pos = to_read;
            avail -= to_read;
            // note that head >= 0, and to_read > 0
            return static_cast<std::streamsize>(to_read + uhead);
        }
        return head == 0 ? std::char_traits<char>::eof() : head;
    }
    
    /**
     * Reads underlying source until specified line ending is met
     * or length threshold exceeded. Lines consisting solely of
     * line endings will be ignored.
     * 
     * @param ending line ending
     * @param max_length length threshold
     * @return line without endline separator or empty string on EOF
     */
    std::string read_line(const std::string& ending = "\n", size_t max_length = (1<<16)) {
        std::string line;
        char ch;
        while(line.length() < max_length && 1 == read({std::addressof(ch), 1})) {
            line.push_back(ch);
            if (ending.length() > 0 && ch == ending.back() && line.length() >= ending.length()) {
                bool matched = true;
                size_t sdiff = line.length() - ending.length();
                for (int i = static_cast<int>(ending.length()) - 2; i >= 0; i--) {
                    if (ending[i] != line[sdiff + i]) {
                        matched = false;
                        break;
                    }
                }
                if (matched) {
                    for (size_t i = 0; i < ending.length(); i++) {
                        line.pop_back();
                    }
                    if (line.length() > 0) {
                        break;
                    }
                }
            }
        }
        return line;
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
    size_t read_into_buffer(char* buf, size_t offset, size_t length) {
        if (!exhausted) {
            size_t result = 0;
            while (result < length) {
                size_t ulen = length - result;
                if (!sl::support::is_streamsize(ulen)) {
                    ulen = static_cast<size_t> (std::numeric_limits<std::streamsize>::max());
                }
                std::streamsize amt = src.read({buf + offset + result, static_cast<std::streamsize> (ulen)});
                if (std::char_traits<char>::eof() != amt) {
                    if (!sl::support::is_sizet(amt)) throw io_exception(TRACEMSG(
                            "Invalid result returned by underlying 'read' operation: [" + sl::support::to_string(amt) + "]"));
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
 * Factory function for creating buffered sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return buffered source
 */
template <typename Source,
        class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
buffered_source<Source> make_buffered_source(Source&& source) {
    return buffered_source<Source>(std::move(source));
}

/**
 * Factory function for creating buffered sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return buffered source
 */
template <typename Source>
buffered_source<reference_source<Source>> make_buffered_source(Source& source) {
    return buffered_source<reference_source<Source>>(make_reference_source(source));
}

} // namespace
}

#endif	/* STATICLIB_IO_BUFFERED_SOURCE_HPP */

