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
   scriptLogger->out() << "#### START ####" << std::endl;

   // Embed IYScriptApi instance
   boost::shared_ptr<IContextAccessor> contextAccessor(boost::make_shared<CContextAccessor>(context));

   try
   {
      // Start module
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);
         m_process = m_executable->startModule(m_scriptFile, contextAccessor->id(), scriptLogger);
      }

      int returnCode = m_process->wait();
      if (returnCode != 0 && isError(returnCode))
         throw CPythonException("Script returned with error " + boost::lexical_cast<std::string>(returnCode));
   }
   catch(CPythonException& e)
   {
      scriptLogger->error() << m_scriptFile->pathName() << " : error running script, " << e.what();
      m_lastError = e.what();
   }

   scriptLogger->out() << "#### END ####";

   // Needed to flush streams and don't loose any log
   scriptLogger->out() << std::endl;
   scriptLogger->error() << std::endl;

   YADOMS_LOG(information) << m_scriptFile->pathName() << " : script exited";

   boost::this_thread::sleep(boost::posix_time::seconds(1));
}

bool CRunner::isError(int code) const
{
#if defined WIN32
   if (static_cast<DWORD>(code) == STATUS_CONTROL_C_EXIT)
      return false;
#endif
   return true;
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
