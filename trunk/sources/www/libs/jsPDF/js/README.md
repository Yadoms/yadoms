#jsPDF

[![Inline docs](http://inch-ci.org/github/MrRio/jsPDF.svg?branch=master)](http://inch-ci.org/github/MrRio/jsPDF)

**Generate PDF files in client-side JavaScript.**

You can [catch me on twitter](http://twitter.com/MrRio): [@MrRio](http://twitter.com/MrRio) or head over to [my company's website](http://parall.ax) for consultancy. 

## Creating your first document

See examples/basic.html. There's a live editor example at index.html.

```javascript

var doc = new jsPDF();
doc.text(20, 20, 'Hello world.');
doc.save('Test.pdf');
```

**Head over to [jsPDF.com](http://jspdf.com) for details or [_here_](http://mrrio.github.io/jsPDF/) for our most recent live editor and examples.**

## Checking out the source

```bash
git clone --recursive git://github.com/MrRio/jsPDF.git
```

## Building

To build, simply run the 'make' command. This will fetch all npm and bower deps, then compile minified JS files.

## Running locally

Due to certain restrictions that local files have, you'll need to run a web server. Just run:

```
npm start
```

You can then access the site at localhost:8000

## Credits

Big thanks to Daniel Dotsenko from [Willow Systems Corporation](http://willow-systems.com) for making huge contributions to the codebase. 

Thanks to Ajaxian.com for [featuring us back in 2009](http://ajaxian.com/archives/dynamically-generic-pdfs-with-javascript).

Everyone else that's contributed patches or bug reports. You rock.

## License

(MIT License)

Copyright (c) 2010-2014 James Hall, https://github.com/MrRio/jsPDF

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
