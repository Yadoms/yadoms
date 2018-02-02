#include "stdafx.h"
#include "PurgeScriptLog.h"


namespace interpreter_cpp_api
{
   CPurgeScriptLog::CPurgeScriptLog(const interpreter_IPC::toInterpreter::PurgeScriptLog& msg)
      : m_scriptInstanceId(msg.scriptinstanceid()),
        m_scriptLogPath(msg.scriptlogpath())
   {
   }

   CPurgeScriptLog::~CPurgeScriptLog()
   {
   }

   int CPurgeScriptLog::getScriptInstanceId() const
   {
      return m_scriptInstanceId;
   }

   const boost::filesystem::path& CPurgeScriptLog::getScriptLogPath() const
   {
      return m_scriptLogPath;
   }
} // namespace interpreter_cpp_api	



