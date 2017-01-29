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
 * File:   operations.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:07 PM
 */

#ifndef STATICLIB_IO_OPERATIONS_HPP
#define	STATICLIB_IO_OPERATIONS_HPP

#include <algorithm>
#include <ios>
#include <sstream>
#include <string>
#include <cstdint>

#include "staticlib/config.hpp"

#include "staticlib/io/IOException.hpp"

namespace staticlib {
namespace io {

/**
 * Writes buffer contents to the specified Sink ensuring that 
 * the specified number of bytes are written to Sink 
 * (making multiple write calls to Sink if needed).
 * 
 * @param sink iostreams sink
 * @param buf data buffer
 * @param buflen data buffer length
 */
template<typename Sink, typename IntType>
void write_all(Sink& sink, const char* buf, IntType buflen) {
    namespace sc = staticlib::config;
    if (!(sc::is_sizet(buflen) && sc::is_streamsize(buflen))) throw IOException(TRACEMSG(
            "Invalid 'write_all' parameter specified, buflen: [" + sc::to_string(buflen) + "]"));
    size_t ulen = static_cast<size_t>(buflen);
    size_t result = 0;
    while (result < ulen) {
        std::streamsize amt = sink.write(buf + result, static_cast<std::streamsize>(ulen - result));
        if (!sc::is_sizet(amt)) throw IOException(TRACEMSG(
                "Invalid result returned by underlying 'write' operation: [" + sc::to_string(amt) + "]"));
        result += static_cast<size_t>(amt);
    }
}

/**
 * Writes string contents to the specified Sink ensuring that 
 * the specified number of bytes are written to Sink 
 * (making multiple write calls to Sink if needed).
 * 
 * @param sink iostreams sink
 * @param str data string
 */
template<typename Sink>
void write_all(Sink& sink, const std::string& str) {
    write_all(sink, str.c_str(), str.length());
}


/**
 * Reads specified number of bytes from the Source into the buffer.
 * May read less then specified bytes only if Source became exhausted.
 * 
 * @param src iostreams source
 * @param buf dest buffer
 * @param buflen number of bytes to read from source
 * @return number of bytes read
 */
template<typename Source, typename IntType>
size_t read_all(Source& src, char* buf, IntType buflen) {
    namespace sc = staticlib::config;
    if (!(sc::is_sizet(buflen) && sc::is_streamsize(buflen))) throw IOException(TRACEMSG(
            "Invalid 'read_all' parameter specified, buflen: [" + sc::to_string(buflen) + "]"));
    size_t ulen = static_cast<size_t> (buflen);
    size_t result = 0;
    while (result < ulen) {
        std::streamsize amt = src.read(buf + result, static_cast<std::streamsize> (ulen - result));
        if (std::char_traits<char>::eof() != amt) {
            if (!sc::is_sizet(amt)) throw IOException(TRACEMSG(
                    "Invalid result returned by underlying 'read' operation: [" + sc::to_string(amt) + "]"));
            result += static_cast<size_t> (amt);
        } else break;
    }
    return result;
}

/**
 * Reads specified number of bytes from the Source into the buffer.
 * Will throw exception if exact specified amount won't be available.
 * 
 * @param src iostreams source
 * @param buf dest buffer
 * @param buflen number of bytes to read from source
 */
template<typename Source, typename IntType>
void read_exact(Source& src, char* buf, IntType buflen) {
    namespace sc = staticlib::config;
    size_t res = read_all(src, buf, buflen);
    if (res != static_cast<size_t>(buflen)) throw IOException(TRACEMSG(
            "Read amount: [" + sc::to_string(res) + "]" +
            " of expected: [" + sc::to_string(buflen) + "]"));
}

/**
 * Copies data from Source to Sink using specified buffer until 
 * source will be exhausted.
 * 
 * @param src iostreams source
 * @param sink iostreams sink
 * @param buf dest buffer
 * @param buflen number of bytes to read from source
 * @return number of bytes copied
 */
template<typename Source, typename Sink, typename IntType>
size_t copy_all(Source& src, Sink& sink, char* buf, IntType buflen) {
    namespace sc = staticlib::config;
    if (!(sc::is_sizet(buflen) && sc::is_streamsize(buflen))) throw IOException(TRACEMSG(
            "Invalid 'copy_all' parameter specified, buflen: [" + sc::to_string(buflen) + "]"));
    size_t ulen = static_cast<size_t> (buflen);
    size_t result = 0;
    size_t amt;
    while (ulen == (amt = read_all(src, buf, ulen))) {
        write_all(sink, buf, amt);
        result += amt;
    }
    // tail
    if (amt > 0) {
        write_all(sink, buf, amt);
        result += amt;
    }
    return result;
}

/**
 * Skips specified number of bytes reading data repeatedly from specified
 * source into specified buffer
 * 
 * @param src input source
 * @param buf buffer for temporary data
 * @param buflen buffer length
 * @param to_skip number of bytes to skip
 */
template<typename Source, typename IntTypeLen, typename IntTypeSkip>
void skip(Source& src, char* buf, IntTypeLen buflen, IntTypeSkip to_skip) {
    namespace sc = staticlib::config;
    if (!(sc::is_sizet(buflen) && sc::is_streamsize(buflen))) throw IOException(TRACEMSG(
            "Invalid 'skip' parameter specified, buflen: [" + sc::to_string(buflen) + "]"));
    if (!(sc::is_sizet(to_skip) && sc::is_streamsize(to_skip))) throw IOException(TRACEMSG(
            "Invalid 'skip' parameter specified, to_skip: [" + sc::to_string(to_skip) + "]"));
    size_t ulen = static_cast<size_t>(buflen);
    size_t uskip = static_cast<size_t>(to_skip);
    while (uskip > 0) {
        size_t chunklen = uskip <= ulen ? uskip : ulen;
        io::read_exact(src, buf, chunklen);
        uskip -= chunklen;
    }
}

} // namespace
}

#endif	/* STATICLIB_IO_OPERATIONS_HPP */

