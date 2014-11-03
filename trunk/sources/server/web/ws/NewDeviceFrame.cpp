#include "stdafx.h"
#include "NewDeviceFrame.h"

namespace web { namespace ws {

   const std::string CNewDeviceFrame::m_device = "device";
   
      
   CNewDeviceFrame::CNewDeviceFrame(boost::shared_ptr<notifications::CNewDeviceNotification> notificationData)
      :CFrameBase(CFrameBase::EFrameType::kDeviceNew)
   {
      shared::CDataContainer local;
      local.set(m_device, notificationData->getDevice());
      m_internalContainer.set(CFrameBase::m_dataFieldName, local);
   }
   
   CNewDeviceFrame::~CNewDeviceFrame()
   {

   }

} //namespace ws
} //namespace web