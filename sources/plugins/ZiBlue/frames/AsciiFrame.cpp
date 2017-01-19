#include "stdafx.h"
#include "AsciiFrame.h"

namespace frames {

   CAsciiFrame::CAsciiFrame(char sourceDest, char qualifier1, char qualifier2, const std::string & data)
      :m_sourceDest(sourceDest), m_content(data)
   {
      m_qualifiers[0] = qualifier1;
      m_qualifiers[1] = qualifier2;
   }

   CAsciiFrame::~CAsciiFrame()
   {

   }

   const std::string & CAsciiFrame::getContent() const
   {
      return m_content;
   }


} //namespace frames



