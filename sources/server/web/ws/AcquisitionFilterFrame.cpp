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
         return m_internalContainer.get<std::vector<int>>(DataFieldName);
      }
   } //namespace ws
} //namespace web


