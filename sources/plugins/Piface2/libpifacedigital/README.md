libpifacedigital
================

A simple library for controlling
[PiFace Digital](http://www.piface.org.uk/products/piface_digital/).
Hides the SPI file descriptors so the user only has to deal with `hw_addr`.

Building
--------

To build the library, first install the required dependency
[libmcp23s17](https://github.com/piface/libmcp23s17). Then run:

    $ make

Install the library to `/usr/local` using:

    $ make install

To test the library, compile and execute the example program:

    $ make example
    $ ./example

A command line utility named `pifacedigital` is also available. Build with:

    $ make pifacedigital
    $ ./pifacedigital read input
    $ ./pifacedigital --help

Usage
-----

See `example.c` for example usage.

Compile your software with the following flags:

    -lpifacedigital -lmcp23s17

You can specify the path of the libraries manually, using:

    -I/path/to/headers -L/path/to/libpifacedigital -lpifacedigital -L/path/to/mcp23s17 -lmcp23s17

Documentation
-------------

An online version of the documentation is available at http://piface.github.io/libpifacedigital.

Build it with (assuming that you are in the directory of the cloned repository):

    $ cd docs/
    $ doxygen pifacedigital-doc.conf

To view as HTML, point your browser to `docs/html/index.html`.

To view as PDF:

    $ cd latex/
    $ make

The pdf is called `refman.pdf`.
