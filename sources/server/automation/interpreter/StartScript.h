#pragma once
#include <shared/script/yInterpreterApi/IStartScript.h>

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
         int getScriptInstanceId() const override;
         const boost::filesystem::path& getScriptPath() const override;
         const std::string& getScriptApiId() const override;
         const boost::filesystem::path& getScriptLogPath() const override;
         // [END] - IStartScript implementation

      private:
         const int m_scriptInstanceId;
         const boost::filesystem::path m_scriptPath;
         const std::string m_scriptApiId;
         const boost::filesystem::path m_scriptLogPath;
      };
   }
} // namespace automation::interpreter
