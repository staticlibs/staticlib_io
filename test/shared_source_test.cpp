/* 
 * File:   shared_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:22 PM
 */

#include <iostream>
#include <memory>
#include <array>
#include <cassert>

#include "staticlib/io/shared_source.hpp"

#include "NonCopyableSource.hpp"

namespace io = staticlib::io;

void test_copyable() {
    std::shared_ptr<NonCopyableSource> source{new NonCopyableSource{}};
    io::shared_source<NonCopyableSource> shared1{source};
    io::shared_source<NonCopyableSource> shared2{source};
    std::array<char, 3> buf;
    auto read1 = shared1.read(buf.data(), 3);
    (void) read1;
    assert(3 == read1);
    assert(3 == source->get_count());
    assert(3 == shared1.get_source().get_count());
    assert(3 == shared2.get_source().get_count());
    auto read2 = shared2.read(buf.data(), 2);
    (void) read2;
    assert(2 == read2);
    assert(5 == source->get_count());
    assert(5 == shared1.get_source().get_count());
    assert(5 == shared2.get_source().get_count());
}

int main() {
    test_copyable();

    return 0;
}

