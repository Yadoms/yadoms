#include "stdafx.h"
#include "Runner.h"
#include "ScriptFile.h"
#include <shared/Log.h>
#include "PythonException.hpp"
#include "ContextAccessor.h"
#include <shared/DataContainer.h>


CRunner::CRunner(const std::string& scriptPath, const std::string& interpreterPath,
   boost::shared_ptr<IPythonExecutable> executable, const shared::CDataContainer& scriptConfiguration)
   :m_scriptFile(boost::make_shared<CScriptFile>(scriptPath)), m_scriptConfiguration(scriptConfiguration), m_interpreterPath(interpreterPath), m_executable(executable)
{
}

CRunner::~CRunner()
{
   interrupt();
}

void CRunner::run(shared::script::yScriptApi::IYScriptApi& context, boost::shared_ptr<shared::script::ILogger> scriptLogger)
{
   m_lastError.clear();

   try
   {
      scriptLogger->out() << "#### START ####" << std::endl;

      // Embed IYScriptApi instance
      boost::shared_ptr<IContextAccessor> contextAccessor(boost::make_shared<CContextAccessor>(context));

      // Start module
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);
         m_process = m_executable->startModule(m_scriptFile->module(), m_scriptFile->abslouteParentPath(), contextAccessor->id(), scriptLogger);
      }

      int returnCode = m_process->wait();
      if (returnCode != 0)
         throw CPythonException("Script returned with error " + boost::lexical_cast<std::string>(returnCode));

      scriptLogger->out() << "#### END ####" << std::endl;

      YADOMS_LOG(information) << m_scriptFile->pathName() << " : script exited";
   }
   catch(CPythonException& e)
   {
      scriptLogger->error() << m_scriptFile->pathName() << " : error running script, " << e.what();
      scriptLogger->out() << "#### END ####";
      m_lastError = e.what();
   }
}

void CRunner::interrupt()
{
   try
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);
      if (!!m_process)
         Poco::Process::requestTermination(m_process->id());
   }
   catch (Poco::SystemException&)
   {
      // Nothing to do. This exception occurs when process is already stopped
   }
}

bool CRunner::isOk() const
{
   return m_lastError.empty();
}

std::string CRunner::error() const
{
   return m_lastError;
}
