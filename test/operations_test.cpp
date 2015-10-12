/* 
 * File:   operations_test.cpp
 * Author: alex
 *
 * Created on October 10, 2015, 1:23 PM
 */

#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include "staticlib/io/operations.hpp"

#include "TwoBytesAtOnceSource.hpp"
#include "TwoBytesAtOnceSink.hpp"

namespace io = staticlib::io;

void test_write_not_all() {
    TwoBytesAtOnceSink sink{};
    auto written = sink.write("abc", 3);
    (void) written;
    assert(2 == written);
    assert(2 == sink.get_data().length());
    assert("ab" == sink.get_data());
}

void test_write_all_buffer() {
    TwoBytesAtOnceSink sink{};
    io::write_all(sink, "abc", 3);
    assert(3 == sink.get_data().length());
    assert("abc" == sink.get_data());
}

void test_write_all_str() {
    TwoBytesAtOnceSink sink{};
    std::string data{"abc"};
    io::write_all(sink, data);
    assert(3 == sink.get_data().length());
    assert("abc" == sink.get_data());
}

void test_read_not_all() {
    TwoBytesAtOnceSource src{"abc"};
    std::array<char, 4> buf;
    auto read = src.read(buf.data(), 3);
    (void) read;
    assert(2 == read);
    std::string res{buf.data(), static_cast<size_t>(read)};
    assert(2 == res.length());
    assert("ab" == res);
}

void test_read_all() {
    TwoBytesAtOnceSource src{"abc"};
    std::array<char, 4> buf;
    auto read = io::read_all(src, buf.data(), 3);
    (void) read;
    assert(3 == read);
    std::string res{buf.data(), read};
    assert(3 == res.length());
    assert("abc" == res);
}

void test_read_exact() {
    TwoBytesAtOnceSource src{"abc"};
    std::array<char, 5> buf;
    bool thrown = false;
    try {
        io::read_exact(src, buf.data(), 4);
    } catch (const std::ios_base::failure& e) {
        (void) e;
        thrown = true;
    }
    (void) thrown;
    assert(thrown);
}

void test_copy() {
    TwoBytesAtOnceSink sink{};
    TwoBytesAtOnceSource src{"abc"};
    std::array<char, 2> buf;
    auto copied = io::copy_all(src, sink, buf.data(), buf.size());
    (void) copied;
    assert(3 == copied);
    assert(3 == sink.get_data().length());
    assert("abc" == sink.get_data());
}

void test_skip() {
    // todo
}

// todo: endianness


int main() {
    test_write_not_all();
    test_write_all_buffer();
    test_write_all_str();
    test_read_not_all();
    test_read_all();
    test_read_exact();
    test_copy();
    test_skip();

    return 0;
}

