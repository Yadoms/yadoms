#pragma once
#include <shared/process/IExternalProcessLogger.h>
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>
#include <shared/logInternal/LogStreamEnhanced.h>
#include <Poco/Logger.h>


// Shortcut to yInterpreterApi namespace
namespace yApi = shared::script::yInterpreterApi;

class CScriptLogger : public shared::process::IExternalProcessLogger //TODO déplacer dans l'aPI ?
{
public:
   CScriptLogger(boost::shared_ptr<yApi::IYInterpreterApi> api,
                 int scriptInstanceId,
                 const std::string& interpreterLoggerName,
                 const boost::filesystem::path& scriptLogPath);
   virtual ~CScriptLogger();

   // IExternalProcessLogger Implementation
   void init() override;
   void information(const std::string& line) override;
   void error(const std::string& line) override;
   // [END] IExternalProcessLogger Implementation

private:
   boost::shared_ptr<yApi::IYInterpreterApi> m_api;
   const int m_scriptInstanceId;//TODO utile ?
   Poco::Logger& m_logger; //TODO réduire la portée ?
   Poco::Message m_msgInformation;
   Poco::Message m_msgError;
};
