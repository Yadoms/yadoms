#include "stdafx.h"
#include "LoadScriptContentRequest.h"

namespace automation
{
   namespace interpreter
   {
      CLoadScriptContentRequest::CLoadScriptContentRequest(const std::string& scriptPath,
                                                           communication::callback::ISynchronousCallback<std::string>& callback)
         : m_scriptPath(scriptPath),
           m_requestPtr(boost::make_shared<communication::callback::CNoDataCallbackRequest<std::string>>(callback))
      {
      }

      CLoadScriptContentRequest::~CLoadScriptContentRequest()
      {
      }

      const std::string& CLoadScriptContentRequest::getScriptPath()
      {
         return m_scriptPath;
      }

      void CLoadScriptContentRequest::sendSuccess(const std::string& content)
      {
         m_requestPtr->sendSuccess(content);
      }

      void CLoadScriptContentRequest::sendError(const std::string& errorMessage)
      {
         m_requestPtr->sendError(errorMessage);
      }
   }
} // namespace automation::interpreter


