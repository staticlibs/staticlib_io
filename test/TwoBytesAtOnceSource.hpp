/* 
 * File:   TwoBytesAtOnceSource.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:44 AM
 */

#ifndef STATICLIB_IO_TWOBYTESATONCESOURCE_HPP
#define	STATICLIB_IO_TWOBYTESATONCESOURCE_HPP

class TwoBytesAtOnceSource {
    std::string data;
    size_t ind = 0;

public:
    TwoBytesAtOnceSource(std::string data) : 
    data(std::move(data)) { }
    
    TwoBytesAtOnceSource(const TwoBytesAtOnceSource&) = delete;

    TwoBytesAtOnceSource& operator=(const TwoBytesAtOnceSource&) = delete;

    TwoBytesAtOnceSource(TwoBytesAtOnceSource&& other) :
    data(std::move(other.data)) { }

    TwoBytesAtOnceSource& operator=(TwoBytesAtOnceSource&& other) {
        data = std::move(other.data);
        return *this;
    }

    std::streamsize read(char* s, std::streamsize n) {
        std::streamsize len = n >= 2 ? 2 : n;
        for (std::streamsize i = 0; i < len; i++) {
            s[i] = data[ind++];
        }
        return len;
    }
    
};

#endif	/* STATICLIB_IO_TWOBYTESATONCESOURCE_HPP */

