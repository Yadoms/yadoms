#pragma once
#include <shared/script/yInterpreterApi/IStartScript.h>
#include <server/communication/callback/NoDataCallbackRequest.h>

namespace automation
{
   namespace interpreter
   {
      class CStartScript : public shared::script::yInterpreterApi::IStartScript
      {
      public:
         CStartScript(const int scriptInstanceId,
                             const std::string& scriptPath,
                             const std::string& yScriptApiId);
         virtual ~CStartScript();

         // IStartScript implementation
         int getScriptInstanceId() override;
         const std::string& getScriptPath() override;
         const std::string& getScriptApiId() override;
         // [END] - IStartScript implementation

      private:
         const int m_scriptInstanceId;
         const std::string m_scriptPath;
         const std::string m_scriptApiId;
      };
   }
} // namespace automation::interpreter


