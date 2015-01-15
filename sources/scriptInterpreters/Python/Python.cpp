#include "stdafx.h"
#include "Python.h"
#include "Initializer.h"
#include <shared/script/ImplementationHelper.h>
#include "PythonLibInclude.h"
#include <shared/Log.h>
#include "PythonObject.h"
#include "Runner.h"
#include "RunnerException.hpp"
#include "ScriptLoader.h"
#include "ScriptFile.h"
#include "SubInterpreter.h"

// Declare the script interpreter
IMPLEMENT_SCRIPT_INTERPRETER(CPython)


CPython::CPython()
   :m_initializer(new CInitializer)
{
}

CPython::~CPython()
{
}

std::string CPython::name() const
{
   return "Python";
}

bool CPython::isAvailable() const
{
   // As Python is staticaly linked, Python library is already loaded.
   // Just test it asking its version (and check version number).
   const std::string pythonVersion(Py_GetVersion());
   if (pythonVersion.empty())
      return false;

   static const std::string expectedVersion("2.7");
   size_t versionFoundPosition = pythonVersion.find(expectedVersion);
   if (versionFoundPosition == std::string::npos || versionFoundPosition != 0)
      return false;

   return true;
}

bool CPython::canInterpret(const std::string& scriptName) const
{
   try
   {
      // Create sub-interpreter (needed for thread context)
      CSubInterpreter interpreter;

      CScriptLoader loader(scriptName);
      loader.load();
   }
   catch(CRunnerException& e)
   {
      YADOMS_LOG(error) << scriptName << " : can not interpret, " << e.what();
      return false;
   }

   YADOMS_LOG(information) << scriptName << " can be interpreted";
   return true;
}

std::string CPython::loadScriptContent(const std::string& scriptName) const
{
   CScriptFile file(scriptName);
   return file.read();
}

void CPython::saveScriptTemplate(const std::string& scriptName, const std::string& content) const
{
   CScriptFile file(scriptName);
   return file.write(content);
}

boost::shared_ptr<shared::script::IRunner> CPython::createRunner(const std::string& scriptName, const shared::CDataContainer& scriptConfiguration) const
{
   boost::shared_ptr<shared::script::IRunner> runner(new CRunner(scriptName, scriptConfiguration));
   return runner;
}

//TODO détourner la console