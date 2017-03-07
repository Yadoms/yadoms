#include "stdafx.h"

// To deal with OpenSSL versions built with old VisualStudio versions (before MSVC 2015),
// we have to declared the __iob_func function (removed from more recents SDK)

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
   return _iob;
}
