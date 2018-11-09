#include "stdafx.h"
#include "DeviceDeletedFrame.h"

namespace web
{
   namespace ws
   {
      const std::string CDeviceDeletedFrame::m_device = "device";

      CDeviceDeletedFrame::CDeviceDeletedFrame(boost::shared_ptr<const database::entities::CDevice> content)
         : CFrameBase(EFrameType::kDeviceDeleted)
      {
         shared::CDataContainer local;
         local.set(m_device, content);
         m_internalContainer.set(m_dataFieldName, local);
      }

      CDeviceDeletedFrame::~CDeviceDeletedFrame()
      {
      }
   } //namespace ws
} //namespace web