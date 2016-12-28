#include "stdafx.h"
#include "StartScriptRequest.h"


namespace interpreter_cpp_api
{
   CStartScriptRequest::CStartScriptRequest(const interpreter_IPC::toInterpreter::StartScriptRequest& msg,
                                            boost::function1<void, const std::string&> sucessCallback,
                                            boost::function1<void, const std::string&> errorCallback)
      : m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }

   CStartScriptRequest::~CStartScriptRequest()
   {
   }

   void CStartScriptRequest::sendSuccess(const std::string& scriptId)
   {
      m_sucessCallback(scriptId);
   }

   void CStartScriptRequest::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace interpreter_cpp_api	


