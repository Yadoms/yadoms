#pragma once
#include <shared/process/IExternalProcessLogger.h>
#include <Poco/Logger.h>


class CScriptLogger : public shared::process::IExternalProcessLogger //TODO déplacer dans l'aPI ?
{
public:
   CScriptLogger(const std::string& interpreterLoggerName,
                 int scriptInstanceId,
                 const boost::filesystem::path& scriptLogPath);
   virtual ~CScriptLogger();

   // IExternalProcessLogger Implementation
   void init() override;
   void information(const std::string& line) override;
   void error(const std::string& line) override;
   // [END] IExternalProcessLogger Implementation

private:
   Poco::Logger& m_logger;
   Poco::Message m_msgInformation;
   Poco::Message m_msgError;
};
