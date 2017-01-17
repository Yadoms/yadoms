#pragma once
#include <shared/communication/StringBuffer.h>

namespace frames {

   class CAsciiFrame
   {
   public:
      static const int HeaderSize = 5;

      CAsciiFrame(char sourceDest, char qualifier1, char qualifier2, const std::string & data)
         :m_sourceDest(sourceDest), m_content(data)
      {
         m_qualifiers[0] = qualifier1;
         m_qualifiers[1] = qualifier2;
      }

      virtual ~CAsciiFrame()
      {

      }

      const std::string & getContent()
      {
         return m_content;
      }
   private:

      char m_sourceDest;
      char m_qualifiers[2];
      std::string  m_content;
   };

} //namespace frames



