/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#include "staticlib/config/noexcept.hpp"
#include "staticlib/config/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that holds a "std::shared_ptr" to the underlying source
 */
template <typename Source>
class shared_source {
    /**
     * Input source
     */
    std::shared_ptr<Source> src;

public:
    /**
     * Constructor
     * 
     * @param src input source
     */
    shared_source(std::shared_ptr<Source> src) :
    src(std::move(src)) { }

    /**
     * Copy constructor
     * 
     * @param other instance
     */    
    shared_source(const shared_source& other) :
    src(other.src) { }

    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance
     */    
    shared_source& operator=(const shared_source& other) {
        src = other.src;
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */    
    shared_source(shared_source&& other) STATICLIB_NOEXCEPT :
    src(std::move(other.src)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */    
    shared_source& operator=(shared_source&& other) STATICLIB_NOEXCEPT {
        src = std::move(other.src);
        return *this;
    }

    /**
     * Read implementation delegated to the underlying source
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize read(staticlib::config::span<char> span) {
        return src->read(span);
    }
    
    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return *src;
    }

};

/**
 * Factory function for creating shared sources
 * 
 * @param source input source
 * @return shared source
 */
template <typename Source>
shared_source<Source> make_shared_source(std::shared_ptr<Source> source) {
    return shared_source<Source>(std::move(source));
}

} // namespace
}

#endif	/* STATICLIB_IO_SHARED_SOURCE_HPP */

