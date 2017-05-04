#pragma once
#include <shared/Export.h>
#include <Poco/Message.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class is a helper for buffer logger
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CBufferLoggerHelper
      {
      public:

         static void log(Poco::Message::Priority logLevel,
                         const std::string& str);

         static Poco::Message::Priority fromLoglevelString(const std::string& stringLogLevel);
      };
   }
} // namespace shared::communication


