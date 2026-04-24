#include "stdafx.h"
#include "SaveScriptContentRequest.h"

#include <utility>

namespace automation
{
   namespace interpreter
   {
      CSaveScriptContentRequest::CSaveScriptContentRequest(std::string scriptPath,
                                                           std::string scriptContent,
                                                           communication::callback::ISynchronousCallback<bool>& callback)
         : m_scriptPath(std::move(scriptPath)),
           m_scriptContent(std::move(scriptContent)),
           m_requestPtr(boost::make_shared<communication::callback::CNoDataCallbackRequest<bool>>(callback))
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


