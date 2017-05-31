#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>
#include "communication/callback/NoDataCallbackRequest.h"

namespace automation
{
   namespace interpreter
   {
      class CLoadScriptContentRequest : public shared::script::yInterpreterApi::ILoadScriptContentRequest
      {
      public:
         CLoadScriptContentRequest(const std::string& scriptPath,
                                   communication::callback::ISynchronousCallback<std::string>& callback);
         virtual ~CLoadScriptContentRequest();

         // ILoadScriptContentRequest implementation
         const std::string& getScriptPath() override;
         void sendSuccess(const std::string& content) override;
         void sendError(const std::string& errorMessage) override;
         // [END] - ILoadScriptContentRequest implementation

      private:
         const std::string m_scriptPath;
         boost::shared_ptr<communication::callback::CNoDataCallbackRequest<std::string>> m_requestPtr;
      };
   }
} // namespace automation::interpreter


