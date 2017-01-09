#pragma once
#include <interpreter_IPC/yadomsToInterpreter.pb.h>
#include <shared/script/yInterpreterApi/IStopScript.h>

namespace interpreter_cpp_api
{
   class CStopScript : public shared::script::yInterpreterApi::IStopScript
   {
   public:
      explicit CStopScript(const interpreter_IPC::toInterpreter::StopScript& msg);
      virtual ~CStopScript();

      // IStopScript Implementation
      int getScriptInstanceId() const override;
      // [END] IStopScript Implementation

   private:
      const int m_scriptInstanceId;
   };
} // namespace interpreter_cpp_api	


