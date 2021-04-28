#pragma once
#include <shared/process/IExternalProcessLogger.h>

class CSpeedTestProcessLogger final : public shared::process::IExternalProcessLogger
{
public:
   explicit CSpeedTestProcessLogger(const std::string& logPrefix);
   ~CSpeedTestProcessLogger() = default;

   // IExternalProcessLogger Implementation
   void init() override;
   void information(const std::string& line) override;
   void error(const std::string& line) override;
   void purgeLogFile() override;
   // [END] IExternalProcessLogger Implementation

   const std::string& stdoutContent() const;

private:
   const std::string m_logPrefix;
   std::string m_stdoutContent;
};
