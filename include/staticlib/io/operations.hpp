/* 
 * File:   operations.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:07 PM
 */

#ifndef STATICLIB_IO_OPERATIONS_HPP
#define	STATICLIB_IO_OPERATIONS_HPP

#include <string>

#include "staticlib/io/IoException.hpp"

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
template<typename Sink>
void write_all(Sink& sink, const char* buf, std::streamsize buflen) {
    std::streamsize result = 0;
    while (result < buflen) {
        std::streamsize amt = sink.write(buf + result, buflen - result);
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
    write_all(sink, str.c_str(), static_cast<std::streamsize> (str.length()));
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
template<typename Source>
size_t read_all(Source& src, char* buf, std::streamsize buflen) {
    if (buflen < 0) return 0;
    size_t ulen = static_cast<size_t> (buflen);
    size_t result = 0;
    while (result < ulen) {
        std::streamsize amt = src.read(buf + result, buflen - result);
        if (0 == amt && std::char_traits<char>::eof() == src.sbumpc()) break;
        result += static_cast<size_t> (amt);
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
template<typename Source>
void read_exact(Source& src, char* buf, std::streamsize buflen) {
    if (buflen < 0) return;
    size_t ulen = static_cast<size_t> (buflen);
    auto res = read_all(src, buf, buflen);
    if (res != ulen) throw std::ios_base::failure(std::string{} +
            "Read amount: [" + detail::io_to_string(res) + "]" +
            " of expected: [" + detail::io_to_string(buflen) + "]");
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
template<typename Source, typename Sink>
size_t copy_all(Source& src, Sink& sink, char* buf, std::streamsize buflen) {
    if (buflen < 0) return 0;
    size_t ulen = static_cast<size_t> (buflen);
    size_t result = 0;
    size_t amt;
    while (ulen == (amt = read_all(src, buf, buflen))) {
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

} // namespace
}

#endif	/* STATICLIB_IO_OPERATIONS_HPP */

