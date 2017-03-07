#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/NoDataCallbackRequest.h"
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>

namespace automation
{
   namespace interpreter
   {
      class CAvalaibleRequest : public shared::script::yInterpreterApi::IAvalaibleRequest
      {
      public:
         explicit CAvalaibleRequest(communication::callback::ISynchronousCallback<bool>& callback);
         virtual ~CAvalaibleRequest();

         // IAvalaibleRequest implementation
         void sendSuccess(bool avalaible) override;
         void sendError(const std::string& errorMessage) override;
         // [END] - IAvalaibleRequest implementation

      private:
         boost::shared_ptr<communication::callback::CNoDataCallbackRequest<bool>> m_requestPtr;
      };
   }
} // namespace automation::interpreter


