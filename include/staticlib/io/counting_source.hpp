/* 
 * File:   counting_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:23 PM
 */

#ifndef STATICLIB_IO_COUNTING_SOURCE_HPP
#define	STATICLIB_IO_COUNTING_SOURCE_HPP

#include <ios>

namespace staticlib {
namespace io {

template<typename Source>
class counting_source {
    Source src;
    size_t count = 0;
    
public:
    counting_source(Source src) :
    src(std::move(src)) { }

    counting_source(const counting_source&) = delete;

    counting_source& operator=(const counting_source&) = delete;

    counting_source(counting_source&& other) :
    src(std::move(other.src)) { }

    counting_source& operator=(counting_source&& other) {
        src = std::move(other.src);
        return *this;
    }

    std::streamsize read(char* buffer, std::streamsize length) {
        std::streamsize res = src.read(buffer, length);
        if (res > 0) {
            count += res;
        }
        return res;
    }
    
    size_t get_count() {
        return count;
    }

    Source& get_source() {
        return src;
    }

    
};

} // namespace
}

#endif	/* STATICLIB_IO_COUNTING_SOURCE_HPP */

