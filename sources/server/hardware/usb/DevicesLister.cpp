#include "stdafx.h"
#include "DevicesLister.h"
#include "Device.h"
#include <shared/Log.h>
#include <libusbpp.hpp>


namespace hardware
{
   namespace usb
   {
      std::vector<boost::shared_ptr<IDevice>> CDevicesLister::fromRequest(const shared::CDataContainer& request) const
      {
         //TODO virer
         request.printToLog(YADOMS_LOG(debug));

         //TODO appliquer les filtres

         const auto devices = LibUSB::LibUSB::FindAllDevices();
         std::vector<boost::shared_ptr<IDevice>> todo;
         for(const auto& device: devices)
            todo.push_back(boost::make_shared<CDevice>(*device));
         return todo;
      }
   } // namespace usb
} // namespace hardware
