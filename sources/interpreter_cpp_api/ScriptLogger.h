#pragma once
#include <shared/process/IExternalProcessLogger.h>
#include <Poco/Logger.h>


namespace interpreter_cpp_api
{
   class CScriptLogger : public shared::process::IExternalProcessLogger
   {
   public:
      CScriptLogger(int scriptInstanceId,
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
} // namespace interpreter_cpp_api	
