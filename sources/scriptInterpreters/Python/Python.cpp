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


CPython::CPython(const boost::filesystem::path pythonInterpreterPath)
   :m_pythonInterpreterPath(pythonInterpreterPath),
    m_pythonExecutable(boost::make_shared<CPythonExecutable>())
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
   if (!m_pythonExecutable->found())
      return false;

   // Now check version
   static const std::string expectedVersionString("Python 2.7");
   if (m_pythonExecutable->version().find(expectedVersionString) == std::string::npos)
      return false;

   return true;
}

std::string CPython::loadScriptContent(const std::string& scriptPath) const
{
   if (scriptPath.empty())
      return CScriptFile::PythonFileRead(boost::filesystem::path(m_pythonInterpreterPath.parent_path() / "template.py").string());

   CScriptFile file(scriptPath);
   return file.read();
}

void CPython::saveScriptContent(const std::string& scriptPath, const std::string& content) const
{
   CScriptFile file(scriptPath);
   file.write(content);
}

boost::shared_ptr<shared::process::IProcess> CPython::createProcess(const std::string& scriptPath,
                                                                    const std::string & scriptLogger,
                                                                    boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                                                                    boost::shared_ptr<shared::process::IProcessObserver> processObserver) const
{
   try
   {
      return boost::make_shared<CScriptProcess>(m_pythonExecutable,
                                                m_pythonInterpreterPath,
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
