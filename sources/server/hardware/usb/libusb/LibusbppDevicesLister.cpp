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
         YADOMS_LOG(trace) << "ibUSB::LibUSB::FindAllDevices() returned " << devices.size() << " devices";
         std::vector<boost::shared_ptr<IDevice>> devicesList;
         for(const auto& device: devices)
         {
            try
            {
               devicesList.push_back(boost::make_shared<CLibusbppDevice>(device));
            }
            catch(const std::exception& e)
            {
               YADOMS_LOG(warning) << "Unable to access device " << e.what();               
            }
         }
         return devicesList;
      }
   } // namespace usb
} // namespace hardware
