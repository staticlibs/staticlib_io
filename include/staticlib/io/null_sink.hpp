/* 
 * File:   null_sink.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:14 PM
 */

#ifndef STATICLIB_IONULL_SINK_HPP
#define	STATICLIB_IONULL_SINK_HPP

#include <ios>

namespace staticlib {
namespace io {

class null_sink {
public:
    null_sink() { }

    null_sink(const null_sink&) { }

    null_sink& operator=(const null_sink&) {
        return *this;
    }

    null_sink(null_sink&&) { }

    null_sink& operator=(null_sink&&) {
        return *this;
    }

    std::streamsize write(const char*, std::streamsize length) {
        // no-op
        return length;
    }

    std::streamsize flush() {
        // no-op
        return 0;
    }

};

} // namespace
}

#endif	/* STATICLIB_IONULL_SINK_HPP */

