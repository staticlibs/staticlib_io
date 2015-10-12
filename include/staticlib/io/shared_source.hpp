/* 
 * File:   shared_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:41 PM
 */

#ifndef STATICLIB_IO_SHARED_SOURCE_HPP
#define	STATICLIB_IO_SHARED_SOURCE_HPP

#include <ios>
#include <memory>

namespace staticlib {
namespace io {

template <typename Source>
class shared_source {
    std::shared_ptr<Source> src;

public:

    shared_source(std::shared_ptr<Source> src) :
    src(std::move(src)) { }

    shared_source(const shared_source& other) :
    src(other.src) { }

    shared_source& operator=(const shared_source& other) {
        src = other.src;
        return *this;
    }

    shared_source(shared_source&& other) :
    src(std::move(other.src)) { }

    shared_source& operator=(shared_source&& other) {
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
shared_source<Source> make_shared_source(std::shared_ptr<Source> source) {
    return shared_source<Source>(std::move(source));
}

} // namespace
}

#endif	/* STATICLIB_IO_SHARED_SOURCE_HPP */

