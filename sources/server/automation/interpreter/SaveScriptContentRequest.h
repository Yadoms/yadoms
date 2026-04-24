#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>
#include "communication/callback/NoDataCallbackRequest.h"

namespace automation
{
   namespace interpreter
   {
      class CSaveScriptContentRequest final : public shared::script::yInterpreterApi::ISaveScriptContentRequest
      {
      public:
         CSaveScriptContentRequest(std::string scriptPath,
                                   std::string scriptContent,
                                   communication::callback::ISynchronousCallback<bool>& callback);
         ~CSaveScriptContentRequest() override = default;

         // ILoadScriptContentRequest implementation
         const std::string& getScriptPath() override;
         const std::string& getScriptContent() override;
         void sendSuccess() override;
         void sendError(const std::string& errorMessage) override;
         // [END] - ILoadScriptContentRequest implementation

      private:
         const std::string m_scriptPath;
         const std::string m_scriptContent;
         boost::shared_ptr<communication::callback::CNoDataCallbackRequest<bool>> m_requestPtr;
      };
   }
} // namespace automation::interpreter


