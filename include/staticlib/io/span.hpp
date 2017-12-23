/*
 * Copyright 2017, alex at staticlibs.net
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
 * File:   span.hpp
 * Author: alex
 *
 * Created on February 5, 2017, 4:41 PM
 */

#ifndef STATICLIB_SUPPORT_SPAN_HPP
#define STATICLIB_SUPPORT_SPAN_HPP

#include <cstdint>
#include <array>
#include <ios>
#include <string>
#include <vector>

#include "staticlib/config.hpp"
#include "staticlib/support.hpp"

namespace staticlib {
namespace io {

/**
 * Span-specific exception
 */
class bad_span_access_exception : public sl::support::exception {
public:

    /**
     * Default constructor
     */
    bad_span_access_exception() = default;
    
    /**
     * Constructor with message
     * 
     * @param msg error message
     */
    bad_span_access_exception(const std::string& msg) :
    sl::support::exception(msg) { }

};


/**
 * Non-owning range of elements in of contiguous memory.
 */
template<typename T>
class span {
    /**
     * Start pointer
     */
    T* first_ptr = nullptr;
    /**
     * End pointer
     */
    T* last_ptr = nullptr;

public:

    /**
     * Constructor
     * 
     * @param buffer span start pointer
     * @param length number of elements
     * @throws bad_span_access_exception on null pointer or on invalid index
     */
    template<typename IntType>
    span(T* buffer, IntType length) :
    first_ptr(buffer),
    last_ptr(buffer) {
        if (nullptr != buffer) {
            if (sl::support::is_sizet(length) && sl::support::is_streamsize(length)) {
                last_ptr += static_cast<size_t> (length);
            } else {
                throw bad_span_access_exception(std::string() + "Invalid 'length' span parameter specified," +
                        " length: [" + sl::support::to_string(length) + "]");
            }
        } else {
            throw bad_span_access_exception(std::string() + "Invalid 'null' data pointer specified to span," +
                    " length: [" + sl::support::to_string(length) + "]");
        }
    }

    /**
     * Constructor from array
     * 
     * @param buffer array of type convertible into T
     */
    template<typename Convertible, size_t Length>
    span(std::array<Convertible, Length>& buffer) :
    span(buffer.data(), buffer.size()) { }

    /**
     * Constructor from vector
     * 
     * @param buffer vector of type T
     */
    span(std::vector<T>& buffer) :
    span(buffer.data(), buffer.size()) { }

    /**
     * Constructor from vector
     * 
     * @param buffer array of type convertible into T
     */
    template<typename Convertible>
    span(std::vector<Convertible>& buffer) :
    span(buffer.data(), buffer.size()) { }

    /**
     * Constructor from string, for `char` and `const char`spans only
     * 
     * @param buffer string
     */
    span(std::string& buffer) :
    span(std::addressof(buffer.front()), buffer.length()) { }

    /**
     * Constructor from string, for `const char`spans only
     * 
     * @param buffer string
     */
    span(const std::string& buffer) :
    span(buffer.data(), buffer.length()) { }

    /**
     * Copy constructor
     * 
     * @param other other span
     */
    span(const span<T>& other) :
    first_ptr(other.first_ptr),
    last_ptr(other.last_ptr) { }
    
    /**
     * Copy assignment operator
     * 
     * @param other other span
     */
    span<T>& operator=(const span<T>& other) {
        first_ptr = other.first_ptr;
        last_ptr = other.last_ptr;
    }

    /**
     * Element access checked operation
     * 
     * @param index index to access element at
     * @return reference to element
     * @throws bad_span_access_exception on invalid index
     */
    T& operator[](size_t index) const {
        if (index < size()) {
            return *(first_ptr + index);
        } else {
            throw bad_span_access_exception(std::string() + "Invalid index access attempt," +
                    " span size: [" + sl::support::to_string(size()) + "],"
                    " index: [" + sl::support::to_string(index) + "]");
        }
    }

    /**
     * Data accessor
     * 
     * @return pointer to span start
     */
    T* data() const STATICLIB_NOEXCEPT {
        return first_ptr;
    }

    /**
     * Emptiness check
     * 
     * @return true, if this span empty, false otherwise
     */
    bool empty() const STATICLIB_NOEXCEPT {
        return 0 == size();
    }

    /**
     * Size accessor
     * 
     * @return span size
     */
    size_t size() const STATICLIB_NOEXCEPT {
        return last_ptr - first_ptr;
    }

    /**
     * Size accessor
     * 
     * @return span size as signed
     */
    std::streamsize size_signed() const STATICLIB_NOEXCEPT {
        return static_cast<std::streamsize>(last_ptr - first_ptr);
    }

    /**
     * Size in bytes accessor
     * 
     * @return size in bytes
     */
    size_t size_bytes() const STATICLIB_NOEXCEPT {
        return (last_ptr - first_ptr) * sizeof (T);
    }

    /**
     * Begin iterator
     * 
     * @return begin iterator
     */
    T* begin() const STATICLIB_NOEXCEPT {
        return first_ptr;
    }

    /**
     * End iterator
     * 
     * @return end iterator
     */
    T* end() const STATICLIB_NOEXCEPT {
        return last_ptr;
    }

};

/**
 * Helper function, creates span instance
 * 
 * @param buffer span start pointer
 * @param length number of elements
 * @return span instance
 */
template<typename T, typename IntType>
span<T> make_span(T* buffer, IntType length) {
    return span<T>(buffer, length);
}

/**
 * Helper function, creates span instance from array
 * 
 * @param buffer array
 * @return span instance
 */
template<typename T, size_t Length>
span<T> make_span(std::array<T, Length>& buffer) {
    return span<T>(buffer.data(), buffer.size());
}

/**
 * Helper function, creates span instance from vector
 * 
 * @param buffer vector
 * @return span instance
 */
template<typename T>
span<T> make_span(std::vector<T>& buffer) {
    return span<T>(buffer.data(), buffer.size());
}

} // namespace
}

#endif /* STATICLIB_SUPPORT_SPAN_HPP */

