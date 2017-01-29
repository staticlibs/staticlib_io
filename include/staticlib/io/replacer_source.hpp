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
 * File:   replacer_source.hpp
 * Author: alex
 *
 * Created on November 5, 2016, 8:10 PM
 */

#ifndef STATICLIB_IO_REPLACER_SOURCE_HPP
#define	STATICLIB_IO_REPLACER_SOURCE_HPP

#include <cstring>
#include <functional>
#include <ios>
#include <map>
#include <string>
#include <vector>

#include "staticlib/config.hpp"

#include "staticlib/io/IOException.hpp"
#include "staticlib/io/buffered_source.hpp"
#include "staticlib/io/reference_source.hpp"

namespace staticlib {
namespace io {

/**
 * Source wrapper that replaces specified substrings in the input
 */
template <typename Source>
class replacer_source {
    // Replace state class
    enum class State {
        PREFIX, PLACEHOLDER
    };
    
    /**
     * Input source
     */
    buffered_source<Source> src;
    /**
     * Whether input source was exhausted
     */
    bool exhausted = false;
    /**
     * Values mapping for replacement
     */
    std::map<std::string, std::string> values;
    /**
     * Function that will be called on error condition
     */
    std::function<void(const std::string&) > on_error;    
    /**
     * Placeholder prefix
     */
    std::string prefix;
    /**
     * Placeholder postfix
     */
    std::string postfix;
    /**
     * Max allowed length of a placeholder
     */
    size_t max_placeholder_len;

    /**
     * Current state
     */
    std::vector<char> buffer;
    size_t buf_len_limit = 4096;
    size_t pos = 0;
    std::string placeholder;
    size_t ind_prefix = 0;
    size_t ind_postfix = 0;
    State state = State::PREFIX;
    
public:

    /**
     * Constructor,
     * created source wrapper will own specified source
     * 
     * @param src input source
     */
    replacer_source(Source&& src, std::map<std::string, std::string> values,
            std::function<void(const std::string&)> on_error,
            std::string prefix = "{{", std::string postfix = "}}", size_t max_placeholder_len = 255) :
    src(make_buffered_source(std::move(src))),
    values(std::move(values)),
    on_error(on_error),
    prefix(std::move(prefix)),
    postfix(std::move(postfix)),
    max_placeholder_len(max_placeholder_len) { }

    /**
     * Deleted copy constructor
     * 
     * @param other instance
     */
    replacer_source(const replacer_source&) = delete;

    /**
     * Deleted copy assignment operator
     * 
     * @param other instance
     * @return this instance 
     */
    replacer_source& operator=(const replacer_source&) = delete;

    /**
     * Move constructor
     * 
     * @param other other instance
     */
    replacer_source(replacer_source&& other) :
    src(std::move(other.src)),
    exhausted(other.exhausted),
    values(std::move(other.values)),
    on_error(other.on_error),
    prefix(std::move(other.prefix)),
    postfix(std::move(other.postfix)),
    max_placeholder_len(other.max_placeholder_len),
    buffer(std::move(other.buffer)),
    buf_len_limit(other.buf_len_limit),
    pos(other.pos),
    placeholder(std::move(other.placeholder)),
    ind_prefix(other.ind_prefix),
    ind_postfix(other.ind_postfix),
    state(other.state) { }

    /**
     * Move assignment operator
     * 
     * @param other other instance
     * @return this instance
     */
    replacer_source& operator=(replacer_source&& other) {
        src = std::move(other.src);
        exhausted = other.exhausted;
        values = std::move(other.values);
        on_error = other.on_error;        
        prefix = std::move(other.prefix);
        postfix = std::move(other.postfix);
        max_placeholder_len = other.max_placeholder_len;
        buffer = std::move(other.buffer);
        buf_len_limit = other.buf_len_limit;
        pos = other.pos;
        placeholder = std::move(other.placeholder);
        ind_prefix = other.ind_prefix;
        ind_postfix = other.ind_postfix;
        state = other.state;
        return *this;
    }

    /**
     * Replacing read implementation
     * 
     * @param buf output buffer
     * @param length number of bytes to process
     * @return number of bytes processed
     */
    std::streamsize read(char* buf, std::streamsize length) {
        namespace sc = staticlib::config;
        if (!sc::is_sizet(length)) throw IOException(TRACEMSG(
                "Invalid 'read' parameter specified, length: [" + sc::to_string(length) + "]"));
        size_t ulen = static_cast<size_t> (length);
        // return from buffer
        std::streamsize read_buffered = copy_buffered(buf, ulen);
        if (read_buffered > 0) {
            return read_buffered;
        }
        // fill buffer
        if (!exhausted) {
            buffer.resize(0);
            pos = 0;
            size_t limitlen = ulen <= buf_len_limit ? ulen : buf_len_limit;
            for(;;) {
                char cur;
                std::streamsize read = 0;
                while (0 == (read = src.read(std::addressof(cur), 1)));
                exhausted = std::char_traits<char>::eof() == read;
                if (exhausted) {
                    break;
                }
                switch (state) {
                case State::PREFIX:
                    do_prefix(cur);
                    break;
                case State::PLACEHOLDER:
                    do_placeholder(cur);
                    break;
                }
                if (buffer.size() >= limitlen) {
                    break;
                }
            }
        }
        // return if any
        std::streamsize read_prepared = copy_buffered(buf, ulen);
        if (read_prepared > 0) {
            return read_prepared;
        } 
        // return eof
        if (placeholder.length() > 0) {
            std::string msg = "Invalid unclosed placeholder: [" + placeholder + "]";
            on_error(msg);
        }
        return std::char_traits<char>::eof();
    }

    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return src.get_source();
    }

    /**
     * Values mapping accessor
     * 
     * @return values mapping
     */
    std::map<std::string, std::string>& get_values() {
        return values;
    }
    
private:

    void do_prefix(char cur) {
        if (cur != prefix[ind_prefix]) {
            if (ind_prefix > 0) {
                size_t wp = buffer.size();
                buffer.resize(wp + ind_prefix);
                std::memcpy(buffer.data() + wp, prefix.c_str(), ind_prefix);
                ind_prefix = 0;
            }
            buffer.push_back(cur);
        } else {
            ind_prefix += 1;
            if (prefix.length() == ind_prefix) {
                ind_prefix = 0;
                state = State::PLACEHOLDER;
            }
        }
    }

    void do_placeholder(char cur) {
        placeholder.push_back(cur);
        if (placeholder.size() == max_placeholder_len) {
            std::string msg = "Parameter name: [" + placeholder + "] is too long";
            on_error(msg);
        }
        if (cur == postfix[ind_postfix]) {
            ind_postfix += 1;
            if (postfix.length() == ind_postfix) {
                ind_postfix = 0;
                placeholder.resize(placeholder.length() - postfix.length());
                auto par = values.find(placeholder);
                if (values.end() != par) {
                    size_t wp = buffer.size();
                    buffer.resize(wp + par->second.length());
                    std::memcpy(buffer.data() + wp, par->second.c_str(), par->second.length());
                    placeholder.clear();
                    state = State::PREFIX;
                } else {
                    std::string msg = "Parameter: [" + placeholder + "] not found";
                    on_error(msg);
                }
            }
        } else {
            ind_postfix = 0;
        }
    } 
    
    std::streamsize copy_buffered(char* buf, size_t ulen) {
        size_t avail = buffer.size() - pos;
        if (avail > 0) {
            size_t ucplen = avail <= ulen ? avail : ulen;
            std::memcpy(buf, buffer.data() + pos, ucplen);
            pos += ucplen;
            return static_cast<std::streamsize>(ucplen);
        }
        return 0;
    }
    
};

/**
 * Factory function for creating replacer sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return replacer source
 */
template <typename Source,
class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
replacer_source<Source> make_replacer_source(Source&& source, std::map<std::string, std::string> values,
        std::function<void(const std::string&)> on_error) {
    return replacer_source<Source>(std::move(source), std::move(values), on_error);
}

/**
 * Factory function for creating replacer sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return replacer source
 */
template <typename Source>
replacer_source<reference_source<Source>> make_replacer_source(Source& source, std::map<std::string, std::string> values,
        std::function<void(const std::string&)> on_error) {
    return replacer_source<reference_source<Source>>(make_reference_source(source), std::move(values), on_error);
}

} // namespace
}

#endif	/* STATICLIB_IO_REPLACER_SOURCE_HPP */

