#include "stdafx.h"
#include "LogStreamEnhanced.h"
#include "../Log.h"

namespace shared
{
   namespace logInternal
   {
      CLogStreamEnhanced::CLogStreamEnhanced(Poco::Logger& logger, Poco::Message::Priority priority)
         : m_internalLogger(logger), m_currentPriority(priority)
      {
      }

      CLogStreamEnhanced::CLogStreamEnhanced(const std::string& loggerName, Poco::Message::Priority priority)
         : m_internalLogger(Poco::Logger::get(loggerName)), m_currentPriority(priority)
      {
      }

      CLogStreamEnhanced::~CLogStreamEnhanced()
      {
         //check logger priority
         //recommended to create Message only for propagated message (performance issue)
         if (m_internalLogger.is(m_currentPriority))
         {
            Poco::Message msg;
            msg.setText(m_buffer.str());
            msg.setPriority(m_currentPriority);
            msg.setThread(CLog::getCurrentThreadName());
            m_internalLogger.log(msg);
         }
      }

      std::ostream& CLogStreamEnhanced::stream(Poco::Message::Priority priority)
      {
         m_currentPriority = priority;
         return m_buffer;
      }

      std::ostream& CLogStreamEnhanced::fatal() { return stream(Poco::Message::PRIO_FATAL); }
      std::ostream& CLogStreamEnhanced::critical() { return stream(Poco::Message::PRIO_CRITICAL); }
      std::ostream& CLogStreamEnhanced::error() { return stream(Poco::Message::PRIO_ERROR); }
      std::ostream& CLogStreamEnhanced::warning() { return stream(Poco::Message::PRIO_WARNING); }
      std::ostream& CLogStreamEnhanced::notice() { return stream(Poco::Message::PRIO_NOTICE); }
      std::ostream& CLogStreamEnhanced::information() { return stream(Poco::Message::PRIO_INFORMATION); }
      std::ostream& CLogStreamEnhanced::debug() { return stream(Poco::Message::PRIO_DEBUG); }
      std::ostream& CLogStreamEnhanced::trace() { return stream(Poco::Message::PRIO_TRACE); }
   } // namespace logInternal
} // namespace shared
