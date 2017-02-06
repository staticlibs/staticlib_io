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
 * File:   unique_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 1:02 PM
 */

#ifndef STATICLIB_IO_UNIQUE_SOURCE_HPP
#define	STATICLIB_IO_UNIQUE_SOURCE_HPP

#include <ios>
#include <memory>

#include "staticlib/config/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that holds a "std::unique_ptr" to the underlying source
 */
template <typename Source>
class unique_source {
    /**
     * Input source
     */
    std::unique_ptr<Source> src;
    
public:    
    /**
     * Constructor
     * 
     * @param src input source
     */
    explicit unique_source(Source* src) : 
    src(src) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */        
    unique_source(const unique_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */        
    unique_source& operator=(const unique_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */        
    unique_source(unique_source&& other) :
    src(std::move(other.src)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */        
    unique_source& operator=(unique_source&& other) {
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
 * Factory function for creating unique sources
 * 
 * @param source input source
 * @return unique source
 */
template <typename Source>
unique_source<Source> make_unique_source(Source* source) {
    return unique_source<Source>(source);
}

} // namespace
}

#endif	/* STATICLIB_IO_UNIQUE_SOURCE_HPP */

