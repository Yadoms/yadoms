#pragma once
#include <toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/ISaveScriptContent.h>

namespace interpreter_cpp_api
{
   class CSaveScriptContent : public shared::script::yInterpreterApi::ISaveScriptContent
   {
   public:
      CSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContent& msg);
      virtual ~CSaveScriptContent();

      // ILoadScriptContentRequest Implementation
      const std::string& getScriptPath() override;
      const std::string& getScriptContent() override;
      // [END] ILoadScriptContentRequest Implementation

   private:
      const std::string m_scriptPath;
      const std::string m_scriptContent;
   };
} // namespace interpreter_cpp_api	


