#pragma once
#include <toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/ISaveScriptContent.h>

namespace interpreter_cpp_api
{
   class CSaveScriptContent : public shared::script::yInterpreterApi::ISaveScriptContent
   {
   public:
      CSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContent& msg,
                         boost::function0<void> sucessCallback,
                         boost::function1<void, const std::string&> errorCallback);
      virtual ~CSaveScriptContent();

      // ILoadScriptContentRequest Implementation
      void sendSuccess() override;
      void sendError(const std::string& errorMessage) override;
      const std::string& getScriptPath() override;
      const std::string& getScriptContent() override;
      // [END] ILoadScriptContentRequest Implementation

   private:
      boost::function0<void> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
      const std::string m_scriptPath;
      const std::string m_scriptContent;
   };
} // namespace interpreter_cpp_api	


