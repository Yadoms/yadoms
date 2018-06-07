#include "stdafx.h"
#include "DeviceBlackListedFrame.h"

namespace web
{
   namespace ws
   {
      const std::string CDeviceBlackListedFrame::m_device = "device";

      CDeviceBlackListedFrame::CDeviceBlackListedFrame(boost::shared_ptr<const database::entities::CDevice> content)
         : CFrameBase(EFrameType::kDeviceBlackListed)
      {
         shared::CDataContainer local;
         local.set(m_device, content);
         m_internalContainer.set(m_dataFieldName, local);
      }

      CDeviceBlackListedFrame::~CDeviceBlackListedFrame()
      {
      }
   } //namespace ws
} //namespace web