/* 
 * File:   buffered_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 8:39 AM
 */

#include <iostream>
#include <string>
#include <cassert>

#include "staticlib/io/buffered_sink.hpp"

#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_buffer_size() {
    io::buffered_sink<TwoBytesAtOnceSink, 4> sink{TwoBytesAtOnceSink{}};
    auto written1 = sink.write("foo", 3);
    (void) written1;
    assert(3 == written1);
    assert(0 == sink.get_sink().get_data().size());
    auto written2 = sink.write("42", 2);
    (void) written2;
    assert(2 == written2);
    assert(3 == sink.get_sink().get_data().size());
    assert("foo" == sink.get_sink().get_data());
}

void test_flush() {
    io::buffered_sink<TwoBytesAtOnceSink, 4> sink{TwoBytesAtOnceSink{}};
    auto written = sink.write("foo", 3);
    (void) written;
    assert(3 == written);
    assert(0 == sink.get_sink().get_data().size());
    auto flushed = sink.flush();
    (void) flushed;
    assert(3 == flushed);
    assert(3 == sink.get_sink().get_data().size());
    assert("foo" == sink.get_sink().get_data());
}

void test_overwrite() {
    io::buffered_sink<TwoBytesAtOnceSink, 4> sink{TwoBytesAtOnceSink{}};
    auto written = sink.write("foo42", 5);
    (void) written;
    assert(5 == written);
    assert(5 == sink.get_sink().get_data().size());
    assert("foo42" == sink.get_sink().get_data());
}

int main() {
    test_buffer_size();
    test_flush();
    test_overwrite();

    return 0;
}

