#include "stdafx.h"
#include "NewDeviceFrame.h"

namespace web
{
   namespace ws
   {
      const std::string CNewDeviceFrame::m_device = "device";

      CNewDeviceFrame::CNewDeviceFrame(boost::shared_ptr<const database::entities::CDevice> content)
         : CFrameBase(EFrameType::kDeviceNew)
      {
         shared::CDataContainer local;
         local.set(m_device, content);
         m_internalContainer.set(m_dataFieldName, local);
      }

      CNewDeviceFrame::~CNewDeviceFrame()
      {
      }
   } //namespace ws
} //namespace web


