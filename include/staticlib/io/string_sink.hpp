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
 * File:   string_sink.hpp
 * Author: alex
 *
 * Created on November 6, 2015, 3:59 PM
 */

#ifndef STATICLIB_IO_STRING_SINK_HPP
#define	STATICLIB_IO_STRING_SINK_HPP

#include <ios>
#include <string>
#include <cstdint>
#include <cstring>

namespace staticlib {
namespace io {

/**
 * Sink implementation that writes data to the underlying "std::string"
 */
class string_sink {
    /**
     * Destination string
     */
    std::string str;
    
public:
    /**
     * Constructor
     */
    string_sink() { }

    /**
     * Constructor
     * 
     * @param str string to write to
     */
    string_sink(std::string&& str) :
    str(std::move(str)) { }
    
    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    string_sink(const string_sink&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    string_sink& operator=(const string_sink&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    string_sink(string_sink&& other) :
    str(std::move(other.str)) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    string_sink& operator=(string_sink&& other) {
        str = std::move(other.str);
        return *this;
    }

    /**
     * Buffered write implementation
     * 
     * @param b source buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize write(const char* b, std::streamsize length) {
        size_t ulen = static_cast<size_t> (length);
        size_t size = str.size();
        str.resize(str.size() + ulen);
        std::memcpy(std::addressof(str.front()) + size, b, ulen);
        return static_cast<std::streamsize> (ulen);
    }
    
    /**
     * Underlying string accessor
     * 
     * @return underlying string
     */
    std::string& get_string() {
        return str;
    }
    
    /**
     * No-op flush implementation
     * 
     * @return 0
     */
    std::streamsize flush() {
        // no-op
        return 0;
    }
    
};

} // namespace
}


#endif	/* STATICLIB_IO_STRING_SINK_HPP */
