#pragma once
#include "ILogger.h"

namespace shared
{
   namespace process
   {
      //-----------------------------------------------------
      ///\brief A process logger
      ///\details This logger logs normally (using current Yadoms log system)
      /// and into a separated file (append mode), dedicated to the process
      //-----------------------------------------------------
      class YADOMS_SHARED_EXPORT CLogger : public ILogger
      {
      public:
         //-----------------------------------------------------
         ///\brief                  Constructor
         ///\param[in] loggerName   The Logger name
         ///\param[in] logFilePath  Path to target log file
         //-----------------------------------------------------
         CLogger(const std::string& loggerName,
                 const boost::filesystem::path& logFilePath);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CLogger();

         // ILogger Implementation
         void init() override;
         void information(const std::string& line) override;
         void error(const std::string& line) override;
         // [END] ILogger Implementation

      private:
         static std::string now();
         static std::string removeEol(const std::string& line);

         const std::string m_loggerName;
         std::ofstream m_logFile;
      };
   }
} // namespace shared::process


