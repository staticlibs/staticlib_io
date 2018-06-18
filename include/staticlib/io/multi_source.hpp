/*
 * Copyright 2018, alex at staticlibs.net
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
 * File:   multi_source.hpp
 * Author: alex
 *
 * Created on June 18, 2018, 11:05 AM
 */

#ifndef STATICLIB_IO_MULTI_SOURCE_HPP
#define STATICLIB_IO_MULTI_SOURCE_HPP

#include <ios>
#include <vector>

#include "staticlib/config.hpp"

#include "staticlib/io/reference_source.hpp"
#include "staticlib/io/span.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that contains a range of sources to read data from
 */
template<typename Range>
class multi_source {
    /**
     * Input source
     */
    Range sources;
    /**
     * Current source iterator
     */
    typename Range::iterator it;

public:
    /**
     * Constructor
     * 
     * @param sources_range input sources
     */
    multi_source(Range&& sources_range):
    sources(std::move(sources_range)),
    it(sources.begin()) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    multi_source(const multi_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    multi_source& operator=(const multi_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    multi_source(multi_source&& other) STATICLIB_NOEXCEPT :
    sources(std::move(other.sources)),
    it(std::move(other.it))  { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    multi_source& operator=(multi_source&& other) STATICLIB_NOEXCEPT {
        sources = std::move(other.sources);
        it = std::move(other.it);
        return *this;
    }

    /**
     * Read implementation over the range of sources
     * 
     * @param span buffer span
     * @return number of bytes processed
     */
    std::streamsize read(span<char> span) {
        while (sources.end() != it) {
            std::streamsize read = it->read(span);
            if (std::char_traits<char>::eof() != read) {
                return read;
            }
            ++it;
        }
        return std::char_traits<char>::eof();
    }

};

/**
 * Factory function for creating multi sources,
 * created source wrapper will own specified sources
 * 
 * @param range input sources
 * @return multi source
 */
template <typename Range,
class = typename std::enable_if<!std::is_lvalue_reference<Range>::value>::type>
multi_source<Range> make_multi_source(Range&& range) {
    return multi_source<Range>(std::move(range));
}

/**
 * Factory function for creating multi sources,
 * created source wrapper will NOT own specified sources
 * 
 * @param range input sources
 * @return multi source
 */
template <typename Range>
multi_source<std::vector<reference_source<typename Range::value_type>>>
make_multi_source(Range& range) {
    auto vec = std::vector<reference_source<typename Range::value_type>>();
    for (auto& el : range) {
        vec.emplace_back(make_reference_source(el));
    }
    return multi_source<std::vector<reference_source<typename Range::value_type>>> (std::move(vec));
}

} // namespace
}

#endif /* STATICLIB_IO_MULTI_SOURCE_HPP */

