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
      int getScriptInstanceId() override;
      const std::string& getScriptPath() override;
      const std::string& getScriptApiId() override;
      // [END] IStartScript Implementation

   private:
      const int m_scriptInstanceId;
      const std::string m_scriptPath;
      const std::string m_scriptApiId;
   };
} // namespace interpreter_cpp_api	


