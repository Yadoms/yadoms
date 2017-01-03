#pragma once
#include <interpreter_IPC/toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/IStartScriptRequest.h>

namespace interpreter_cpp_api
{
   class CStartScriptRequest : public shared::script::yInterpreterApi::IStartScriptRequest
   {
   public:
      CStartScriptRequest(const interpreter_IPC::toInterpreter::StartScriptRequest& msg,
                          boost::function1<void, const std::string&> sucessCallback,
                          boost::function1<void, const std::string&> errorCallback);
      virtual ~CStartScriptRequest();

      // IStartScriptRequest Implementation
      const std::string& getScriptPath() override;
      const std::string& getScriptApiId() override;
      void sendSuccess(const std::string& scriptProcessId) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IStartScriptRequest Implementation

   private:
      const std::string m_scriptPath;
      const std::string m_scriptApiId;
      boost::function1<void, const std::string&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace interpreter_cpp_api	


