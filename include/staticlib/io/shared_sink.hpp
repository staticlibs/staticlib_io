/* 
 * File:   shared_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:41 PM
 */

#ifndef STATICLIB_IO_SHARED_SINK_HPP
#define	STATICLIB_IO_SHARED_SINK_HPP

namespace staticlib {
namespace io {

template <typename Sink>
class shared_sink {
    std::shared_ptr<Sink> sink;

public:

    shared_sink(std::shared_ptr<Sink> sink) :
    sink(std::move(sink)) { }

    shared_sink(const shared_sink& other) :
    sink(other.sink) { }

    shared_sink& operator=(const shared_sink& other) {
        sink = other.sink;
        return *this;
    }

    shared_sink(shared_sink&& other) :
    sink(std::move(other.sink)) { }

    shared_sink& operator=(shared_sink&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    std::streamsize write(const char* buffer, std::streamsize length) {
        return sink->write(buffer, length);
    }

    std::streamsize flush() {
        return sink->flush();
    }
    
    Sink& get_sink() {
        return *sink;
    }
    
};

} // namespace
}

#endif	/* STATICLIB_IO_SHARED_SINK_HPP */

