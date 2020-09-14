#include "stdafx.h"
#include "DeviceRemoved.h"

namespace pluginSystem
{
   CDeviceRemoved::CDeviceRemoved(const std::string& device,
                                  const boost::shared_ptr<shared::CDataContainer>& details)
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

   const boost::shared_ptr<shared::CDataContainer>& CDeviceRemoved::details() const
   {
      return m_details;
   }
} // namespace pluginSystem	


