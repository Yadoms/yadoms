#include "stdafx.h"
#include "SaveScriptContentRequest.h"

namespace automation
{
   namespace interpreter
   {
      CSaveScriptContentRequest::CSaveScriptContentRequest(const std::string& scriptPath,
                                                           const std::string& scriptContent,
                                                           communication::callback::ISynchronousCallback<bool>& callback)
         : m_scriptPath(scriptPath),
           m_scriptContent(scriptContent),
           m_requestPtr(boost::make_shared<communication::callback::CNoDataCallbackRequest<bool>>(callback))
      {
      }

      CSaveScriptContentRequest::~CSaveScriptContentRequest()
      {
      }

      const std::string& CSaveScriptContentRequest::getScriptPath()
      {
         return m_scriptPath;
      }

      const std::string& CSaveScriptContentRequest::getScriptContent()
      {
         return m_scriptContent;
      }

      void CSaveScriptContentRequest::sendSuccess()
      {
         m_requestPtr->sendSuccess(true);
      }

      void CSaveScriptContentRequest::sendError(const std::string& errorMessage)
      {
         m_requestPtr->sendError(errorMessage);
      }
   }
} // namespace automation::interpreter


