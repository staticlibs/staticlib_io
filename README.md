Staticlibs IO library
=====================

[![travis](https://travis-ci.org/staticlibs/staticlib_io.svg?branch=master)](https://travis-ci.org/staticlibs/staticlib_io)
[![appveyor](https://ci.appveyor.com/api/projects/status/github/staticlibs/staticlib_io?svg=true)](https://ci.appveyor.com/project/staticlibs/staticlib-io)

This project is a part of [Staticlibs](http://staticlibs.net/).

This project implements stream-like operations on the arbitrary `Source` and `Sink` template implementations.

This library is similar in nature with [Boost.Iostreams](http://www.boost.org/doc/libs/1_59_0/libs/iostreams/doc/index.html)
library, but much less powerful and much less complex.

Library follows [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines) using `span` instead of "buffer pointer + length" pair.

This library is header-only and depends on [staticlib_config](https://github.com/staticlibs/staticlib_config) and [staticlib_support](https://github.com/staticlibs/staticlib_support).

Link to the [API documentation](http://staticlibs.github.io/staticlib_io/docs/html/namespacestaticlib_1_1io.html).

Sources and Sinks
-----------------

This library is built around the notions of the `Source` (input stream) and the `Sink` (output stream)
template concepts.

`Source` concept:

    class Source {
        // move constructor and assignment operator
        Source(Source&& other);
        Source& operator=(Source&& other);

        // read method
        std::streamsize read(sl::io::span<char> span);
    };

`Sink` concept:

    class Sink {
        // move constructor and assignment operator
        Sink(Sink&& other);
        Sink& operator=(Sink&& other);

        // write method
        std::streamsize write(sl::io::span<const char> span);

        // flush method
        std::streamsize flush();
    };

Library implements a set of generic operations (`read_all`, `copy`) on arbitrary sources
and sinks and a number of template wrappers like buffered and counting sources and sinks.

See usage examples in [tests](https://github.com/staticlibs/staticlib_io/tree/master/test).

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2018-06-16**

 * version 1.2.8
 * `memory_sink` added

**2018-06-12**

 * version 1.2.7
 * support string literals in `span`

**2018-06-04**

 * version 1.2.6
 * hex rework

**2018-03-20**

 * version 1.2.5
 * more hex utils

**2018-02-22**

 * version 1.2.3
 * allow empty state for span

**2017-12-22**

 * version 1.2.2
 * vs2017 support

**2017-04-08**

 * version 1.2.1
 * `span` moved into this project

**2017-02-04**

 * version 1.2.0
 * use GSL-like `span`

**2016-01-16**

 * version 1.1.5
 * minor CMake changes

**2015-12-13**

 * version 1.1.4
 * `reference_*` source and sink added

**2015-11-24**

 * version 1.1.3
 * endian utils removed
 * test cleanup
 * cmake scripts cleanup

**2015-11-14**

 * version 1.1.2
 * use `std::char_traits<char>::eof()` instead of configurable template arg

**2015-11-09**

 * version 1.1.1 
 * `string_source` fix

**2015-11-06**

 * version 1.1.0 
 * `array_source` and `string` source and sink added

**2015-11-01**

 * version 1.0.0 
