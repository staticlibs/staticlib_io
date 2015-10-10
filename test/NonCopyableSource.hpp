/* 
 * File:   NonCopyableSource.hpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:03 PM
 */

#ifndef NONCOPYABLESOURCE_HPP
#define	NONCOPYABLESOURCE_HPP

class NonCopyableSource {
    size_t count = 0;
    
public:
    NonCopyableSource() { }

    NonCopyableSource(const NonCopyableSource&) = delete;

    NonCopyableSource& operator=(const NonCopyableSource&) = delete;

    NonCopyableSource(NonCopyableSource&&) = delete;

    NonCopyableSource& operator=(NonCopyableSource&&) = delete;

    std::streamsize read(char*, std::streamsize n) {
        count += n;
        return n;
    }
    
    size_t get_count() {
        return count;
    }

};

#endif	/* NONCOPYABLESOURCE_HPP */

