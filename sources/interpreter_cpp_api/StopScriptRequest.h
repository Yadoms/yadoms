#pragma once
#include <interpreter_IPC/toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/IStopScriptRequest.h>

namespace interpreter_cpp_api
{
   class CStopScriptRequest : public shared::script::yInterpreterApi::IStopScriptRequest
   {
   public:
      CStopScriptRequest(const interpreter_IPC::toInterpreter::StopScriptRequest& msg,
                         boost::function0<void> sucessCallback,
                         boost::function1<void, const std::string&> errorCallback);
      virtual ~CStopScriptRequest();

      // IStartScriptRequest Implementation
      std::string getScriptId() const override;
      void sendSuccess() override;
      void sendError(const std::string& errorMessage) override;
      // [END] IStartScriptRequest Implementation

   private:
      const std::string m_scriptId;
      boost::function0<void> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace interpreter_cpp_api	


