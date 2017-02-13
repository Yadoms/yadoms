#include "stdafx.h"
#include "AvalaibleRequest.h"

namespace automation
{
   namespace interpreter
   {
      CAvalaibleRequest::CAvalaibleRequest(communication::callback::ISynchronousCallback<bool>& callback)
         : m_requestPtr(boost::make_shared<communication::callback::CNoDataCallbackRequest<bool>>(callback))
      {
      }

      CAvalaibleRequest::~CAvalaibleRequest()
      {
      }

      void CAvalaibleRequest::sendSuccess(bool avalaible)
      {
         m_requestPtr->sendSuccess(avalaible);
      }

      void CAvalaibleRequest::sendError(const std::string& errorMessage)
      {
         m_requestPtr->sendError(errorMessage);
      }
   }
} // namespace automation::interpreter


