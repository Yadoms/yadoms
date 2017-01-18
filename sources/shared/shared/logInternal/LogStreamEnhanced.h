#pragma once
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include "ILogStream.h"

namespace shared
{
   namespace logInternal
   {
      class CLogStreamEnhanced : public ILogStream
      {
      public:
         explicit CLogStreamEnhanced(Poco::Logger& logger, Poco::Message::Priority priority = Poco::Message::PRIO_INFORMATION);
         explicit CLogStreamEnhanced(const std::string& loggerName, Poco::Message::Priority priority = Poco::Message::PRIO_INFORMATION);
         virtual ~CLogStreamEnhanced();

         std::ostream& fatal() override;
         std::ostream& critical() override;
         std::ostream& error() override;
         std::ostream& warning() override;
         std::ostream& notice() override;
         std::ostream& information() override;
         std::ostream& debug() override;
         std::ostream& trace() override;

      private:
         std::ostream& stream(Poco::Message::Priority priority);

         Poco::Logger& m_internalLogger;
         Poco::Message::Priority m_currentPriority;
         std::stringstream m_buffer;
      };
   } // namespace logInternal
} // namespace shared
