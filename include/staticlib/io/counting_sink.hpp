/* 
 * File:   counting_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:23 PM
 */

#ifndef STATICLIB_IO_COUNTING_SINK_HPP
#define	STATICLIB_IO_COUNTING_SINK_HPP

#include <ios>

namespace staticlib {
namespace io {

template<typename Sink>
class counting_sink {
    Sink sink;
    size_t count = 0;

public:
    counting_sink(Sink sink) :
    sink(std::move(sink)) { }

    counting_sink(const counting_sink&) = delete;

    counting_sink& operator=(const counting_sink&) = delete;

    counting_sink(counting_sink&& other) :
    sink(std::move(other.sink)) { }

    counting_sink& operator=(counting_sink&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    std::streamsize write(const char* buffer, std::streamsize length) {
        std::streamsize res = sink.write(buffer, length);
        if (res > 0) {
            count += res;
        }
        return res;
    }

    std::streamsize flush() {
        return sink.flush();
    }

    size_t get_count() {
        return count;
    }

    Sink& get_sink() {
        return sink;
    }

};

} // namespace
}

#endif	/* STATICLIB_IO_COUNTING_SINK_HPP */

