#pragma once
#include <shared/Export.h>
#include "Buffer.hpp"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This interface manage a raw buffer logger
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT IBufferLogger
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~IBufferLogger()
         {
         }

         //--------------------------------------------------------------
         /// \brief	            Log buffer as recieved data
         /// \param[in] log      Buffer to log
         //--------------------------------------------------------------
         virtual void logReceived(const CByteBuffer& data) = 0;

         //--------------------------------------------------------------
         /// \brief	            Log buffer as sent data
         /// \param[in] log      Buffer to log
         //--------------------------------------------------------------
         virtual void logSent(const CByteBuffer& data) = 0;
      };
   }
} // namespace shared::communication


