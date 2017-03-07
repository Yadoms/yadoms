#include "stdafx.h"
#include "SaveScriptContentRequest.h"


namespace interpreter_cpp_api
{
   CSaveScriptContentRequest::CSaveScriptContentRequest(const interpreter_IPC::toInterpreter::SaveScriptContentRequest& msg,
                                          boost::function0<void> sucessCallback,
                                          boost::function1<void, const std::string&> errorCallback)
      : m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback),
        m_scriptPath(msg.scriptpath()),
        m_scriptContent(msg.content())
   {
   }

   CSaveScriptContentRequest::~CSaveScriptContentRequest()
   {
   }

   void CSaveScriptContentRequest::sendSuccess()
   {
      m_sucessCallback();
   }

   void CSaveScriptContentRequest::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }

   const std::string& CSaveScriptContentRequest::getScriptPath()
   {
      return m_scriptPath;
   }

   const std::string& CSaveScriptContentRequest::getScriptContent()
   {
      return m_scriptContent;
   }
} // namespace interpreter_cpp_api	


