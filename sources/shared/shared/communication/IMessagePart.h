#pragma once
#include <shared/Export.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Part of a SmallHeaderMessageCutter message
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT IMessagePart
      {
      public:
         virtual ~IMessagePart()
         {
         }

         virtual const unsigned char* formattedMessage() const = 0;

         virtual size_t formattedSize() const = 0;
      };
   }
} // namespace shared::communication


