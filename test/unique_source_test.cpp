/* 
 * File:   unique_source_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 3:18 PM
 */

#include <iostream>
#include <memory>
#include <array>
#include <cassert>

#include "staticlib/io/unique_source.hpp"
#include "staticlib/io/counting_source.hpp"

#include "NonCopyableSource.hpp"

namespace io = staticlib::io;

void test_movable() {
    io::unique_source<NonCopyableSource> source{new NonCopyableSource{}};
    io::counting_source<io::unique_source<NonCopyableSource>> wrapped{std::move(source)};
    std::array<char, 3> buf;
    auto read = wrapped.read(buf.data(), 3);
    (void) read;
    assert(3 == read);
    assert(3 == wrapped.get_count());
    assert(3 == wrapped.get_source().get_source().get_count());
}

int main() {
    test_movable();

    return 0;
}

