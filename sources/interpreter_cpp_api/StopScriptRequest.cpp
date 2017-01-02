#include "stdafx.h"
#include "StopScriptRequest.h"


namespace interpreter_cpp_api
{
   CStopScriptRequest::CStopScriptRequest(const interpreter_IPC::toInterpreter::StopScriptRequest& msg,
                                          boost::function0<void> sucessCallback,
                                          boost::function1<void, const std::string&> errorCallback)
      : m_scriptProcessId(msg.scriptprocessid()),
        m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }
   
   CStopScriptRequest::~CStopScriptRequest()
   {
   }

   std::string CStopScriptRequest::getScriptProcessId() const
   {
      return m_scriptProcessId;
   }

   void CStopScriptRequest::sendSuccess()
   {
      m_sucessCallback();
   }

   void CStopScriptRequest::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace interpreter_cpp_api	


