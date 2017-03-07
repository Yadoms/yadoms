#pragma once
#include <shared/Export.h>
#include "IBufferLogger.h"

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
         //--------------------------------------------------------------
         explicit CAsciiBufferLogger();

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CAsciiBufferLogger();

         // IBufferLogger implementation
         void logReceived(const CByteBuffer& data) override;
         void logSent(const CByteBuffer& data) override;
         // [END] IBufferLogger implementation

      protected:
         static std::string msgToString(const CByteBuffer& data);
      };
   }
} // namespace shared::communication


