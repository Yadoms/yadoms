#include "stdafx.h"
#include "PurgeScriptLog.h"

namespace automation
{
   namespace interpreter
   {
      CPurgeScriptLog::CPurgeScriptLog(int scriptProcessId,
                                       const boost::filesystem::path& scriptLogPath)
         : m_scriptProcessId(scriptProcessId),
           m_scriptLogPath(scriptLogPath)
      {
      }

      CPurgeScriptLog::~CPurgeScriptLog()
      {
      }

      int CPurgeScriptLog::getScriptInstanceId() const
      {
         return m_scriptProcessId;
      }

      const boost::filesystem::path& CPurgeScriptLog::getScriptLogPath() const
      {
         return m_scriptLogPath;
      }
   }
} // namespace automation::interpreter


