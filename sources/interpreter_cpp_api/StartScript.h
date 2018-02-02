#pragma once
#include <interpreter_IPC/yadomsToInterpreter.pb.h>
#include <shared/script/yInterpreterApi/IStartScript.h>

namespace interpreter_cpp_api
{
   class CStartScript : public shared::script::yInterpreterApi::IStartScript
   {
   public:
      explicit CStartScript(const interpreter_IPC::toInterpreter::StartScript& msg);
      virtual ~CStartScript();

      // IStartScript Implementation
      int getScriptInstanceId() const override;
      const boost::filesystem::path& getScriptPath() const override;
      const std::string& getScriptApiId() const override;
      const boost::filesystem::path& getScriptLogPath() const override;
      // [END] IStartScript Implementation

   private:
      const int m_scriptInstanceId;
      const boost::filesystem::path m_scriptPath;
      const std::string m_scriptApiId;
      const boost::filesystem::path m_scriptLogPath;
   };
} // namespace interpreter_cpp_api	


