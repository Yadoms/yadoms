#include "stdafx.h"
#include "StartScriptRequest.h"

namespace automation
{
   namespace interpreter
   {
      CStartScriptRequest::CStartScriptRequest(const std::string& scriptPath,
                                               communication::callback::ISynchronousCallback<std::string>& callback)
         : m_scriptPath(scriptPath),
           m_requestPtr(boost::make_shared<communication::callback::CNoDataCallbackRequest<std::string>>(callback))
      {
      }

      CStartScriptRequest::~CStartScriptRequest()
      {
      }

      const std::string& CStartScriptRequest::getScriptPath()
      {
         return m_scriptPath;
      }

      void CStartScriptRequest::sendSuccess(const std::string& scriptProcessId)
      {
         m_requestPtr->sendSuccess(scriptProcessId);
      }

      void CStartScriptRequest::sendError(const std::string& errorMessage)
      {
         m_requestPtr->sendError(errorMessage);
      }
   }
} // namespace automation::interpreter


