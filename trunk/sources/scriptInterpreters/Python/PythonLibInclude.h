#pragma once

#if defined WIN32 && defined _DEBUG
   #undef _DEBUG // Undef _DEBUG to use only release version of Python.lib. It permits to download the Pyhton installer version (https://www.python.org/downloads/windows/)
   #include <Python.h>
   #define _DEBUG
#else
   #include <Python.h>
#endif

