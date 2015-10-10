/* 
 * File:   copying_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 12:48 PM
 */

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/copying_source.hpp"

#include "TwoBytesAtOnceSource.hpp"
#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_copy() {
    io::copying_source<TwoBytesAtOnceSource, TwoBytesAtOnceSink> src{TwoBytesAtOnceSource{"42"}, TwoBytesAtOnceSink{}};
    std::string dest{};
    dest.resize(2);
    auto read = src.read(std::addressof(dest.front()), 2);
    (void) read;
    assert(2 == read);
    assert(2 == dest.size());
    assert("42" == dest);
    assert(2 == src.get_sink().get_data().size());
    assert("42" == src.get_sink().get_data());
}

void test_omit_tail() {
    io::copying_source<TwoBytesAtOnceSource, TwoBytesAtOnceSink> src{TwoBytesAtOnceSource{"42foo"}, TwoBytesAtOnceSink{}};
    std::string dest{};
    dest.resize(2);
    auto read = src.read(std::addressof(dest.front()), 2);
    (void) read;
    assert(2 == read);
    assert(2 == dest.size());
    assert("42" == dest);
    assert(2 == src.get_sink().get_data().size());
    assert("42" == src.get_sink().get_data());
}

int main() {
    test_copy();
    test_omit_tail();

    return 0;
}

