#include "stdafx.h"
#include "PurgeScriptLog.h"


namespace interpreter_cpp_api
{
   CPurgeScriptLog::CPurgeScriptLog(const interpreter_IPC::toInterpreter::PurgeScriptLog& msg)
      : m_scriptInstanceId(msg.scriptinstanceid())
   {
   }

   CPurgeScriptLog::~CPurgeScriptLog()
   {
   }

   int CPurgeScriptLog::getScriptInstanceId() const
   {
      return m_scriptInstanceId;
   }
} // namespace interpreter_cpp_api	



