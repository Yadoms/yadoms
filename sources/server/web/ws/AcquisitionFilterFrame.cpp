#include "stdafx.h"
#include "AcquisitionFilterFrame.h"

namespace web
{
   namespace ws
   {
      CAcquisitionFilterFrame::CAcquisitionFilterFrame(const std::string& content)
         : CFrameBase(content)
      {
      }

      CAcquisitionFilterFrame::~CAcquisitionFilterFrame()
      {
      }

      std::vector<int> CAcquisitionFilterFrame::getFilter() const
      {
         return m_internalContainer.get<std::vector<int>>(m_dataFieldName);
      }
   } //namespace ws
} //namespace web


