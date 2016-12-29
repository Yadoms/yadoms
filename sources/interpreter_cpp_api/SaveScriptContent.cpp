#include "stdafx.h"
#include "SaveScriptContent.h"


namespace interpreter_cpp_api
{
   CSaveScriptContent::CSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContent& msg,
                                          boost::function0<void> sucessCallback,
                                          boost::function1<void, const std::string&> errorCallback)
      : m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback),
        m_scriptPath(msg.scriptpath()),
        m_scriptContent(msg.content())
   {
   }

   CSaveScriptContent::~CSaveScriptContent()
   {
   }

   void CSaveScriptContent::sendSuccess()
   {
      m_sucessCallback();
   }

   void CSaveScriptContent::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
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


