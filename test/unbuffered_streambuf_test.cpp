/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

