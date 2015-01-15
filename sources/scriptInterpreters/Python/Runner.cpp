#include "stdafx.h"
#include "Runner.h"
#include "PythonLibInclude.h"
#include <shared/Log.h>
#include "RunnerException.hpp"
#include "PythonObject.h"
#include <shared/DataContainer.h>
#include "ScriptLoader.h"
#include "SubInterpreter.h"


// Function name of the Python script entry point
static const std::string ScriptEntryPoint("yMain");

CRunner::CRunner(const std::string& scriptName, const shared::CDataContainer& scriptConfiguration)
   :m_scriptName(scriptName), m_scriptConfiguration(scriptConfiguration)
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
      CScriptLoader loader(m_scriptName);
      loader.load();

      // Run the script
      CPythonObject pyReturnValue(PyObject_CallObject(loader.yMain().get(), NULL));
      if (pyReturnValue.isNull())
         throw CRunnerException("Script exited with error");

      YADOMS_LOG(information) << m_scriptName << " : script exited with code " << PyInt_AsLong(*pyReturnValue);
   }
   catch(CRunnerException& e)
   {
      YADOMS_LOG(error) << m_scriptName << " : error running script, " << e.what();
      m_lastError = e.what();
   }
}

void CRunner::stop()
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
