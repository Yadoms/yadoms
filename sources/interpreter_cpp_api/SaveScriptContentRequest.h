#pragma once
#include <interpreter_IPC/toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>

namespace interpreter_cpp_api
{
   class CSaveScriptContentRequest : public shared::script::yInterpreterApi::ISaveScriptContentRequest
   {
   public:
      CSaveScriptContentRequest(const interpreter_IPC::toInterpreter::SaveScriptContentRequest& msg,
                                boost::function0<void> sucessCallback,
                                boost::function1<void, const std::string&> errorCallback);
      virtual ~CSaveScriptContentRequest();

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


