#include "stdafx.h"
#include "PurgeScriptLog.h"

namespace automation
{
   namespace interpreter
   {
      CPurgeScriptLog::CPurgeScriptLog(int scriptProcessId)
         : m_scriptProcessId(scriptProcessId)
      {
      }

      CPurgeScriptLog::~CPurgeScriptLog()
      {
      }

      int CPurgeScriptLog::getScriptInstanceId() const
      {
         return m_scriptProcessId;
      }
   }
} // namespace automation::interpreter
