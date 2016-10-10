libmcp23s17
===========

A simple library for accessing the [MCP23S17](http://www.microchip.com/MCP23S17)
port expander through SPI.

Building
--------

To build the library, use:

    $ make

Install the library to `/usr/local` using:

    $ make install

To test the library, compile and execute the example program:

    $ make example
    $ ./example

Another example program, using interrupts, is available. Build with:

    $ make interrupt_example
    $ ./interrupt_example

Usage
-----

See `example.c` and `interrupt_example.c` for ideas on how to use.

Compile your software with the following flag:

    -lmcp23s17

You can specify the path of the library manually, using:

    -I/path/to/headers -L/path/to/libmcp23s17 -lmcp23s17

Documentation
-------------

An online version of the documentation is available at http://piface.github.io/libmcp23s17.

Build it with (assuming that you are in the directory of the cloned repository):

    $ cd docs/
    $ doxygen libmcp23s17-doc.conf

To view as HTML, point your browser to `docs/html/index.html`.

To view as PDF:

    $ cd latex/
    $ make

The pdf is called `refman.pdf`.
