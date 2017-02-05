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
 * File:   unbuffered_streambuf.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:10 PM
 */

#ifndef STATICLIB_IO_UNBUFFERED_STREAMBUF_HPP
#define	STATICLIB_IO_UNBUFFERED_STREAMBUF_HPP

#include <ios>
#include <streambuf>
#include <utility>

#include "staticlib/config/span.hpp"

#include "staticlib/io/reference_sink.hpp"
#include "staticlib/io/reference_source.hpp"

namespace staticlib {
namespace io {

namespace detail {

/**
 * Base class for unbuffered streambuf implementations
 * 
 */
class unbuffered_streambuf_base : public std::streambuf {
public:   
    /**
     * Unsupported method, throws 'UtilsException'
     */
    virtual int underflow() override {
        throw std::ios_base::failure("'underflow' operation is not supported by unbuffered streambuf");
    }

    /**
     * Unsupported method, throws 'UtilsException'
     */
    virtual int uflow() override {
        throw std::ios_base::failure("'uflow' operation is not supported by unbuffered streambuf");
    }

    /**
     * Unsupported method, throws 'UtilsException'
     */
    virtual int overflow(int) override {
        throw std::ios_base::failure("'overflow' operation is not supported by unbuffered streambuf");
    }

protected:

    /**
     * Reads count characters from the input sequence and stores them into a character array pointed to by s
     * Should be overriden by Source streambuf inheritors, default implementation throws 'UtilsException'
     * 
     * @param s destination buffer 
     * @param count number of characters to read
     */
    virtual std::streamsize xsgetn(char*, std::streamsize) override {
        throw std::ios_base::failure("'xsgetn' operation is not supported by unbuffered streambuf");
    }

    /**
     * Writes count characters to the output sequence from the character array whose first element is pointed to by s. 
     * Should be overriden by Sink streambuf inheritors, default implementation throws 'UtilsException'
     * 
     * @param s source buffer 
     * @param count number of characters to erite
     */
    virtual std::streamsize xsputn(const char*, std::streamsize) override {
        throw std::ios_base::failure("'xsputn' operation is not supported by unbuffered streambuf");
    }

    /**
     * Unsupported method, throws 'UtilsException'
     */
    virtual unbuffered_streambuf_base* setbuf(char*, std::streamsize) override {
        throw std::ios_base::failure("'setbuf' operation is not supported by unbuffered streambuf");
    }

    /**
     * Unsupported method, throws 'UtilsException'
     */
    virtual std::streampos seekoff(std::streamoff, std::ios_base::seekdir, std::ios_base::openmode) override {
        throw std::ios_base::failure("'seekoff' operation is not supported by unbuffered streambuf");
    }

    /**
     * Unsupported method, throws 'UtilsException'
     */
    virtual std::streampos seekpos(std::streampos, std::ios_base::openmode) override {
        throw std::ios_base::failure("'seekpos' operation is not supported by unbuffered streambuf");
    }

};

} // namespace

/**
 * Unbuffered implementation of input streambuf, wraps Source reference (in terms of Boost.Iostreams).
 * Should be used directly, must NOT be used wrapped into std::istream.
 */
// use -1 as std::char_traits<char>::eof() won't work in msvc
template <typename Source, std::streamsize source_eof = -1>
class unbuffered_istreambuf : public detail::unbuffered_streambuf_base {
    Source source;
    bool exhausted = false;

public:
    /**
     * Constructor,
     * created source wrapper will own specified source
     * 
     * @param source Source instance (in terms of Boost.Iostreams)
     */
    unbuffered_istreambuf(Source&& source) : 
    source(std::move(source)) { }

    /**
     * Deleted copy constructor
     */
    unbuffered_istreambuf(const unbuffered_istreambuf&) = delete;

    /**
     * Deleted copy assignment operator
     */
    unbuffered_istreambuf& operator=(const unbuffered_istreambuf&) = delete;

    /**
     * Move constructor
     */
    unbuffered_istreambuf(unbuffered_istreambuf&& other) :
    source(std::move(other.source)),
    exhausted(other.exhausted) { 
        other.exhausted = true;
    }

    /**
     * Move assignment operator
     */
    unbuffered_istreambuf& operator=(unbuffered_istreambuf&& other) {
        source = std::move(other.source);
        exhausted = other.exhausted;
        other.exhausted = true;
        return *this;
    }

    /**
     * Compatibility method to mimic expected EOF reporting of this streambuf.
     * Return 'std::char_traits<char>::eof()' if the Source was exhausted,
     * throws 'UtilsException' otherwise.
     */
    virtual int uflow() override {
        if (exhausted) {
            return std::char_traits<char>::eof();
        } else {
            throw std::ios_base::failure("'uflow' operation is not supported by unbuffered streambuf");
        }
    }
    
    /**
     * Underlying source accessor
     * 
     * @return underlying source reference
     */
    Source& get_source() {
        return source;
    }

protected:
    /**
     * Reads count characters from the input sequence and stores them into a character array pointed to by s
     * On Source EOF returns 0 and sets this streambuf state to 'exhausted'
     * 
     * @param s destination buffer 
     * @param count number of characters to read
     */
    virtual std::streamsize xsgetn(char* s, std::streamsize count) override {
        if (!exhausted) {
            auto res = source.read({s, count});
            if (source_eof != res) {
                return res;
            } else {
                exhausted = true;
                return 0;
            }
        } else {
            return 0;
        }
    }
};

/**
 * Factory function for creating unbuffered_istreambuf sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return unbuffered_istreambuf source
 */
template <typename Source,
        class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
unbuffered_istreambuf<Source> make_unbuffered_istreambuf(Source&& source) {
    return unbuffered_istreambuf<Source>(std::move(source));
}

/**
 * Factory function for creating unbuffered_istreambuf sources,
 * created source wrapper will own specified source
 * 
 * @param source input source
 * @return unbuffered_istreambuf source
 */
template <typename Source,
class = typename std::enable_if<!std::is_lvalue_reference<Source>::value>::type>
unbuffered_istreambuf<Source>* make_unbuffered_istreambuf_ptr(Source&& source) {
    return new unbuffered_istreambuf<Source>(std::move(source));
}

/**
 * Factory function for creating unbuffered_istreambuf sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return unbuffered_istreambuf source
 */
template <typename Source>
unbuffered_istreambuf<reference_source<Source>> make_unbuffered_istreambuf(Source& source) {
    return unbuffered_istreambuf<reference_source<Source>>(make_reference_source(source));
}

/**
 * Factory function for creating unbuffered_istreambuf sources,
 * created source wrapper will NOT own specified source
 * 
 * @param source input source
 * @return unbuffered_istreambuf source
 */
template <typename Source>
unbuffered_istreambuf<reference_source<Source>>* make_unbuffered_istreambuf_ptr(Source& source) {
    return new unbuffered_istreambuf<reference_source<Source>>(make_reference_source(source));
}

/**
 * Unbuffered implementation of output streambuf, wraps Sink reference (in terms of Boost.Iostreams).
 * Should be used directly, must NOT be used wrapped into std::ostream.
 */
template <typename Sink>
class unbuffered_ostreambuf : public detail::unbuffered_streambuf_base {
    Sink sink;

public:
    /**
     * Constructor,
     * created sink wrapper will own specified sink
     * 
     * @param sink Sink reference (in terms of Boost.Iostreams)
     */
    unbuffered_ostreambuf(Sink&& sink) : 
    sink(std::move(sink)) { }
    
    /**
     * Deleted copy constructor
     */
    unbuffered_ostreambuf(const unbuffered_ostreambuf&) = delete;

    /**
     * Deleted copy assignment operator
     */
    unbuffered_ostreambuf& operator=(const unbuffered_ostreambuf&) = delete;

    /**
     * Move constructor
     */
    unbuffered_ostreambuf(unbuffered_ostreambuf&& other) :
    sink(std::move(other.sink)) { }

    /**
     * Move assignment operator
     */
    unbuffered_ostreambuf& operator=(unbuffered_ostreambuf&& other) {
        sink = std::move(other.sink);
        return *this;
    }

    /**
     * Underlying sink accessor
     * 
     * @return underlying sink reference
     */
    Sink& get_sink() {
        return sink;
    }

protected:

    /**
     * Writes count characters to the output sequence from the character array whose first element is pointed to by s. 
     * 
     * @param s source buffer 
     * @param count number of characters to write
     */
    virtual std::streamsize xsputn(const char* s, std::streamsize count) override {
        return sink.write({s, count});
    }
};

/**
 * Factory function for creating unbuffered_ostreambuf sinks,
 * created sink wrapper will own specified sink
 * 
 * @param sink destination sink
 * @return unbuffered_ostreambuf sink
 */
template <typename Sink,
        class = typename std::enable_if<!std::is_lvalue_reference<Sink>::value>::type>
unbuffered_ostreambuf<Sink> make_unbuffered_ostreambuf(Sink&& sink) {
    return unbuffered_ostreambuf<Sink>(std::move(sink));
}

/**
 * Factory function for creating unbuffered_ostreambuf sinks,
 * created sink wrapper will own specified sink
 * 
 * @param sink destination sink
 * @return unbuffered_ostreambuf sink
 */
template <typename Sink,
class = typename std::enable_if<!std::is_lvalue_reference<Sink>::value>::type>
unbuffered_ostreambuf<Sink> make_unbuffered_ostreambuf_ptr(Sink&& sink) {
    return new unbuffered_ostreambuf<Sink>(std::move(sink));
}

/**
 * Factory function for creating unbuffered_ostreambuf sinks,
 * created sink wrapper will NOT own specified sink
 * 
 * @param sink destination sink
 * @return unbuffered_ostreambuf sink
 */
template <typename Sink>
unbuffered_ostreambuf<reference_sink<Sink>> make_unbuffered_ostreambuf(Sink& sink) {
    return unbuffered_ostreambuf<reference_sink<Sink>>(make_reference_sink(sink));
}

/**
 * Factory function for creating unbuffered_ostreambuf sinks,
 * created sink wrapper will NOT own specified sink
 * 
 * @param sink destination sink
 * @return unbuffered_ostreambuf sink
 */
template <typename Sink>
unbuffered_ostreambuf<reference_sink<Sink>> make_unbuffered_ostreambuf_ptr(Sink& sink) {
    return new unbuffered_ostreambuf<reference_sink<Sink>>(make_reference_sink(sink));
}
    
} // namespace
}

#endif	/* STATICLIB_IO_UNBUFFERED_STREAMBUF_HPP */

