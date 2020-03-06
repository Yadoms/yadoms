#include "stdafx.h"
#include "DeviceRemoved.h"

namespace pluginSystem
{
   CDeviceRemoved::CDeviceRemoved(const std::string& device,
                                  const shared::CDataContainerSharedPtr& details)
      : m_device(device),
        m_details(details)
   {
   }

   CDeviceRemoved::~CDeviceRemoved()
   {
   }

   const std::string& CDeviceRemoved::device() const
   {
      return m_device;
   }

   const shared::CDataContainerSharedPtr& CDeviceRemoved::details() const
   {
      return m_details;
   }
} // namespace pluginSystem	


