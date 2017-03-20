#pragma once
#include <shared/Export.h>
#include "IMessageCutterPart.h"
#include "SmallHeader.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Part of a SmallHeaderMessageCutter message
      /// \note For performance issues, SmallHeaderMessagePart doesn't have it's own message copy, only pointer
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT SmallHeaderMessageCutterPart : public IMessageCutterPart
      {
      public:
         SmallHeaderMessageCutterPart(unsigned char partNumber,
                                      unsigned char partCount,
                                      const unsigned char* usefulData,
                                      size_t usefulSize);
         virtual ~SmallHeaderMessageCutterPart();

         // IMessageCutterPart Implementation
         const unsigned char* formattedMessage() const override;
         size_t formattedSize() const override;
         // [END] IMessageCutterPart Implementation

         static size_t headerSize();

      private:
         void formatMessage();

         const SmallHeader m_header;
         const unsigned char* m_usefulData;
         const size_t m_usefulSize;
         boost::shared_ptr<unsigned char[]> m_formattedMessage;
         size_t m_formattedSize;
      };
   }
} // namespace shared::communication


