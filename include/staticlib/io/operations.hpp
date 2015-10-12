/* 
 * File:   operations.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:07 PM
 */

#ifndef STATICLIB_IO_OPERATIONS_HPP
#define	STATICLIB_IO_OPERATIONS_HPP

#include <ios>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdint>

#include "staticlib/io/endian.hpp"

namespace staticlib {
namespace io {

namespace detail {

/**
 * Generic `to_string` implementation, already exists as `std::to_string`
 * in most C++11 compilers except GCC 4.8
 * 
 * @param t value to stringify
 * @return string representation of specified value
 */
template<typename T>
std::string io_to_string(T t) {
    std::stringstream ss{};
    ss << t;
    return ss.str();
}

} // namespace

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
    std::streamsize sbuflen = static_cast<std::streamsize>(buflen);
    std::streamsize result = 0;
    while (result < sbuflen) {
        std::streamsize amt = sink.write(buf + result, sbuflen - result);
        result += amt;
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
template<typename Source, typename IntType, std::streamsize source_eof = -1>
size_t read_all(Source& src, char* buf, IntType buflen) {
    std::streamsize sbuflen = static_cast<std::streamsize>(buflen);
    if (sbuflen < 0) return 0;
    std::streamsize result = 0;
    while (result < sbuflen) {
        std::streamsize amt = src.read(buf + result, sbuflen - result);
        if (source_eof != amt) {
            result += static_cast<size_t> (amt);
        } else break;
    }
    return static_cast<size_t>(result);
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
    std::streamsize sbuflen = static_cast<std::streamsize>(buflen);
    if (sbuflen < 0) return;
    size_t ulen = static_cast<size_t> (sbuflen);
    auto res = read_all(src, buf, sbuflen);
    if (res != ulen) throw std::ios_base::failure(std::string{} +
            "Read amount: [" + detail::io_to_string(res) + "]" +
            " of expected: [" + detail::io_to_string(sbuflen) + "]");
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
    std::streamsize sbuflen = static_cast<std::streamsize>(buflen);
    if (sbuflen < 0) return 0;
    size_t ulen = static_cast<size_t> (sbuflen);
    size_t result = 0;
    size_t amt;
    while (ulen == (amt = read_all(src, buf, sbuflen))) {
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

template<typename Source, typename IntTypeLen, typename IntTypeSkip>
void skip(Source& src, char* buf, IntTypeLen buflen, IntTypeSkip to_skip) {
    std::streamsize sbuflen = static_cast<std::streamsize>(buflen);
    std::streamsize sto_skip = static_cast<std::streamsize>(to_skip);
    while (sto_skip > 0) {
        uint16_t chunklen = std::min(sto_skip, sbuflen);
        io::read_exact(src, buf, chunklen);
        sto_skip -= chunklen;
    }
}

template<typename IntType, typename Source>
IntType read_16_le(Source& src) {
    uint16_t res = 0;
    read_exact(src, reinterpret_cast<char*>(std::addressof(res)), 2);
    return static_cast<IntType>(le16toh(res));
}

template<typename IntType, typename Source>
IntType read_16_be(Source& src) {
    uint16_t res = 0;
    read_exact(src, reinterpret_cast<char*>(std::addressof(res)), 2);
    return static_cast<IntType>(be16toh(res));
}

template<typename IntType, typename Source>
IntType read_32_le(Source& src) {
    uint32_t res = 0;
    read_exact(src, reinterpret_cast<char*>(std::addressof(res)), 4);
    return static_cast<IntType>(le32toh(res));
}

template<typename IntType, typename Source>
IntType read_32_be(Source& src) {
    uint32_t res = 0;
    read_exact(src, reinterpret_cast<char*>(std::addressof(res)), 4);
    return static_cast<IntType>(be32toh(res));
}

template<typename IntType, typename Source>
IntType read_64_le(Source& src) {
    uint64_t res = 0;
    read_exact(src, reinterpret_cast<char*>(std::addressof(res)), 8);
    return static_cast<IntType>(le64toh(res));
}

template<typename IntType, typename Source>
IntType read_64_be(Source& src) {
    uint64_t res = 0;
    read_exact(src, reinterpret_cast<char*>(std::addressof(res)), 8);
    return static_cast<IntType>(be64toh(res));
}

template<typename IntType, typename Sink>
void write_16_le(Sink& sink, IntType val) {
    uint16_t val16 = le16toh(static_cast<uint16_t>(val));
    write_all(sink, reinterpret_cast<char*>(std::addressof(val16)), 2);
}

template<typename IntType, typename Sink>
void write_16_be(Sink& sink, IntType val) {
    uint16_t val16 = be16toh(static_cast<uint16_t> (val));
    write_all(sink, reinterpret_cast<char*>(std::addressof(val16)), 2);
}

template<typename IntType, typename Sink>
void write_32_le(Sink& sink, IntType val) {
    uint32_t val32 = le32toh(static_cast<uint32_t> (val));
    write_all(sink, reinterpret_cast<char*>(std::addressof(val32)), 4);
}

template<typename IntType, typename Sink>
void write_32_be(Sink& sink, IntType val) {
    uint32_t val32 = be32toh(static_cast<uint32_t> (val));
    write_all(sink, reinterpret_cast<char*>(std::addressof(val32)), 4);
}

template<typename IntType, typename Sink>
void write_64_le(Sink& sink, IntType val) {
    uint64_t val64 = le64toh(static_cast<uint64_t> (val));
    write_all(sink, reinterpret_cast<char*>(std::addressof(val64)), 8);
}

template<typename IntType, typename Sink>
void write_64_be(Sink& sink, IntType val) {
    uint64_t val64 = be64toh(static_cast<uint64_t> (val));
    write_all(sink, reinterpret_cast<char*>(std::addressof(val64)), 8);
}



} // namespace
}

#endif	/* STATICLIB_IO_OPERATIONS_HPP */

