#include "stdafx.h"
#include "DeviceDeletedFrame.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         const std::string CDeviceDeletedFrame::Device = "device";

         CDeviceDeletedFrame::CDeviceDeletedFrame(boost::shared_ptr<const database::entities::CDevice> content)
            : CFrameBase(EFrameType::kDeviceDeleted)
         {
            shared::CDataContainer local;
            local.set(Device, content);
            m_internalContainer.set(DataFieldName, local);
         }
      } //namespace ws
   } //namespace poco
} //namespace web
