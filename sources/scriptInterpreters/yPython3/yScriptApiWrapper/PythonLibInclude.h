#pragma once


#ifdef PYTHON3_USE_SOURCES

   #include <Python.h>

#else

   #if defined WIN32 && defined _DEBUG
      // Include these low level headers before undefing _DEBUG. Otherwise when doing
      // a debug build against a release build of python the compiler will end up 
      // including these low level headers without DEBUG enabled, causing it to try 
      // and link release versions of this low level C api.
      #include <basetsd.h>
      #include <assert.h>
      #include <ctype.h>
      #include <errno.h>
      #include <io.h>
      #include <math.h>
      #include <sal.h>
      #include <stdarg.h>
      #include <stddef.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <string.h>
      #include <sys/stat.h>
      #include <time.h>
      #include <wchar.h>
      #undef _DEBUG // Undef _DEBUG to use only release version of Python.lib. The debug version of Python.lib is not provided with the Windows installer version (https://www.python.org/downloads/windows/)
      #include <Python.h>
      #define _DEBUG
   #else
      #include <Python.h>
   #endif

#endif // PYTHON3_USE_SOURCES