/* 
 * File:   shared_sink_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:02 PM
 */

#include <iostream>
#include <memory>
#include <cassert>

#include "staticlib/io/shared_sink.hpp"

#include "NonCopyableSink.hpp"

namespace io = staticlib::io;

void test_copyable() {
    std::shared_ptr<NonCopyableSink> sink{new NonCopyableSink{}};
    io::shared_sink<NonCopyableSink> shared1{sink};
    io::shared_sink<NonCopyableSink> shared2{sink};
    auto written1 = shared1.write("foo", 3);
    (void) written1;
    assert(3 == written1);
    assert(3 == sink->get_count());
    assert(3 == shared1.get_sink().get_count());
    assert(3 == shared2.get_sink().get_count());
    auto written2 = shared2.write("42", 2);
    (void) written2;
    assert(2 == written2);
    assert(5 == sink->get_count());
    assert(5 == shared1.get_sink().get_count());
    assert(5 == shared2.get_sink().get_count());
}

int main() {
    test_copyable();

    return 0;
}

