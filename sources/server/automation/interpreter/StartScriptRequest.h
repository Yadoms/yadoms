#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include <shared/script/yInterpreterApi/IStartScriptRequest.h>
#include <server/communication/callback/NoDataCallbackRequest.h>

namespace automation
{
   namespace interpreter
   {
      class CStartScriptRequest : public shared::script::yInterpreterApi::IStartScriptRequest
      {
      public:
         CStartScriptRequest(const std::string& scriptPath,
                             communication::callback::ISynchronousCallback<std::string>& callback);
         virtual ~CStartScriptRequest();

         // IStartScriptRequest implementation
         const std::string& getScriptPath() override;
         void sendSuccess(const std::string& scriptProcessId) override;
         void sendError(const std::string& errorMessage) override;
         // [END] - IStartScriptRequest implementation

      private:
         const std::string m_scriptPath;
         boost::shared_ptr<communication::callback::CNoDataCallbackRequest<std::string>> m_requestPtr;
      };
   }
} // namespace automation::interpreter


