#pragma once
#include <toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>

namespace interpreter_cpp_api
{
   class CLoadScriptContentRequest : public shared::script::yInterpreterApi::ILoadScriptContentRequest
   {
   public:
      CLoadScriptContentRequest(const interpreter_IPC::toInterpreter::LoadScriptContentRequest& msg,
                                boost::function1<void, const std::string&> sucessCallback,
                                boost::function1<void, const std::string&> errorCallback);
      virtual ~CLoadScriptContentRequest();

      // ILoadScriptContentRequest Implementation
      const std::string& getScriptPath() override;
      void sendSuccess(const std::string& content) override;
      void sendError(const std::string& errorMessage) override;
      // [END] ILoadScriptContentRequest Implementation

   private:
      const std::string m_scriptpath;
      boost::function1<void, const std::string&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace interpreter_cpp_api	


