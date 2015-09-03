
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

/* Add method documentation */
/* TODO : virer toutes les lignes "%feature("autodoc"..." lorsqu'elles seront générées */
/* Can not use Doxygen comments from the C++ code, because "autodoc" is a Python specificity */
%feature("autodoc", "Read the last known state of the keyword. State returned as string (empty if no known state).") readKeyword;
%feature("autodoc", "Wait for a new acquisition on a keyword, with timeout (format is \"hh:mm:ss.xxx\". No timeout if empty). State returned as string (empty if timeout).") waitForAcquisition;
%feature("autodoc", "Wait for a new acquisition on a keyword list, with timeout (format is \"hh:mm:ss.xxx\". No timeout if empty). Returned value is a pair of the keyword Id who changed, and its new value. The keyword Id is -1 if timeout.") waitForAcquisitions;
%feature("autodoc", "Change state of a keyword.") writeKeyword;
%feature("autodoc", "Send a notification.") sendNotification;
%feature("autodoc", "Get general informations. Supported values for key are sunrise, sunset, latitude, longitude, altitude, yadomsServerOS, yadomsServerVersion.") getInfo;
%feature("autodoc", "Can be used to prevent a rule for auto-restart (if set to false).") ruleEnable;

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

