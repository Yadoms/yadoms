#include "stdafx.h"
#include "Python.h"
#include <shared/script/ImplementationHelper.h>
#include <shared/Log.h>
#include "Runner.h"
#include "PythonException.hpp"
#include "PythonExecutable.h"
#include "ScriptFile.h"

// Declare the script interpreter
IMPLEMENT_SCRIPT_INTERPRETER(CPython)


CPython::CPython()
   :m_executable(boost::make_shared<CPythonExecutable>())
{
}

CPython::~CPython()
{
}

std::string CPython::type() const
{
   static const std::string interpreterName("python");
   return interpreterName;
}

std::string CPython::name() const
{
   static const std::string interpreterName("Python");
   return interpreterName;
}

bool CPython::isAvailable() const
{
   if (!m_executable->found())
      return false;

   // Now check version
   static const std::string expectedVersionString("Python 2.7");
   if (m_executable->version().find(expectedVersionString) == std::string::npos)
      return false;

   return true;
}

std::string CPython::loadScriptContent(const std::string& scriptPath) const
{
   if (scriptPath.empty())
      return CScriptFile::PythonFileRead("scriptInterpreters/python/template.py");

   CScriptFile file(scriptPath);
   return file.read();
}

void CPython::saveScriptContent(const std::string& scriptPath, const std::string& content) const
{
   CScriptFile file(scriptPath);
   file.write(content);
}

boost::shared_ptr<shared::script::IRunner> CPython::createRunner(
   const std::string& scriptPath,
   boost::shared_ptr<shared::script::ILogger> scriptLogger,
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
   boost::shared_ptr<shared::script::IStopNotifier> stopNotifier,
   const shared::CDataContainer& scriptConfiguration) const
{
   try
   {
      boost::shared_ptr<shared::script::IRunner> runner(boost::make_shared<CRunner>(scriptPath, m_executable, scriptLogger, yScriptApi, stopNotifier, scriptConfiguration));
      return runner;
   }
   catch (CPythonException& ex)
   {
      YADOMS_LOG(error) << "Unable to create the Python runner object, " << ex.what();
      return boost::shared_ptr<shared::script::IRunner>();
   }
}
