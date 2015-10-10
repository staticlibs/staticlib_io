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

#include "staticlib/io/IoException.hpp"

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
template <typename Source, std::streamsize source_eof = -1 >
class unbuffered_istreambuf : public detail::unbuffered_streambuf_base {
    Source source;
    bool exhausted = false;

public:
    /**
     * Constructor
     * 
     * @param source Source instance (in terms of Boost.Iostreams)
     */
    unbuffered_istreambuf(Source source) : 
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
    exhausted(other.exhausted) { }

    /**
     * Move assignment operator
     */
    unbuffered_istreambuf& operator=(unbuffered_istreambuf&& other) {
        source = std::move(other.source);
        exhausted(other.exhausted);
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
            throw IoException("'uflow' operation is not supported by unbuffered streambuf");
        }
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
            auto res = source.read(s, count);
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
 * Unbuffered implementation of output streambuf, wraps Sink reference (in terms of Boost.Iostreams).
 * Should be used directly, must NOT be used wrapped into std::ostream.
 */
template <typename Sink>
class unbuffered_ostreambuf : public detail::unbuffered_streambuf_base {
    Sink sink;

public:
    /**
     * Constructor
     * 
     * @param sink Sink reference (in terms of Boost.Iostreams)
     */
    unbuffered_ostreambuf(Sink sink) : 
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

protected:

    /**
     * Writes count characters to the output sequence from the character array whose first element is pointed to by s. 
     * 
     * @param s source buffer 
     * @param count number of characters to write
     */
    virtual std::streamsize xsputn(const char* s, std::streamsize count) override {
        return sink.write(s, count);
    }
};
    
} // namespace
}

#endif	/* STATICLIB_IO_UNBUFFERED_STREAMBUF_HPP */

