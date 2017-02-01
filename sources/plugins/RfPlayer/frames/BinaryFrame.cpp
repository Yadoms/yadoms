#include "stdafx.h"
#include "BinaryFrame.h"

namespace frames {


   CBinaryFrame::CBinaryFrame(char sourceDest, boost::shared_ptr<shared::communication::CByteBuffer> content)
      :m_sourceDest(sourceDest), m_content(content)
   {

   }

   CBinaryFrame::~CBinaryFrame()
   {

   }

   const char CBinaryFrame::getSourceDest() const
   {
      return m_sourceDest;
   }

   boost::shared_ptr< shared::communication::CByteBuffer > CBinaryFrame::getContent() const
   {
      return m_content;
   }



} //namespace frames

