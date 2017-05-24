#pragma once
#include <shared/script/yInterpreterApi/IStopScript.h>

namespace automation
{
   namespace interpreter
   {
      class CStopScript : public shared::script::yInterpreterApi::IStopScript
      {
      public:
         explicit CStopScript(int scriptProcessId);
         virtual ~CStopScript();

         // IStopScript implementation
         int getScriptInstanceId() const override;
         // [END] - IStopScript implementation

      private:
         const int m_scriptProcessId;
      };
   }
} // namespace automation::interpreter
