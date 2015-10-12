/* 
 * File:   copying_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:24 PM
 */

#ifndef STATICLIB_IO_COPYING_SOURCE_HPP
#define	STATICLIB_IO_COPYING_SOURCE_HPP

#include <ios>
#include <utility>

#include "staticlib/io/operations.hpp"

namespace staticlib {
namespace io {

template<typename Source, typename Sink>
class copying_source {
    Source src;
    Sink sink;

public:
    copying_source(Source src, Sink sink) :
    src(std::move(src)),
    sink(std::move(sink)) { }

    copying_source(const copying_source&) = delete;

    copying_source& operator=(const copying_source&) = delete;

    copying_source(copying_source&& other) :
    src(std::move(other.src)),
    sink(std::move(other.sink))  { }

    copying_source& operator=(copying_source&& other) {
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

    Source& get_source() {
        return src;
    }

    Sink& get_sink() {
        return sink;
    }
    
};

template<typename Source, typename Sink>
copying_source<Source, Sink> make_copying_source(Source&& source, Sink&& sink) {
    return copying_source<Source, Sink>(std::move(source), std::move(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_COPYING_SOURCE_HPP */

