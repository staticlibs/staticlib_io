/* 
 * File:   TwoBytesAtOnceSink.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:43 AM
 */

#ifndef STATICLIB_IO_TWOBYTESATONCESINK_HPP
#define	STATICLIB_IO_TWOBYTESATONCESINK_HPP

class TwoBytesAtOnceSink {
    std::string data;

public:

    TwoBytesAtOnceSink() { }
    
    TwoBytesAtOnceSink(const TwoBytesAtOnceSink&) = delete;
    
    TwoBytesAtOnceSink& operator=(const TwoBytesAtOnceSink&) = delete;
    
    TwoBytesAtOnceSink(TwoBytesAtOnceSink&& other) :
    data(std::move(other.data)) { }
    
    TwoBytesAtOnceSink& operator=(TwoBytesAtOnceSink&& other) {
        data = std::move(other.data);
        return *this;
    }

    std::streamsize write(const char* s, std::streamsize n) {
        std::streamsize len = n >= 2 ? 2 : n;
        for (std::streamsize i = 0; i < len; i++) {
            data.push_back(s[i]);
        }
        return len;
    }
    
    void flush() { }
    
    std::string& get_data() {
        return data;
    }
};

#endif	/* STATICLIB_IO_TWOBYTESATONCESINK_HPP */

