#include "stdafx.h"
#include "StartScript.h"

namespace automation
{
   namespace interpreter
   {
      CStartScript::CStartScript(const int scriptInstanceId,
                                               const std::string& scriptPath,
                                               const std::string& yScriptApiId)
         : m_scriptInstanceId(scriptInstanceId),
           m_scriptPath(scriptPath),
           m_scriptApiId(yScriptApiId)
      {
      }

      CStartScript::~CStartScript()
      {
      }

      int CStartScript::getScriptInstanceId()
      {
         return m_scriptInstanceId;
      }

      const std::string& CStartScript::getScriptPath()
      {
         return m_scriptPath;
      }

      const std::string& CStartScript::getScriptApiId()
      {
         return m_scriptApiId;
      }
   }
} // namespace automation::interpreter


