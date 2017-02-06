/*
 * Copyright 2016, alex at staticlibs.net
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
 * File:   source_istream.hpp
 * Author: alex
 *
 * Created on April 15, 2016, 10:30 AM
 */

#ifndef STATICLIB_IO_SOURCE_ISTREAM_HPP
#define	STATICLIB_IO_SOURCE_ISTREAM_HPP

#include <ios>
#include <istream>
#include <memory>

#include "staticlib/io/unbuffered_streambuf.hpp"
#include "staticlib/io/reference_source.hpp"

namespace staticlib {
namespace io {

/**
 * Helper template that wraps an arbitrary source
 * into owning stream instance. That stream may be used by client
 * only to obtain unbuffered streambuf from it.
 */
template <typename Source>
class source_istream : public std::istream {
    /**
     * Unbuffered streambuf
     */
    staticlib::io::unbuffered_istreambuf<Source> streambuf;

public:

    /**
     * Constructor
     * 
     * @param source arbitrary source
     */
    explicit source_istream(Source&& source) :
    std::istream(std::addressof(streambuf)),
    streambuf(std::move(source)) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    source_istream(const source_istream&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    source_istream& operator=(const source_istream&) = delete;

    /**
     * Deleted move constructor
     * 
     * @param other instance
     */
    source_istream(source_istream&&) = delete;

    /**
     * Deleted move assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    source_istream& operator=(source_istream&&) = delete;

};

/**
 * Factory function for creating source istream unique pointers,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return source istream pointer
 */
template <typename Source,
        class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
std::unique_ptr<std::istream> make_source_istream_ptr(Source&& source) {
    return std::unique_ptr<std::istream>(new source_istream<Source>(std::move(source)));
}

/**
 * Factory function for creating source istream unique pointers,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return source istream pointer
 */
template <typename Source>
std::unique_ptr<std::istream> make_source_istream_ptr(Source& source) {
    return std::unique_ptr<std::istream>(new source_istream<reference_source<Source>>(make_reference_source(source)));
}

} // namespace
}

#endif	/* STATICLIB_IO_SOURCE_ISTREAM_HPP */

