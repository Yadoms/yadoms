#include "stdafx.h"
#include "StopScriptRequest.h"

namespace automation
{
   namespace interpreter
   {
      CStopScriptRequest::CStopScriptRequest(const std::string& scriptProcessId,
                                             communication::callback::ISynchronousCallback<bool>& callback)
         : m_scriptProcessId(scriptProcessId),
           m_requestPtr(boost::make_shared<communication::callback::CNoDataCallbackRequest<bool>>(callback))
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
         m_requestPtr->sendSuccess(true);
      }

      void CStopScriptRequest::sendError(const std::string& errorMessage)
      {
         m_requestPtr->sendError(errorMessage);
      }
   }
} // namespace automation::interpreter


