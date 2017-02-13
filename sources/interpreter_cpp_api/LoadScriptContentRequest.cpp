#include "stdafx.h"
#include "LoadScriptContentRequest.h"


namespace interpreter_cpp_api
{
   CLoadScriptContentRequest::CLoadScriptContentRequest(const interpreter_IPC::toInterpreter::LoadScriptContentRequest& msg,
                                                        boost::function1<void, const std::string&> sucessCallback,
                                                        boost::function1<void, const std::string&> errorCallback)
      : m_scriptpath(msg.scriptpath()),
        m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }

   CLoadScriptContentRequest::~CLoadScriptContentRequest()
   {
   }

   const std::string& CLoadScriptContentRequest::getScriptPath()
   {
      return m_scriptpath;
   }

   void CLoadScriptContentRequest::sendSuccess(const std::string& content)
   {
      m_sucessCallback(content);
   }

   void CLoadScriptContentRequest::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace interpreter_cpp_api	


