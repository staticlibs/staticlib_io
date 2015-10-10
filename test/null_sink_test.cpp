/* 
 * File:   null_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:20 PM
 */

#include <cassert>

#include "staticlib/io/null_sink.hpp"

namespace io = staticlib::io;

void test_write_and_flush() {
    io::null_sink sink{};
    auto written = sink.write("foo", 3);
    (void) written;
    assert(3 == written);
    sink.flush();
}

int main() {
    test_write_and_flush();

    return 0;
}

