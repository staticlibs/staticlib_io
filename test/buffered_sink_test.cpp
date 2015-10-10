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
    sink.write("foo", 3);
    assert(0 == sink.get_sink().get_data().size());
    sink.write("42", 2);
    assert(3 == sink.get_sink().get_data().size());
    assert("foo" == sink.get_sink().get_data());
}

void test_flush() {
    io::buffered_sink<TwoBytesAtOnceSink, 4> sink{TwoBytesAtOnceSink{}};
    sink.write("foo", 3);
    assert(0 == sink.get_sink().get_data().size());
    sink.flush();
    assert(3 == sink.get_sink().get_data().size());
    assert("foo" == sink.get_sink().get_data());
}

void test_overwrite() {
    io::buffered_sink<TwoBytesAtOnceSink, 4> sink{TwoBytesAtOnceSink{}};
    sink.write("foo42", 5);
    assert(5 == sink.get_sink().get_data().size());
    assert("foo42" == sink.get_sink().get_data());
}

int main() {
    test_buffer_size();
    test_flush();
    test_overwrite();

    return 0;
}

