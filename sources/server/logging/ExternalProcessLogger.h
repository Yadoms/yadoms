#pragma once
#include <shared/process/IExternalProcessLogger.h>

namespace logging
{
   //-----------------------------------------------------
   ///\brief A process logger
   ///\details This logger logs normally (using current Yadoms log system)
   /// and into a separated file (append mode), dedicated to the process
   //-----------------------------------------------------
   class CExternalProcessLogger : public shared::process::IExternalProcessLogger
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] loggerName   The Logger name
      ///\param[in] logFilePath  Path to target log file
      //-----------------------------------------------------
      CExternalProcessLogger(const std::string& loggerName,
                             const boost::filesystem::path& logFilePath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExternalProcessLogger();

      // ILogger Implementation
      void init() override;
      void information(const std::string& line) override;
      void error(const std::string& line) override;
      // [END] ILogger Implementation

   private:
      static std::string now();

      const std::string m_loggerName;
      std::ofstream m_logFile;
   };
} // namespace logging


