/* 
 * File:   unbuffered_streambuf_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 2:47 PM
 */

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/unbuffered_streambuf.hpp"

#include "TwoBytesAtOnceSource.hpp"
#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_unbuffered_source() {
    TwoBytesAtOnceSource src{"abc"};
    io::unbuffered_istreambuf<TwoBytesAtOnceSource> istreambuf{std::move(src)};
    std::istream stream{std::addressof(istreambuf)};
    // pass it somewhere through std api
    std::array<char, 3> buf;
    auto read = stream.rdbuf()->sgetn(buf.data(), 3);
    (void) read;
    assert(2 == read);
    std::string res{buf.data(), static_cast<size_t>(read)};
    assert("ab" == res);
}

void test_unbuffered_sink() {
    TwoBytesAtOnceSink sink{};
    io::unbuffered_ostreambuf<TwoBytesAtOnceSink> ostreambuf{std::move(sink)};
    std::ostream stream{std::addressof(ostreambuf)};
    // pass it somewhere through std api
    auto written = stream.rdbuf()->sputn("abc", 3);
    (void) written;
    assert(2 == written);
    assert("ab" == ostreambuf.get_sink().get_data());
}

int main() {
    test_unbuffered_source();
    test_unbuffered_sink();

    return 0;
}

