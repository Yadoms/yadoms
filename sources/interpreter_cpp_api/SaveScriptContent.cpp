#include "stdafx.h"
#include "SaveScriptContent.h"


namespace interpreter_cpp_api
{
   CSaveScriptContent::CSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContent& msg)
      : m_scriptPath(msg.scriptpath()),
        m_scriptContent(msg.content())
   {
   }

   CSaveScriptContent::~CSaveScriptContent()
   {
   }

   const std::string& CSaveScriptContent::getScriptPath()
   {
      return m_scriptPath;
   }

   const std::string& CSaveScriptContent::getScriptContent()
   {
      return m_scriptContent;
   }
} // namespace interpreter_cpp_api	


