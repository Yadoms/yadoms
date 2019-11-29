#pragma once
#include "SmallHeader.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Part of a SmallHeaderMessageCutter message
      /// \note For performance issues, SmallHeaderMessagePart doesn't have it's own message copy, only pointer
      //--------------------------------------------------------------
      class SmallHeaderMessageAssemblerPart
      {
      public:
         SmallHeaderMessageAssemblerPart(boost::shared_ptr<const unsigned char[]> message,
                                         size_t messageSize);
         virtual ~SmallHeaderMessageAssemblerPart();

         static size_t headerSize();
         bool isFirst() const;
         bool isLast() const;
         size_t partCount() const;
         const unsigned char* usefulMessage() const;
         size_t usefulSize() const;

      private:
         SmallHeader m_header;
         const unsigned char* m_usefulData;
         size_t m_usefulSize;
      };
   }
} // namespace shared::communication


