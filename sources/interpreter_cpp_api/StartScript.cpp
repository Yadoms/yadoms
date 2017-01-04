#include "stdafx.h"
#include "StartScript.h"


namespace interpreter_cpp_api
{
   CStartScript::CStartScript(const interpreter_IPC::toInterpreter::StartScript& msg)
      : m_scriptInstanceId(msg.scriptinstanceid()),
        m_scriptPath(msg.scriptpath()),
        m_scriptApiId(msg.scriptapiid())
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
} // namespace interpreter_cpp_api	


