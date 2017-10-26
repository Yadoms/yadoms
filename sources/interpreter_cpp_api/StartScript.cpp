#include "stdafx.h"
#include "StartScript.h"


namespace interpreter_cpp_api
{
   CStartScript::CStartScript(const interpreter_IPC::toInterpreter::StartScript& msg)
      : m_scriptInstanceId(msg.scriptinstanceid()),
        m_scriptPath(msg.scriptpath()),
        m_scriptApiId(msg.scriptapiid()),
        m_scriptLogPath(msg.scriptlogpath())
   {
   }

   CStartScript::~CStartScript()
   {
   }

   int CStartScript::getScriptInstanceId() const
   {
      return m_scriptInstanceId;
   }

   const boost::filesystem::path& CStartScript::getScriptPath() const
   {
      return m_scriptPath;
   }

   const std::string& CStartScript::getScriptApiId() const
   {
      return m_scriptApiId;
   }

   const boost::filesystem::path& CStartScript::getScriptLogPath() const
   {
      return m_scriptLogPath;
   }
} // namespace interpreter_cpp_api	
