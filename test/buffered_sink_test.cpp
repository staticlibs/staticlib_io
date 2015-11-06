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

void test_make() {
    auto sink = io::make_buffered_sink(TwoBytesAtOnceSink{});
}

int main() {
    test_buffer_size();
    test_flush();
    test_overwrite();
    test_make();

    return 0;
}

