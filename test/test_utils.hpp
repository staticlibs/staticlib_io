/*
 * Copyright 2017, alex at staticlibs.net
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
 * File:   test_utils.hpp
 * Author: alex
 *
 * Created on January 29, 2017, 1:25 PM
 */

#ifndef STATICLIB_IO_TEST_TEST_UTILS_HPP
#define	STATICLIB_IO_TEST_TEST_UTILS_HPP

#include <functional>

#include "staticlib/io/IOException.hpp"

bool throws_exc(std::function<void() > fun) {
    namespace si = staticlib::io;
    try {
        fun();
    } catch (const si::IOException& e) {
        (void) e;
        return true;
    }
    return false;
}

#endif	/* STATICLIB_IO_TEST_TEST_UTILS_HPP */

