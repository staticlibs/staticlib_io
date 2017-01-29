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
 * File:   counting_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:23 PM
 */

#ifndef STATICLIB_IO_COUNTING_SOURCE_HPP
#define	STATICLIB_IO_COUNTING_SOURCE_HPP

#include <ios>

#include "staticlib/config.hpp"

#include "staticlib/io/reference_source.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that counts the number of bytes read through it
 */
template<typename Source>
class counting_source {
    /**
     * Input source
     */
    Source src;
    /**
     * Number of bytes read
     */
    size_t count = 0;
    
public:
    /**
     * Constructor,
     * created source wrapper will own specified source
     * 
     * @param src input source
     */
    counting_source(Source&& src) :
    src(std::move(src)) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    counting_source(const counting_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    counting_source& operator=(const counting_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    counting_source(counting_source&& other) :
    src(std::move(other.src)),
    count(other.count) {
        other.count = 0;
    }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    counting_source& operator=(counting_source&& other) {
        src = std::move(other.src);
        count = other.count;
        other.count = 0;
        return *this;
    }

    /**
     * Counting read implementation
     * 
     * @param buffer output buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize read(char* buffer, std::streamsize length) {
        namespace sc = staticlib::config;
        std::streamsize res = src.read(buffer, length);
        if (sc::is_sizet(res)) {
            count += static_cast<size_t>(res);
        }
        return res;
    }

    /**
     * Returns number of bytes read through this instance
     * 
     * @return number of bytes read through this instance
     */
    size_t get_count() {
        return count;
    }

    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return src;
    }
    
};

/**
 * Factory function for creating counting sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return counting source
 */
template <typename Source,
        class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
counting_source<Source> make_counting_source(Source&& source) {
    return counting_source<Source>(std::move(source));
}

/**
 * Factory function for creating counting sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return counting source
 */
template <typename Source>
counting_source<reference_source<Source>> make_counting_source(Source& source) {
    return counting_source<reference_source<Source>>(make_reference_source(source));
}

} // namespace
}

#endif	/* STATICLIB_IO_COUNTING_SOURCE_HPP */

