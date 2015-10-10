/* 
 * File:   streambuf_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:40 PM
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include "staticlib/io/streambuf_source.hpp"

namespace io = staticlib::io;

void test_read() {
    std::istringstream stream{"foo"};
    io::streambuf_source src{stream.rdbuf()};
    std::string dest{};
    dest.resize(3);
    auto read = src.read(std::addressof(dest.front()), 3);
    (void) read;
    assert(3 == read);
    assert("foo" == dest);
}

int main() {
    test_read();

    return 0;
}

