#include "stdafx.h"
#include "StopScript.h"

namespace automation
{
   namespace interpreter
   {
      CStopScript::CStopScript(int scriptProcessId)
         : m_scriptProcessId(scriptProcessId)
      {
      }

      CStopScript::~CStopScript()
      {
      }

      int CStopScript::getScriptInstanceId() const
      {
         return m_scriptProcessId;
      }
   }
} // namespace automation::interpreter
