#include "stdafx.h"
#include "AsciiFrame.h"

namespace frames {
namespace incoming {

   CAsciiFrame::CAsciiFrame(char sourceDest, char qualifier1, char qualifier2, const std::string & data)
      :m_sourceDest(sourceDest), m_content(data)
   {
      m_qualifiers[0] = qualifier1;
      m_qualifiers[1] = qualifier2;
   }

   CAsciiFrame::~CAsciiFrame()
   {

   }

   const char CAsciiFrame::getSourceDest() const
   {
      return m_sourceDest;
   }

   const char CAsciiFrame::getQualifier(const int index) const
   {
      if (index == 1)
         return m_qualifiers[1];
      return m_qualifiers[0];
   }

   const std::string & CAsciiFrame::getContent() const
   {
      return m_content;
   }


   void CAsciiFrame::printToLog(std::ostream & out) const
   {

   }

   void CAsciiFrame::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {

   }

} //namespace incoming
} //namespace frames



