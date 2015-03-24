#include "stdafx.h"
#include "Runner.h"
#include <shared/Log.h>
#include "PythonException.hpp"
#include "PythonObject.h"
#include "PythonBorrowedObject.h"
#include <shared/DataContainer.h>
#include "ScriptLoader.h"
#include "SubInterpreter.h"
#include "ConsoleRedirector.h"
#include "swigpyrun.h"  // Generated file (at pre-build-step)


CRunner::CRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration)
   :m_scriptPath(scriptPath), m_scriptConfiguration(scriptConfiguration)
{
}

CRunner::~CRunner()
{
   interrupt();
}

void CRunner::run(shared::script::yScriptApi::IYScriptApi& context)
{
   m_lastError.clear();

   try
   {
      // Create sub-interpreter (needed for thread context)
      CSubInterpreter interpreter;

      // Load script file
      CScriptLoader loader(m_scriptPath);
      loader.load();

      // Run the script
      CPythonObject pyContext(SWIG_NewPointerObj(&context, SWIG_TypeQuery("shared::script::yScriptApi::IYScriptApi *"), 0));
      CPythonObject tuple(PyTuple_New(1));
      Py_XINCREF(*pyContext); // Increment pyContext reference count, because PyTuple_SetItem steals the reference
      if (PyTuple_SetItem(*tuple, 0, *pyContext))
         throw CPythonException("Unable to create context");
      CPythonObject ymainFunction(PyObject_GetAttrString(loader.module().get(), "yMain"));
      if (ymainFunction.isNull() || PyCallable_Check(*ymainFunction) == 0)
         throw CPythonException("Script exited with error");
      CConsoleRedirector stdoutRedirector(tuple);
      CPythonObject pyReturnValue(PyObject_CallObject(*ymainFunction, *tuple));

      // If errno = 4 (interrupt function call), it's because of stopping Yadoms (Python is catching CTRL-C),
      // so don't throw a CPythonException, or rule will be set to error state, and not started again at next Yadoms startup.
      if (pyReturnValue.isNull())
      {
         if (!isPythonError(PyExc_IOError, 4))
            throw CPythonException("Script yMain function returned with error");

         // We are here because because receive a CTRL-C. So Yadoms will end, 
         // just wait that thread stop is required
         YADOMS_LOG(information) << m_scriptPath << " : script exited";
         boost::this_thread::sleep_for(boost::chrono::seconds(10000)); // Should throw boost::thread_interrupted
         YADOMS_LOG(error) << m_scriptPath << " : timeout waiting for stop rule thread";
      }

      YADOMS_LOG(information) << m_scriptPath << " : script exited";
   }
   catch(CPythonException& e)
   {
      std::string error(std::string(" : error running script, ") + e.what());
      YADOMS_LOG(error) << m_scriptPath << error;
      context.logError(error);
      m_lastError = e.what();
   }
}

void CRunner::interrupt()
{
   PyErr_SetInterrupt();
}

bool CRunner::isOk() const
{
   return m_lastError.empty();
}

std::string CRunner::error() const
{
   return m_lastError;
}

bool CRunner::isPythonError(PyObject* pyException, int pyErrorCode) const
{
   if (!PyErr_Occurred())
      return false;
   
   if (!PyErr_ExceptionMatches(pyException))
      return false;

   PyObject* pyErrorType = NULL;
   PyObject* pyErrorMessage = NULL;
   PyObject* pyTraceBack = NULL;
   PyErr_Fetch(&pyErrorType, &pyErrorMessage, &pyTraceBack);

   // First item of the pyErrorMessage tuple is errno
   CPythonBorrowedObject item(PyTuple_GetItem(pyErrorMessage, 0));
   long errCode = PyInt_AsLong(*item);

   PyErr_Restore(pyErrorType, pyErrorMessage, pyTraceBack);

   return errCode == pyErrorCode;
}