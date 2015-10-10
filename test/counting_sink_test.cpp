/* 
 * File:   counting_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:07 PM
 */

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/counting_sink.hpp"

#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_count() {
    io::counting_sink<TwoBytesAtOnceSink> sink{TwoBytesAtOnceSink{}};
    auto written = sink.write("42", 2);
    (void) written;
    assert(2 == written);
    assert(2 == sink.get_count());
}

void test_count_overflow() {
    io::counting_sink<TwoBytesAtOnceSink> sink{TwoBytesAtOnceSink{}};
    auto written = sink.write("foo", 3);
    (void) written;
    assert(2 == written);
    assert(2 == sink.get_count());
}

int main() {
    test_count();
    test_count_overflow();

    return 0;
}

