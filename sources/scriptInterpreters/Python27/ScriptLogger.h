#pragma once
#include <shared/process/IExternalProcessLogger.h>

class CScriptLogger : public shared::process::IExternalProcessLogger
{
public:
   explicit CScriptLogger(int scriptInstanceId);
   virtual ~CScriptLogger();

   // IExternalProcessLogger Implementation
   void init() override;
   void information(const std::string& line) override;
   void error(const std::string& line) override;
   // [END] IExternalProcessLogger Implementation

private:
   const std::string& m_logPrefix;
};

