#include "stdafx.h"
#include "StopScript.h"


namespace interpreter_cpp_api
{
   CStopScript::CStopScript(const interpreter_IPC::toInterpreter::StopScript& msg)
      : m_scriptInstanceId(msg.scriptinstanceid())
   {
   }
   
   CStopScript::~CStopScript()
   {
   }

   int CStopScript::getScriptInstanceId() const
   {
      return m_scriptInstanceId;
   }
} // namespace interpreter_cpp_api	


