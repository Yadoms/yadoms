#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include <shared/script/yInterpreterApi/IStopScriptRequest.h>
#include <server/communication/callback/NoDataCallbackRequest.h>

namespace automation
{
   namespace interpreter
   {
      class CStopScriptRequest : public shared::script::yInterpreterApi::IStopScriptRequest
      {
      public:
         CStopScriptRequest(const std::string& scriptProcessId,
                            communication::callback::ISynchronousCallback<bool>& callback);
         virtual ~CStopScriptRequest();

         // IStopScriptRequest implementation
         std::string getScriptProcessId() const override;
         void sendSuccess() override;
         void sendError(const std::string& errorMessage) override;
         // [END] - IStopScriptRequest implementation

      private:
         const std::string m_scriptProcessId;
         boost::shared_ptr<communication::callback::CNoDataCallbackRequest<bool>> m_requestPtr;
      };
   }
} // namespace automation::interpreter


