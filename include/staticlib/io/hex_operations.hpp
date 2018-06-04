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
 * File:   hex_operations.hpp
 * Author: alex
 *
 * Created on March 20, 2018, 5:15 PM
 */

#ifndef STATICLIB_IO_HEX_HPP
#define STATICLIB_IO_HEX_HPP

#include <cctype>
#include <cstdint>
#include <array>
#include <string>

#include "staticlib/io/array_source.hpp"
#include "staticlib/io/hex_sink.hpp"
#include "staticlib/io/hex_source.hpp"
#include "staticlib/io/operations.hpp"
#include "staticlib/io/string_sink.hpp"

namespace staticlib {
namespace io {

/**
 * Encodes the specified string as a hexadecimal string
 * 
 * @param plain plain string
 * @return hex string
 */
inline std::string string_to_hex(const std::string& plain) {
    if (plain.empty()) return std::string();
    auto src = array_source(plain.c_str(), plain.length());
    auto sink = string_sink();
    {
        auto hsink = make_hex_sink(sink);
        copy_all(src, hsink);
    }
    return std::move(sink.get_string());
}

/**
 * Decodes the specified hexadecimal string
 * 
 * @param hexstr hex string
 * @return plain string
 */
inline std::string string_from_hex(const std::string& hexstr) {
    if (hexstr.empty()) return std::string();
    auto src = make_hex_source(array_source(hexstr.c_str(), hexstr.length()));
    auto sink = string_sink();
    copy_all(src, sink);
    return std::move(sink.get_string());
}

/**
 * Format hex and plain strings as a hex+plain string.
 * 
 * Formats specified hex-string as a `hex [plain]` string.
 * Non-printable characters in plain string are replaced with spaces.
 * 
 * Intended to be used for logging.
 * 
 * @param hexstr string in hexadecimal encoding
 * @param plain plain string
 * @return formatted string
 */
inline std::string format_hex_and_plain(const std::string& hexstr, const std::string& plain) {
    if (hexstr.empty() || plain.empty()) return std::string();
    
    // prettify hex
    auto pretty = std::string();
    for (size_t i = 0; i < hexstr.length() - 1; i += 2 ) {
        if (pretty.length() > 0) {
            pretty.push_back(' ');
        }
        pretty.push_back(hexstr[i]);
        pretty.push_back(hexstr[i + 1]);
    }

    // cleanup plain
    auto clean = std::string();
    for (size_t i = 0; i < plain.length(); i++) {
        char ch = plain[i];
        if (ch < 32 || std::isspace(ch)) {
            clean.push_back(' ');
        } else {
           clean.push_back(ch);
        }
    }

    // return formatted
    return pretty + " [" + clean + "]";
}

/**
 * Format hex-string as a hex+plain string.
 * 
 * Formats specified hex-string as a `hex [plain]` string.
 * Non-printable characters in plain string are replaced with spaces.
 * 
 * Intended to be used for logging.
 * 
 * @param hexstr string in hexadecimal encoding
 * @return formatted string
 */
inline std::string format_hex(const std::string& hexstr) {
    if (hexstr.empty()) return std::string();
    auto plain = string_from_hex(hexstr);
    return format_hex_and_plain(hexstr, plain);
}

/**
 * Format plain string as a hex+plain string.
 * 
 * Formats specified hex-string as a `hex [plain]` string.
 * Non-printable characters in plain string are replaced with spaces.
 * 
 * Intended to be used for logging.
 * 
 * @param plain plain string
 * @return formatted string
 */
inline std::string format_plain_as_hex(const std::string& plain) {
    if (plain.empty()) return std::string();
    auto hexstr = string_to_hex(plain);
    return format_hex_and_plain(hexstr, plain);
}

} // namespace
}

#endif /* STATICLIB_IO_HEX_HPP */

