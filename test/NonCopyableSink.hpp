/* 
 * File:   NonCopyableSink.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:04 PM
 */

#ifndef NONCOPYABLESINK_HPP
#define	NONCOPYABLESINK_HPP

class NonCopyableSink {
    size_t count = 0;
    
public:

    NonCopyableSink() { }

    NonCopyableSink(const NonCopyableSink&) = delete;

    NonCopyableSink& operator=(const NonCopyableSink&) = delete;

    NonCopyableSink(NonCopyableSink&&) = delete;

    NonCopyableSink& operator=(NonCopyableSink&&) = delete;

    std::streamsize write(const char*, std::streamsize n) {
        count += n;
        return n;
    }

    std::streamsize flush() {
        return 0;
    }
    
    size_t get_count() {
        return count;
    }

};

#endif	/* NONCOPYABLESINK_HPP */

