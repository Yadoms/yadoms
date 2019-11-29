#pragma once

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Part of a SmallHeaderMessageCutter message
      //--------------------------------------------------------------
      class IMessageCutterPart
      {
      public:
         virtual ~IMessageCutterPart()
         {
         }

         virtual const unsigned char* formattedMessage() const = 0;

         virtual size_t formattedSize() const = 0;
      };
   }
} // namespace shared::communication


