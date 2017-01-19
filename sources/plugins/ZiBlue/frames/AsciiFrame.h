#pragma once

namespace frames {

   class CAsciiFrame
   {
   public:
      static const int HeaderSize = 5;

      CAsciiFrame(char sourceDest, char qualifier1, char qualifier2, const std::string & data);
      virtual ~CAsciiFrame();
      const std::string & getContent() const;
   private:

      char m_sourceDest;
      char m_qualifiers[2];
      std::string  m_content;
   };

} //namespace frames



