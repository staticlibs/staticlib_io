/* 
 * File:   unique_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 1:02 PM
 */

#ifndef STATICLIB_IO_UNIQUE_SINK_HPP
#define	STATICLIB_IO_UNIQUE_SINK_HPP

#include <ios>
#include <memory>

namespace staticlib {
namespace io {

template <typename Sink>
class unique_sink {
    std::unique_ptr<Sink> sink;

public:
    unique_sink(Sink* sink) :
    sink(sink) { }

    unique_sink(const unique_sink&) = delete;

    unique_sink& operator=(const unique_sink&) = delete;

    unique_sink(unique_sink&& other) :
    sink(std::move(other.sink)) { }

    unique_sink& operator=(unique_sink&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    std::streamsize write(const char* buffer, std::streamsize length) {
        return sink->write(buffer, length);
    }
    
    void flush() {
        sink->flush();
    }

};

} // namespace
}

#endif	/* STATICLIB_IO_UNIQUE_SINK_HPP */

