/* 
 * File:   flushable_sink.hpp
 * Author: alex
 *
 * Created on October 13, 2015, 7:53 AM
 */

#ifndef STATICLIB_IO_FLUSHABLE_SINK_HPP
#define	STATICLIB_IO_FLUSHABLE_SINK_HPP

#include <ios>

namespace staticlib {
namespace io {

template<typename NonFlushableSink>
class flushable_sink {
    NonFlushableSink sink;

public:
    flushable_sink(NonFlushableSink sink) :
    sink(std::move(sink)) { }

    flushable_sink(const flushable_sink&) = delete;

    flushable_sink& operator=(const flushable_sink&) = delete;

    flushable_sink(flushable_sink&& other) :
    sink(std::move(other.sink)) { }

    flushable_sink& operator=(flushable_sink&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    std::streamsize write(const char* buffer, std::streamsize length) {
        return sink.write(buffer, length);
    }

    std::streamsize flush() {
        return 0;
    }

    NonFlushableSink& get_sink() {
        return sink;
    }

};

template <typename NonFlushableSink>
flushable_sink<NonFlushableSink> make_flushable_sink(NonFlushableSink&& sink) {
    return flushable_sink<NonFlushableSink>(std::move(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_FLUSHABLE_SINK_HPP */

