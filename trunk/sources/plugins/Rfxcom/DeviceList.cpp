#include "stdafx.h"
#include "DeviceList.h"
#include <shared/exception/InvalidParameter.hpp>


CDeviceList::CDeviceList(boost::shared_ptr<yApi::IYadomsApi> context)
   :m_context(context)
{
}

CDeviceList::~CDeviceList()
{
}

void CDeviceList::declareDevice(const std::string& device, const std::string& model, const std::string& parameters)
{
   if (m_devices.find(device) != m_devices.end())
      return;  // Device already exists

   // Device doesn't exist in the list, search it in database
   if (m_context->deviceExists(device))
   {
      // Device already exists in database, add it into list
      m_devices[device] = m_context->getDeviceDetails(device);
      return;
   }

   // Device doesn't exist in database, declare it and add it into list
   m_context->declareDevice(device, model, parameters);
   m_devices[device] = parameters;
}

const std::string& CDeviceList::getDeviceParameters(const std::string& device)
{
   DeviceList::const_iterator it = m_devices.find(device);
   if (it != m_devices.end())
      return it->second;

   // Device doesn't exist in the list, search it in database
   if (!m_context->deviceExists(device))
      throw shared::exception::CInvalidParameter(device);

   // Device already exists in database, add it into list
   m_devices[device] = m_context->getDeviceDetails(device);
   return m_devices[device];
}


