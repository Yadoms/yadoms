#pragma once
#include "IBufferLogger.h"
#include <Poco/Message.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a buffer logger
      //--------------------------------------------------------------
      class CBufferLogger : public IBufferLogger
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Constructor
         /// \param[in] logLevel Log level to use (string from known log levels)
         //--------------------------------------------------------------
         explicit CBufferLogger(const std::string& logLevel = "information");

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CBufferLogger();

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


