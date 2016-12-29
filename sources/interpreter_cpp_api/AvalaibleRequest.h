#pragma once
#include <interpreter_IPC/toInterpreter.pb.h>
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>

namespace interpreter_cpp_api
{
   class CAvalaibleRequest : public shared::script::yInterpreterApi::IAvalaibleRequest
   {
   public:
      CAvalaibleRequest(const interpreter_IPC::toInterpreter::AvalaibleRequest& msg,
                        boost::function1<void, bool> sucessCallback,
                        boost::function1<void, const std::string&> errorCallback);
      virtual ~CAvalaibleRequest();

      // IAvalaibleRequest Implementation
      void sendSuccess(bool avalaible) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IAvalaibleRequest Implementation

   private:
      boost::function1<void, bool> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace interpreter_cpp_api	


