#pragma once
#include <shared/communication/Buffer.hpp>

namespace frames {

   class CBinaryFrame
   {
   public:
      CBinaryFrame(boost::shared_ptr<shared::communication::CByteBuffer> content)
         :m_content(content)
      {

      }

      virtual ~CBinaryFrame()
      {

      }

      static const int HeaderSize = 5;

      char m_sourceDest;
      boost::shared_ptr< shared::communication::CByteBuffer > m_content;
   };

} //namespace frames

