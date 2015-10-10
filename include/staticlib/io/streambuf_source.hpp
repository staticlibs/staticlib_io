/* 
 * File:   streambuf_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:32 PM
 */

#ifndef STATICLIB_IO_STREAMBUF_SOURCE_HPP
#define	STATICLIB_IO_STREAMBUF_SOURCE_HPP

#include <ios>
#include <streambuf>

namespace staticlib {
namespace io {

class streambuf_source {
    std::streambuf* streambuf;
    
public:
    streambuf_source(std::streambuf* streambuf) :
    streambuf(streambuf) { }

    streambuf_source(const streambuf_source&) = delete;

    streambuf_source& operator=(const streambuf_source&) = delete;

    streambuf_source(streambuf_source&& other) :
    streambuf(other.streambuf) { }

    streambuf_source& operator=(streambuf_source&& other) {
        streambuf = other.streambuf;
        return *this;
    }

    std::streamsize read(char* buffer, std::streamsize length) {
        return streambuf->sgetn(buffer, length);
    }
    
};

} // namespace
}

#endif	/* STATICLIB_IO_STREAMBUF_SOURCE_HPP */

