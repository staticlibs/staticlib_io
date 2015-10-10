/* 
 * File:   counting_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:12 PM
 */

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/counting_source.hpp"

#include "TwoBytesAtOnceSource.hpp"

namespace io = staticlib::io;

void test_count() {
    io::counting_source<TwoBytesAtOnceSource> src{TwoBytesAtOnceSource{"42"}};
    std::array<char, 2> arr{{}};
    auto read = src.read(arr.data(), arr.size());
    (void) read;
    assert(2 == read);
    assert(2 == src.get_count());
}

void test_count_overflow() {
    io::counting_source<TwoBytesAtOnceSource> src{TwoBytesAtOnceSource{"foo42"}};
    std::array<char, 5> arr{{}};
    auto read = src.read(arr.data(), arr.size());
    (void) read;
    assert(2 == read);
    assert(2 == src.get_count());
}

int main() {
    test_count();
    test_count_overflow();

    return 0;
}

