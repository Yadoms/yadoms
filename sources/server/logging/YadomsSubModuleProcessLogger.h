#pragma once
#include <shared/process/IExternalProcessLogger.h>
#include <Poco/Logger.h>

namespace logging
{
   //-----------------------------------------------------
   ///\brief A Yadoms sub-module process logger
   ///\details This logger is used to log process output, when this process
   /// use YADOMS_LOG functionnalities
   //-----------------------------------------------------
   class CYadomsSubModuleProcessLogger : public shared::process::IExternalProcessLogger
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] loggerName   The Logger name
      //-----------------------------------------------------
      explicit CYadomsSubModuleProcessLogger(const std::string& loggerName);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CYadomsSubModuleProcessLogger();

      // ILogger Implementation
      void init() override;
      void information(const std::string& line) override;
      void error(const std::string& line) override;
      void purgeLogFile() override;
      // [END] ILogger Implementation

   protected:
      virtual std::string extractMessage(const std::string& line,
                                         std::string& outMessage) const;
      static void doLog(const std::string& logLevel,
                        const std::string& message);

   private:
      const std::string m_loggerName;
      Poco::Logger& m_logger;
   };
} // namespace logging


