/* 
 * File:   streambuf_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:29 PM
 */

#include <iostream>
#include <sstream>
#include <cassert>

#include "staticlib/io/streambuf_sink.hpp"

namespace io = staticlib::io;

void test_write() {
    std::ostringstream stream{};
    io::streambuf_sink sink{stream.rdbuf()};
    auto written = sink.write("foo", 3);
    (void) written;
    assert(3 == written);
    assert("foo" == stream.str());
}

int main() {
    test_write();

    return 0;
}

