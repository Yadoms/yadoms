
%module(docstring="The Yadoms Script API") yScriptApiWrapper

%{
/* Put headers and other declarations here */
#include <vector>
#include <string>
#include <utility>
#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/script/yScriptApi/WaitForEventResult.h>

#include <iostream>

#include "yScriptApiInstance.h"

%}


%begin %{
/* Add some Hack to build in debug version with Python release library, when using STL */
/* More informations here : https://github.com/swig/swig/issues/325 */
#if defined(_DEBUG) && defined(SWIG_PYTHON_INTERPRETER_NO_DEBUG)
#include <crtdefs.h>
#endif
%}

%include stl.i

// Used by IYScriptApi::waitForEvent
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
%include <shared/script/yScriptApi/WaitForEventResult.h>

// yScriptApi instance creation method
// Create the yScriptApi instance (need the context accessor ID provided by Yadoms)
shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId);

// yScriptApi instance destruction method
void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi);

