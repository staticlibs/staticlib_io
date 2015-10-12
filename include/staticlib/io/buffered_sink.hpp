/* 
 * File:   buffered_sink.hpp
 * Author: alex
 *
 * Created on October 9, 2015, 12:15 PM
 */

#ifndef STATICLIB_IO_BUFFERED_SINK_HPP
#define	STATICLIB_IO_BUFFERED_SINK_HPP

#include <ios>
#include <array>
#include <utility>
#include <cstring>

namespace staticlib {
namespace io {

template <typename Sink, std::size_t buf_size = 8192>
class buffered_sink {
    Sink sink;

    std::array<char, buf_size> buffer;
    size_t pos = 0;
    size_t avail = 0;

public:
    buffered_sink(Sink sink) :
    sink(std::move(sink)) { }

    ~buffered_sink() {
        flush();
    }
    
    buffered_sink(const buffered_sink&) = delete;

    buffered_sink& operator=(const buffered_sink&) = delete;

    buffered_sink(buffered_sink&& other) :
    sink(std::move(other.sink)),
    buffer(std::move(other.buffer)),
    pos(other.pos),
    avail(other.avail) { }

    buffered_sink& operator=(buffered_sink&& other) {
        sink = std::move(other.sink);
        buffer = std::move(other.buffer);
        pos = other.pos;
        avail = other.avail;
        return *this;
    }

    std::streamsize write(const char* b, std::streamsize length) {
        if (length >= static_cast<std::streamsize>(buffer.size())) {
            write_to_sink(buffer.data(), 0, pos);
            pos = 0;
            avail = buffer.size();
            write_to_sink(b, 0, length);
        } else if (length <= static_cast<std::streamsize>(avail)) {
            std::memcpy(buffer.data() + pos, b, length);
            pos += length;
            if (static_cast<std::streamsize>(avail) > length) {
                avail -= length;
                return length;
            } else {
                write_to_sink(buffer.data(), 0, buffer.size());
                pos = 0;
                avail = buffer.size();
            }
        } else {
            write_to_sink(buffer.data(), 0, pos);
            std::memcpy(buffer.data(), b, length);
            pos = length;
            avail = buffer.size() - length;
        }
        return length;
    }

    std::streamsize flush() {
        std::streamsize flushed = 0;
        if (pos > 0) {
            write_to_sink(buffer.data(), 0, pos);
            flushed += pos;
            pos = 0;
            avail = buffer.size();
        }
        flushed += sink.flush();
        return flushed;
    }
    
    Sink& get_sink() {
        return sink;
    }

    std::array<char, buf_size>& get_buffer() {
        return buffer;
    }

private:
    void write_to_sink(const char* buf, std::streamsize offset, size_t length) {
        size_t result = 0;
        while (result < length) {
            auto amt = sink.write(buf + offset + result, length - result);
            result += static_cast<size_t> (amt);
        }
    }
    
};

template <typename Sink>
buffered_sink<Sink> make_buffered_sink(Sink&& sink) {
    return buffered_sink<Sink>(std::move(sink));
}

} // namespace
}

#endif	/* STATICLIB_IO_BUFFERED_SINK_HPP */

