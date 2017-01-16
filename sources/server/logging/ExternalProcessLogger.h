#pragma once
#include <shared/process/IExternalProcessLogger.h>
#include <Poco/Logger.h>

namespace logging
{
   //-----------------------------------------------------
   ///\brief A process logger
   ///\details This logger logs normally (using current Yadoms log system)
   /// and into a separated file (append mode), dedicated to the process
   //-----------------------------------------------------
   class CExternalProcessLogger : public shared::process::IExternalProcessLogger //TODO en faire une autre classe (celle-ci est spécifique à un module générant du YADOMS_LOG)
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] loggerName   The Logger name
      //-----------------------------------------------------
      explicit CExternalProcessLogger(const std::string& loggerName);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExternalProcessLogger();

      // ILogger Implementation
      void init() override;
      void information(const std::string& line) override;
      void error(const std::string& line) override;
      // [END] ILogger Implementation

   protected:
      std::string extractMessage(const std::string& line,
                                 std::string& outMessage) const;
      static void doLog(const std::string& logLevel,
                 const std::string& message);

   private:
      const std::string m_loggerName;
      Poco::Logger& m_logger;
   };
} // namespace logging
