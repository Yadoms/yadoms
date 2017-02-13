#include "stdafx.h"
#include "StartScript.h"

namespace automation
{
   namespace interpreter
   {
      CStartScript::CStartScript(const int scriptInstanceId,
                                 const boost::filesystem::path& scriptPath,
                                 const std::string& yScriptApiId,
                                 const boost::filesystem::path& scriptLogPath)
         : m_scriptInstanceId(scriptInstanceId),
           m_scriptPath(scriptPath),
           m_scriptApiId(yScriptApiId),
           m_scriptLogPath(scriptLogPath)
      {
      }

      CStartScript::~CStartScript()
      {
      }

      int CStartScript::getScriptInstanceId()
      {
         return m_scriptInstanceId;
      }

      const boost::filesystem::path& CStartScript::getScriptPath()
      {
         return m_scriptPath;
      }

      const std::string& CStartScript::getScriptApiId()
      {
         return m_scriptApiId;
      }

      const boost::filesystem::path& CStartScript::getScriptLogPath()
      {
         return m_scriptLogPath;
      }
   }
} // namespace automation::interpreter
