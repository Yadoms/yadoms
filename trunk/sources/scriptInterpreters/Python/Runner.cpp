#include "stdafx.h"
#include "Runner.h"
#include "PythonLibInclude.h"
#include <shared/Log.h>
#include "RunnerException.hpp"
#include "PythonObject.h"
#include "PythonBorrowedObject.h"
#include <shared/DataContainer.h>
#include "ScriptLoader.h"
#include "SubInterpreter.h"
#include "swigpyrun.h"  // Generated file (at pre-build-step)


CRunner::CRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration)
   :m_scriptPath(scriptPath), m_scriptConfiguration(scriptConfiguration)
{
}

CRunner::~CRunner()
{
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
      m_isStopping = false;
      CPythonObject pyContext(SWIG_NewPointerObj(&context, SWIG_TypeQuery("shared::script::yScriptApi::IYScriptApi *"), 0));
      CPythonObject tuple(PyTuple_New(1));
      Py_XINCREF(*pyContext); // Increment pyContext reference count, because PyTuple_SetItem steals the reference
      if (PyTuple_SetItem(*tuple, 0, *pyContext))
         PyErr_Print();
      CPythonObject ymainFunction(PyObject_GetAttrString(loader.module().get(), "yMain"));
      if (ymainFunction.isNull() || PyCallable_Check(*ymainFunction) == 0)
         throw CRunnerException("Script exited with error");
      CPythonObject pyReturnValue2(PyObject_CallObject(*ymainFunction, *tuple));
      if (pyReturnValue2.isNull())
         PyErr_Print();

      YADOMS_LOG(information) << m_scriptPath << " : script exited";
   }
   catch(CRunnerException& e)
   {
      YADOMS_LOG(error) << m_scriptPath << " : error running script, " << e.what();
      m_lastError = e.what();
   }
}

void CRunner::stop()
{
   m_isStopping = true;
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
