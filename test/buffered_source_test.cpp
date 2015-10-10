/* 
 * File:   buffered_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 9:20 AM
 */

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/buffered_source.hpp"

#include "TwoBytesAtOnceSource.hpp"

namespace io = staticlib::io;

void test_buffered() {
    io::buffered_source<TwoBytesAtOnceSource, 3> src{TwoBytesAtOnceSource{"foo42"}};
    std::string dest{};
    dest.resize(3);
    src.read(std::addressof(dest.front()), 3);
    assert(3 == dest.size());
    assert("foo" == dest);
}

void test_overread() {
    io::buffered_source<TwoBytesAtOnceSource, 3> src{TwoBytesAtOnceSource{"foo42"}};
    std::string dest{};
    dest.resize(5);
    src.read(std::addressof(dest.front()), 5);
    assert(5 == dest.size());
    assert("foo42" == dest);
}

int main() {
    test_buffered();
    test_overread();

    return 0;
}

