#include "stdafx.h"
#include "LibusbppDevicesLister.h"
#include "LibusbppDevice.h"
#include <shared/Log.h>
#include <libusbpp.hpp>


namespace hardware
{
   namespace usb
   {
      std::vector<boost::shared_ptr<IDevice>> CLibusbppDevicesLister::listUsbDevices()
      {
         const auto devices = LibUSB::LibUSB::FindAllDevices();
         std::vector<boost::shared_ptr<IDevice>> devicesList;
         for(const auto& device: devices)
            devicesList.push_back(boost::make_shared<CLibusbppDevice>(device));
         return devicesList;
      }
   } // namespace usb
} // namespace hardware
