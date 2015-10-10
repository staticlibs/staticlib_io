/* 
 * File:   streambuf_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:32 PM
 */

#ifndef STATICLIB_IO_STREAMBUF_SINK_HPP
#define	STATICLIB_IO_STREAMBUF_SINK_HPP

#include <ios>
#include <streambuf>

namespace staticlib {
namespace io {

class streambuf_sink {
    std::streambuf* streambuf;

public:
    streambuf_sink(std::streambuf* streambuf) :
    streambuf(streambuf) { }

    streambuf_sink(const streambuf_sink&) = delete;

    streambuf_sink& operator=(const streambuf_sink&) = delete;

    streambuf_sink(streambuf_sink&& other) :
    streambuf(other.streambuf) { }

    streambuf_sink& operator=(streambuf_sink&& other) {
        streambuf = other.streambuf;
        return *this;
    }

    std::streamsize write(const char* buffer, std::streamsize length) {
        return streambuf->sputn(buffer, length);
    }

    std::streamsize flush() {
        return streambuf->pubsync();
    }

};

} // namespace
}


#endif	/* STATICLIB_IO_STREAMBUF_SINK_HPP */

