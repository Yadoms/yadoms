#pragma once
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include "ILogStream.h"

namespace shared {
   namespace logInternal {

      class CLogStreamEnhanced : public ILogStream
      {
      public:
         CLogStreamEnhanced(Poco::Logger& logger, Poco::Message::Priority priority = Poco::Message::PRIO_INFORMATION);
         CLogStreamEnhanced(const std::string& loggerName, Poco::Message::Priority priority = Poco::Message::PRIO_INFORMATION);
         virtual ~CLogStreamEnhanced();

         virtual std::ostream& fatal();
         virtual std::ostream& critical();
         virtual std::ostream& error();
         virtual std::ostream& warning();
         virtual std::ostream& notice();
         virtual std::ostream& information();
         virtual std::ostream& debug();
         virtual std::ostream& trace();

      private:
         std::ostream& stream(Poco::Message::Priority priority);

         Poco::Logger & m_internalLogger;
         Poco::Message::Priority m_currentPriority;
         std::stringstream m_buffer;
      };

   } // namespace logInternal
} // namespace shared
