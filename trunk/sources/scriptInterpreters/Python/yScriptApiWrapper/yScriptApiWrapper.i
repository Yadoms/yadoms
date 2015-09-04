
%module(docstring="The Yadoms Script API") yScriptApiWrapper

%{
/* Put headers and other declarations here */
#include <vector>
#include <string>
#include <utility>
#include <shared/script/yScriptApi/IYScriptApi.h>

#include <iostream>

#include "yScriptApiInstance.h"

%}


%begin %{
/* Add some Hack to build in debug version with Python release library, when using STL */
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

// Used by IYScriptApi::waitForAcquisitions
%template() std::vector<int>;
%template() std::pair<int, std::string>;

/* Add an exception handler to each library method */
%exception {
   try {
      $action
   } catch (std::exception &e) {
      PyErr_SetString(PyExc_RuntimeError, const_cast<char*>(e.what()));
      return NULL;
   } catch (...) {
      PyErr_SetString(PyExc_RuntimeError, "Unknown exception");
      return NULL;
   }
}

%include <shared/script/yScriptApi/IYScriptApi.h>

// yScriptApi instance creation method
shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId);
%feature("autodoc", "Create the yScriptApi instance (need the context accessor ID provided by Yadoms)") createScriptApiInstance;

// yScriptApi instance destruction method
void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi);
%feature("autodoc", "Delete the yScriptApi instance") deleteScriptApiInstance;

