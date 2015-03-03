
%module(docstring="The Yadoms Script API") yScriptApiWrapper

%{
/* Put headers and other declarations here */
#include <vector>
#include <string>
#include <utility>
#include <shared/script/yScriptApi/IYScriptApi.h>
%}

%begin %{
/* Add some Hack to build in debug version with Pyhton release library, when using STL */
/* More informations here : https://github.com/swig/swig/issues/325 */
#if defined(_DEBUG) && defined(SWIG_PYTHON_INTERPRETER_NO_DEBUG)
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
#endif
%}

%include stl.i

/* Add method documentation */
/* Can not use Doxygen comments from the C++ code, because "autodoc" is a Python specificity */
/* TODO à compléter */
%feature("autodoc", "Read the last known state of the keyword (empty if no known state). State returned as string.") readKeyword;

%include <shared/script/yScriptApi/IYScriptApi.h>
