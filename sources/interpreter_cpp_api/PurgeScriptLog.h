#pragma once
#include <interpreter_IPC/yadomsToInterpreter.pb.h>
#include <shared/script/yInterpreterApi/IPurgeScriptLog.h>

namespace interpreter_cpp_api
{
   class CPurgeScriptLog : public shared::script::yInterpreterApi::IPurgeScriptLog
   {
   public:
      explicit CPurgeScriptLog(const interpreter_IPC::toInterpreter::PurgeScriptLog& msg);
      virtual ~CPurgeScriptLog();

      // IPurgeScriptLog Implementation
      int getScriptInstanceId() const override;
      // [END] IPurgeScriptLog Implementation

   private:
      const int m_scriptInstanceId;
   };
} // namespace interpreter_cpp_api	


