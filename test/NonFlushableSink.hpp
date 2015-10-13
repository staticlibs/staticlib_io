/* 
 * File:   NonFlushableSink.hpp
 * Author: alex
 *
 * Created on October 13, 2015, 8:01 AM
 */

#ifndef NONFLUSHABLESINK_HPP
#define	NONFLUSHABLESINK_HPP

class NonFlushableSink {
    size_t count = 0;

public:

    NonFlushableSink() { }

    NonFlushableSink(const NonFlushableSink&) = delete;

    NonFlushableSink& operator=(const NonFlushableSink&) = delete;

    NonFlushableSink(NonFlushableSink&& other) :
    count(other.count) { }

    NonFlushableSink& operator=(NonFlushableSink&& other) { 
        count = other.count;
        return *this;
    }

    std::streamsize write(const char*, std::streamsize n) {
        count += static_cast<size_t> (n);
        return n;
    }

    size_t get_count() {
        return count;
    }

};

#endif	/* NONFLUSHABLESINK_HPP */

