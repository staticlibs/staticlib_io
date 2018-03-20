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
 * File:   hex.hpp
 * Author: alex
 *
 * Created on March 20, 2018, 5:15 PM
 */

#ifndef STATICLIB_IO_HEX_HPP
#define STATICLIB_IO_HEX_HPP

#include <cctype>
#include <cstdint>
#include <array>

#include "staticlib/io/array_source.hpp"
#include "staticlib/io/operations.hpp"
#include "staticlib/io/string_sink.hpp"

namespace staticlib {
namespace io {

/**
 * Reads bytes from specified source one by one,
 * converts them to HEX and writes to specified sink.
 * 
 * @param src source with plain data
 * @param sink sink for HEX data
 * @return number of bytes converted to HEX
 */
template<typename Source, typename Sink>
size_t copy_to_hex(Source& src, Sink& sink) {
    char ch = '\0';
    auto hbuf = std::array<char, 2>();
    hbuf[0] = '\0';
    hbuf[1] = '\0';
    size_t count = 0;
    while(1 == read_all(src, {std::addressof(ch), 1})) {
        // http://stackoverflow.com/a/18025541/314015
        unsigned char uch = static_cast<unsigned char>(ch);
        hbuf[0] = operations_detail::symbols[static_cast<size_t>(uch >> 4)];
        hbuf[1] = operations_detail::symbols[static_cast<size_t>(uch & 0x0f)];
        sl::io::write_all(sink, {hbuf.data(), hbuf.size()});
        count += 1;
    }
    return count;
}

/**
 * Reads HEX pairs from specified source,
 * parses them into byte representation and
 * writes to specified sink.
 * 
 * @param src source with HEX data
 * @param sink sink for plain data
 * @return number of bytes converted from HEX
 */
template<typename Source, typename Sink>
size_t copy_from_hex(Source& src, Sink& sink) {
    std::array<char, 3> hbuf;
    hbuf[0] = '\0';
    hbuf[1] = '\0';
    hbuf[2] = '\0';
    size_t count = 0;
    size_t read = 0;
    while (2 == (read = read_all(src, {hbuf.data(), 2}))) {
        char* end = nullptr;
        errno = 0;
        char byte = static_cast<char> (strtol(hbuf.data(), std::addressof(end), 16));
        if (errno == ERANGE || end != hbuf.data() + 2) {
            throw io_exception(TRACEMSG("Error parsing byte from HEX-pair: [" + std::string(hbuf.data(), 2) + "]"));
        }
        sink.write({std::addressof(byte), 1});
        count += 1;
    }
    if (0 != read) {
        throw io_exception(TRACEMSG("Invalid non-even number of bytes available in HEX source," +
                " tail size: [" + sl::support::to_string(read) +"]"));
    }
    return count;
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
std::string format_hex_and_plain(const std::string& hexstr) {
    // decode hex
    auto src = array_source(hexstr.c_str(), hexstr.length());
    auto sink = string_sink();
    copy_from_hex(src, sink);
    auto& st = sink.get_string();

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
    auto plain = std::string();
    for (size_t i = 0; i < st.length(); i++) {
        char ch = st[i];
        if (ch < 32 || std::isspace(ch)) {
            plain.push_back(' ');
        } else {
            plain.push_back(ch);
        }
    }

    return pretty + " [" + plain + "]";
}

} // namespace
}

#endif /* STATICLIB_IO_HEX_HPP */

