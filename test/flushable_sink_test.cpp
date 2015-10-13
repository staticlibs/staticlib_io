/* 
 * File:   flushable_sink_test.cpp
 * Author: alex
 *
 * Created on October 13, 2015, 7:57 AM
 */

#include <iostream>
#include <memory>
#include <cassert>

#include "staticlib/io/flushable_sink.hpp"
#include "staticlib/io/counting_sink.hpp"

#include "NonFlushableSink.hpp"

namespace io = staticlib::io;

void test_flushable() {
    io::flushable_sink<NonFlushableSink> sink{NonFlushableSink{}};
    io::counting_sink<io::flushable_sink<NonFlushableSink>>wrapped{std::move(sink)};
    auto written = wrapped.write("foo", 3);
    (void) written;
    assert(3 == written);
    assert(3 == wrapped.get_count());
    assert(3 == wrapped.get_sink().get_sink().get_count());
}

int main() {
    test_flushable();

    return 0;
}


