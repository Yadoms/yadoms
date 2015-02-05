
%module yScriptApiWrapper

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


%include <shared/script/yScriptApi/IYScriptApi.h>