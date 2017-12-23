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
 * File:   reference_source.hpp
 * Author: alex
 *
 * Created on December 13, 2015, 2:18 PM
 */

#ifndef STATICLIB_IO_REFERENCE_SOURCE_HPP
#define STATICLIB_IO_REFERENCE_SOURCE_HPP

#include <ios>
#include <functional>

#include "staticlib/config.hpp"

#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that holds a "std::reference_wrapper" to the underlying source
 */
template <typename Source>
class reference_source {
    /**
     * Input source
     */
    std::reference_wrapper<Source> src;

public:

    /**
     * Constructor
     * 
     * @param src input source
     */
    explicit reference_source(Source& src) :
    src(src) { }

    /**
     * Copy constructor
     * 
     * @param other instance
     */
    reference_source(const reference_source& other) :
    src(other.src) { }

    /**
     * Copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    reference_source& operator=(const reference_source& other) {
        this->src = other.src;
        return *this;
    }

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    reference_source(reference_source&& other) STATICLIB_NOEXCEPT :
    src(std::move(other.src)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    reference_source& operator=(reference_source&& other) STATICLIB_NOEXCEPT {
        src = std::move(other.src);
        return *this;
    }

    /**
     * Read implementation delegated to the underlying source
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize read(span<char> span) {
        return src.get().read(span);
    }

    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return src.get();
    }

};

/**
 * Factory function for creating reference sources
 * 
 * @param source input source
 * @return reference source
 */
template <typename Source>
reference_source<Source> make_reference_source(Source& source) {
    return reference_source<Source>(source);
}

} // namespace
}


#endif /* STATICLIB_IO_REFERENCE_SOURCE_HPP */

