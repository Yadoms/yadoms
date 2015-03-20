#include "stdafx.h"
#include "AcquisitionFilterFrame.h"

namespace web { namespace ws {

   CAcquisitionFilterFrame::CAcquisitionFilterFrame(const std::string & content)
      :CFrameBase(content)
   {

   }

   CAcquisitionFilterFrame::~CAcquisitionFilterFrame()
   {

   }

   const std::vector<int> CAcquisitionFilterFrame::getFilter()
   { 
      return m_internalContainer.get< std::vector<int> >(m_dataFieldName); 
   }

} //namespace ws
} //namespace web