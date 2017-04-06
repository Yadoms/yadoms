#include "stdafx.h"
#include "BufferLoggerHelper.h"
#include <shared/Log.h>

namespace shared
{
   namespace communication
   {
      void CBufferLoggerHelper::log(Poco::Message::Priority logLevel,
                                    const std::string& str)
      {
         switch (logLevel)
         {
         case Poco::Message::PRIO_FATAL:
            YADOMS_LOG(fatal) << str;
            break;
         case Poco::Message::PRIO_CRITICAL:
            YADOMS_LOG(critical) << str;
            break;
         case Poco::Message::PRIO_ERROR:
            YADOMS_LOG(error) << str;
            break;
         case Poco::Message::PRIO_WARNING:
            YADOMS_LOG(warning) << str;
            break;
         case Poco::Message::PRIO_NOTICE:
            YADOMS_LOG(notice) << str;
            break;
         case Poco::Message::PRIO_INFORMATION:
            YADOMS_LOG(information) << str;
            break;
         case Poco::Message::PRIO_DEBUG:
            YADOMS_LOG(debug) << str;
            break;
         case Poco::Message::PRIO_TRACE:
            YADOMS_LOG(trace) << str;
            break;
         default:
            YADOMS_LOG(information) << str;
            break;
         }
      }

      Poco::Message::Priority CBufferLoggerHelper::fromLoglevelString(const std::string& stringLogLevel)
      {
         if (stringLogLevel == "fatal")
            return Poco::Message::PRIO_FATAL;
         if (stringLogLevel == "critical")
            return Poco::Message::PRIO_CRITICAL;
         if (stringLogLevel == "error")
            return Poco::Message::PRIO_ERROR;
         if (stringLogLevel == "warning")
            return Poco::Message::PRIO_WARNING;
         if (stringLogLevel == "notice")
            return Poco::Message::PRIO_NOTICE;
         if (stringLogLevel == "information")
            return Poco::Message::PRIO_INFORMATION;
         if (stringLogLevel == "debug")
            return Poco::Message::PRIO_DEBUG;
         if (stringLogLevel == "trace")
            return Poco::Message::PRIO_TRACE;

         YADOMS_LOG(warning) << "CAsciiBufferLeveledLogger : unsupported log level " << stringLogLevel << ", will use information level";
         return Poco::Message::PRIO_INFORMATION;
      }
   }
} // namespace shared::communication


