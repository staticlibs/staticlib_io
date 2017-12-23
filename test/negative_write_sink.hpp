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
 * File:   negative_write_sink.hpp
 * Author: alex
 *
 * Created on January 29, 2017, 2:09 PM
 */

#ifndef STATICLIB_IO_TEST_NEGATIVE_WRITE_SINK_HPP
#define STATICLIB_IO_TEST_NEGATIVE_WRITE_SINK_HPP

class negative_write_sink {
public:

    negative_write_sink() { }

    std::streamsize write(sl::io::span<const char>) {
        return -2;
    }

    std::streamsize flush() {
        return 0;
    }
};

#endif /* STATICLIB_IO_TEST_NEGATIVE_WRITE_SINK_HPP */

