#include "stdafx.h"
#include "Python.h"
#include "Initializer.h"
#include <shared/script/ImplementationHelper.h>
#include "PythonLibInclude.h"
#include <shared/Log.h>
#include "PythonObject.h"
#include "Runner.h"
#include "PythonException.hpp"
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
   static const std::string interpreterName("Python");
   return interpreterName;
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

std::string CPython::loadScriptContent(const std::string& scriptPath) const
{
   CScriptFile file(scriptPath);
   return file.read();
}

void CPython::saveScriptContent(const std::string& scriptPath, const std::string& content) const
{
   CScriptFile file(scriptPath);
   file.write(content);
}

boost::shared_ptr<shared::script::IRunner> CPython::createRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration, const std::string& interpreterPath) const
{
   boost::shared_ptr<shared::script::IRunner> runner(new CRunner(scriptPath, interpreterPath, scriptConfiguration));
   return runner;
}
