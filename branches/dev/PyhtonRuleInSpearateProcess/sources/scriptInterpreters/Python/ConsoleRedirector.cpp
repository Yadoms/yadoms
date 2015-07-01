#include "stdafx.h"
#include "ConsoleRedirector.h"
//TODO ménage
//#include "PythonLibInclude.h"
//#include "PythonBorrowedObject.h"
#include "PythonException.hpp"


CConsoleRedirector::CConsoleRedirector()
{
   //
   // Python code used to redirect stdout/stderr
   // This code :
   // - create the yCatchStdOut class which :
   //   - implement the write method to overwrite this from sys.stdout/sys.stderr
   //   - the write method send string to the yScriptApi (log method) when a line is complete
   //       (remove the newLine character)
   //   - the write method send to stdErr if yCatchStdOut class was created with the
   //       corresponding flag, stdOut else
   // - define the yRedirectConsole which make 2 instances of yCatchStdOut class,
   //     to redirect sys.stdout and sys.stderr
   //
   //TODO virer ?
   static const std::string pythonCode =
"import sys\n\
class yCatchStdOut:\n\
   def __init__(self, yApi, stdErr):\n\
      self.yApi = yApi\n\
      self.stdErr = stdErr\n\
      self.buffer = ''\n\
   def write(self, txt):\n\
      self.buffer += txt;\n\
      if self.buffer.endswith('\\n'):\n\
         self.buffer = self.buffer[:-1]\n\
         if self.stdErr:\n\
            self.yApi.logError(self.buffer)\n\
         else:\n\
            self.yApi.log(self.buffer)\n\
         self.buffer = ''\n\
      \n\
def yRedirectConsole(yApi):\n\
   sys.stdout = yCatchStdOut(yApi, False)\n\
   sys.stderr = yCatchStdOut(yApi, True)\n\
";

   try
   {
      //TODO
      //// Import the main module
      //CPythonBorrowedObject pyMainModule(PyImport_AddModule("__main__"));
      //if (pyMainModule.isNull())
      //   throw CPythonException("unable to get __main__");

      //// Declare yCatchStdOut class and yRedirectConsole function
      //if (PyRun_SimpleString(pythonCode.c_str()) != 0)
      //   throw CPythonException("error declaring yCatchStdOut class and yRedirectConsole");

      //// Activate the redirection
      //CPythonObject pyRedirectConsoleFunction(PyObject_GetAttrString(*pyMainModule, "yRedirectConsole"));
      //if (pyRedirectConsoleFunction.isNull() || PyCallable_Check(*pyRedirectConsoleFunction) == 0)
      //   throw CPythonException("yRedirectConsole is not of expected type");
      //CPythonObject pyRedirectConsoleFunctionResult(PyObject_CallObject(*pyRedirectConsoleFunction, *yApiTuple));
      //if (pyRedirectConsoleFunctionResult.isNull())
      //   throw CPythonException("yRedirectConsole function returned with error");
   }
   catch (CPythonException&e)
   {
      throw CPythonException(std::string("Unable to redirect console : ") + e.what());
   }
}

CConsoleRedirector::~CConsoleRedirector()
{   
}




