#pragma once
#include <shared/script/yInterpreterApi/IPurgeScriptLog.h>

namespace automation
{
   namespace interpreter
   {
      class CPurgeScriptLog : public shared::script::yInterpreterApi::IPurgeScriptLog
      {
      public:
         CPurgeScriptLog(int scriptProcessId,
                         const boost::filesystem::path& scriptLogPath);
         virtual ~CPurgeScriptLog();

         // IPurgeScriptLog implementation
         int getScriptInstanceId() const override;
         const boost::filesystem::path& getScriptLogPath() const override;
         // [END] - IPurgeScriptLog implementation

      private:
         const int m_scriptProcessId;
         const boost::filesystem::path& m_scriptLogPath;
      };
   }
} // namespace automation::interpreter


