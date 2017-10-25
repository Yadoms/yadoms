#pragma once
#include <shared/script/yInterpreterApi/IPurgeScriptLog.h>

namespace automation
{
   namespace interpreter
   {
      class CPurgeScriptLog : public shared::script::yInterpreterApi::IPurgeScriptLog
      {
      public:
         explicit CPurgeScriptLog(int scriptProcessId);
         virtual ~CPurgeScriptLog();

         // IPurgeScriptLog implementation
         int getScriptInstanceId() const override;
         // [END] - IPurgeScriptLog implementation

      private:
         const int m_scriptProcessId;
      };
   }
} // namespace automation::interpreter
