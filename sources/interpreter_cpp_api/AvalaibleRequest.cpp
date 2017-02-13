#include "stdafx.h"
#include "AvalaibleRequest.h"


namespace interpreter_cpp_api
{
   CAvalaibleRequest::CAvalaibleRequest(const interpreter_IPC::toInterpreter::AvalaibleRequest& msg,
                                        boost::function1<void, bool> sucessCallback,
                                        boost::function1<void, const std::string&> errorCallback)
      : m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }

   CAvalaibleRequest::~CAvalaibleRequest()
   {
   }

   void CAvalaibleRequest::sendSuccess(bool avalaible)
   {
      m_sucessCallback(avalaible);
   }

   void CAvalaibleRequest::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace interpreter_cpp_api	


