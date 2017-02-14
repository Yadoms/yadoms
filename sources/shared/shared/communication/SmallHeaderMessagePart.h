#pragma once
#include <shared/Export.h>
#include "IMessagePart.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Part of a SmallHeaderMessageCutter message
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT SmallHeaderMessagePart : public IMessagePart
      {
         struct Header
         {
            explicit Header(unsigned char partNumber,
                            unsigned char partCount)
               : m_partNumber(partNumber),
                 m_partCount(partCount)
            {
            }

            unsigned char m_partNumber;
            unsigned char m_partCount;

            static size_t size()
            {
               return sizeof(m_partNumber) + sizeof(m_partCount);
            }
         };

      public:
         explicit SmallHeaderMessagePart(unsigned char partNumber,
                                         unsigned char partCount,
                                         const unsigned char* usefulData,
                                         size_t usefulSize);

         virtual ~SmallHeaderMessagePart();

         // IMessagePart Implementation
         const unsigned char* formattedMessage() const override;
         size_t formattedSize() const override;
         // [END] IMessagePart Implementation

         static size_t headerSize();

      private:
         void formatMessage();

         const Header m_header;
         const unsigned char* m_usefulData;
         const size_t m_usefulSize;
         boost::shared_ptr<unsigned char[]> m_formattedMessage;
         size_t m_formattedSize;
      };
   }
} // namespace shared::communication


