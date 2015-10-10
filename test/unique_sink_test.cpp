/* 
 * File:   unique_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 3:10 PM
 */

#include <iostream>
#include <memory>
#include <cassert>

#include "staticlib/io/unique_sink.hpp"
#include "staticlib/io/counting_sink.hpp"

#include "NonCopyableSink.hpp"

namespace io = staticlib::io;

void test_movable() {
    io::unique_sink<NonCopyableSink> sink{new NonCopyableSink{}};
    io::counting_sink<io::unique_sink<NonCopyableSink>> wrapped{std::move(sink)};    
    auto written = wrapped.write("foo", 3);
    (void) written;
    assert(3 == written);
    assert(3 == wrapped.get_count());
    assert(3 == wrapped.get_sink().get_sink().get_count());
}

int main() {
    test_movable();

    return 0;
}

