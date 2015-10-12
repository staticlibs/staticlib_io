/* 
 * File:   unique_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 1:02 PM
 */

#ifndef STATICLIB_IO_UNIQUE_SOURCE_HPP
#define	STATICLIB_IO_UNIQUE_SOURCE_HPP

#include <ios>
#include <memory>

namespace staticlib {
namespace io {

template <typename Source>
class unique_source {
    std::unique_ptr<Source> src;
    
public:    
    unique_source(Source* src) : 
    src(src) { }
    
    unique_source(const unique_source&) = delete;
    
    unique_source& operator=(const unique_source&) = delete;

    unique_source(unique_source&& other) :
    src(std::move(other.src)) { }

    unique_source& operator=(unique_source&& other) {
        src = std::move(other.src);
        return *this;
    }
    
    std::streamsize read(char* buffer, std::streamsize length) {
        return src->read(buffer, length);
    }

    Source& get_source() {
        return *src;
    }
    
};

template <typename Source>
unique_source<Source> make_unique_source(Source* source) {
    return unique_source<Source>(source);
}

} // namespace
}

#endif	/* STATICLIB_IO_UNIQUE_SOURCE_HPP */

