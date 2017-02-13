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
                      const boost::filesystem::path& scriptPath,
                      const std::string& yScriptApiId,
                      const boost::filesystem::path& scriptLogPath);
         virtual ~CStartScript();

         // IStartScript implementation
         int getScriptInstanceId() override;
         const boost::filesystem::path& getScriptPath() override;
         const std::string& getScriptApiId() override;
         const boost::filesystem::path& getScriptLogPath() override;
         // [END] - IStartScript implementation

      private:
         const int m_scriptInstanceId;
         const boost::filesystem::path m_scriptPath;
         const std::string m_scriptApiId;
         const boost::filesystem::path m_scriptLogPath;
      };
   }
} // namespace automation::interpreter
