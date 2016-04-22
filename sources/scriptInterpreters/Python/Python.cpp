#include "stdafx.h"
#include "Python.h"
#include <shared/script/ImplementationHelper.h>
#include <shared/Log.h>
#include "PythonExecutable.h"
#include "ScriptFile.h"
#include "ScriptProcess.h"
#include <shared/process/ProcessException.hpp>

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

boost::shared_ptr<shared::process::IProcess> CPython::createProcess(const std::string& scriptPath,
                                                                    boost::shared_ptr<shared::process::ILogger> scriptLogger,
                                                                    boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                                                                    boost::shared_ptr<shared::process::IProcessObserver> processObserver,
                                                                    const shared::CDataContainer& scriptConfiguration) const
{
   try
   {
      return boost::make_shared<CScriptProcess>(m_executable,
                                                boost::make_shared<CScriptFile>(scriptPath),
                                                yScriptApi,
                                                scriptLogger,
                                                processObserver);
   }
   catch (shared::process::CProcessException& ex)
   {
      YADOMS_LOG(error) << "Unable to create the Python process, " << ex.what();
      return boost::shared_ptr<shared::process::IProcess>();
   }
}
