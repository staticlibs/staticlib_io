/* 
 * File:   copying_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:24 PM
 */

#ifndef STATICLIB_IO_COPYING_SOURCE_HPP
#define	STATICLIB_IO_COPYING_SOURCE_HPP

#include <ios>

#include "staticlib/io/operations.hpp"

namespace staticlib {
namespace io {

template<typename Source, typename Sink>
class counting_source {
    Source src;
    Sink sink;

public:
    counting_source(Source src, Sink sink) :
    src(std::move(src)),
    sink(std::move(sink)) { }

    counting_source(const counting_source&) = delete;

    counting_source& operator=(const counting_source&) = delete;

    counting_source(counting_source&& other) :
    src(std::move(other.src)),
    sink(std::move(other.sink))  { }

    counting_source& operator=(counting_source&& other) {
        src = std::move(other.src);
        sink = std::move(other.sink);
        sink = std::move(other.sink);
        return *this;
    }

    std::streamsize read(char* buffer, std::streamsize length) {
        std::streamsize res = src.read(buffer, length);
        if (std::char_traits<char>::eof() != res) {
            write_all(sink, buffer, res);
        }
        return res;
    }

    void flush() {
        sink.flush();
    }
    
};

} // namespace
}

#endif	/* STATICLIB_IO_COPYING_SOURCE_HPP */

