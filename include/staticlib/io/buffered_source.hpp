/* 
 * File:   buffered_source.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:15 PM
 */

#ifndef STATICLIB_IO_BUFFERED_SOURCE_HPP
#define	STATICLIB_IO_BUFFERED_SOURCE_HPP

#include <ios>
#include <array>
#include <utility>
#include <cstring>

namespace staticlib {
namespace io {

template <typename Source, std::size_t buf_size = 8192, std::streamsize source_eof = -1>
class buffered_source {
    Source src;
    bool exhausted = false;

    std::array<char, buf_size> buffer;
    size_t pos = 0;
    size_t avail = 0;

public:
    buffered_source(Source src) :
    src(std::move(src)) { }

    buffered_source(const buffered_source&) = delete;

    buffered_source& operator=(const buffered_source&) = delete;

    buffered_source(buffered_source&& other) :
    src(std::move(other.src)),
    exhausted(other.exhausted),
    buffer(std::move(other.buffer)),
    pos(other.pos),
    avail(other.avail) { }

    buffered_source& operator=(buffered_source&& other) {
        src = std::move(other.src);
        exhausted = other.exhausted;
        buffer = std::move(other.buffer);
        pos = other.pos;
        avail = other.avail;
        return *this;
    }

    std::streamsize read(char* buf, std::streamsize length) {
        size_t ulen = static_cast<size_t>(length);
        // return from buffer
        if (ulen <= avail) {
            std::memcpy(buf, buffer.data() + pos, ulen);
            pos += ulen;
            avail -= ulen;
            return length;
        }
        // copy all data already available
        size_t uhead = avail;
        if (uhead > 0) {
            std::memcpy(buf, buffer.data() + pos, uhead);
        }
        pos = 0;
        avail = 0;
        std::streamsize head = static_cast<std::streamsize> (uhead);
        // try to guess whether to do direct read, or fill buffer first
        if (ulen > buffer.size()) {
            // read directly into the destination
            size_t result = read_into_buffer(buf, head, ulen - uhead);
            size_t out = result + uhead;
            return out > 0 ? out : std::char_traits<char>::eof();
        }
        // fill buffer
        avail = read_into_buffer(buffer.data(), 0, buffer.size());
        if (avail > 0) {
            // copy tail from buffer
            size_t to_read = std::min(ulen - uhead, avail);
            std::memcpy(buf + head, buffer.data(), to_read);
            pos = to_read;
            avail -= to_read;
            // note that head >= 0, and to_read > 0
            return to_read + head;
        }
        return head == 0 ? std::char_traits<char>::eof() : head;
    }
    
    Source& get_source() {
        return src;
    }
    
    std::array<char, buf_size>& get_buffer() {
        return buffer;
    }
    
private:
    // repeatable source read logic
    size_t read_into_buffer(char* buf, std::streamsize offset, size_t length) {
        if (!exhausted) {
            size_t result = 0;
            while (result < length) {
                auto amt = src.read(buf + offset + result, length - result);
                if (source_eof != amt) {
                    result += static_cast<size_t> (amt);
                } else {
                    exhausted = true;
                    break;
                }
            }
            return result;
        }
        return 0;
    }
};

template <typename Source>
buffered_source<Source> make_buffered_source(Source&& source) {
    return buffered_source<Source>(std::move(source));
}

} // namespace
}

#endif	/* STATICLIB_IO_BUFFERED_SOURCE_HPP */

