#pragma once
#include <shared/Export.h>
#include "IBufferLogger.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a buffer logger
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CNoBufferLogger : public IBufferLogger
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Constructor
         //--------------------------------------------------------------
         explicit CNoBufferLogger();

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CNoBufferLogger();

         // IBufferLogger implementation
         void logReceived(const CByteBuffer& data) override;
         void logSent(const CByteBuffer& data) override;
         // [END] IBufferLogger implementation
      };
   }
} // namespace shared::communication


