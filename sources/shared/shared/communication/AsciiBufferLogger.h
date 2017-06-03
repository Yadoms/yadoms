#pragma once
#include <shared/Export.h>
#include "IBufferLogger.h"
#include <Poco/Message.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage an ASCII buffer logger
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CAsciiBufferLogger : public IBufferLogger
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Constructor
         /// \param[in] logLevel Log level to use (string from known log levels)
         //--------------------------------------------------------------
         explicit CAsciiBufferLogger(const std::string& logLevel = "information");

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CAsciiBufferLogger();

         // IBufferLogger implementation
         void logReceived(const CByteBuffer& data) override;
         void logSent(const CByteBuffer& data) override;
         // [END] IBufferLogger implementation

         static std::string msgToString(const CByteBuffer& data);
      protected:
         const Poco::Message::Priority m_logLevel;
      };
   }
} // namespace shared::communication


