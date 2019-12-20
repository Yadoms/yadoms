#include "stdafx.h"
#include "DevicesLister.h"
#include "Device.h"
#include <shared/Log.h>


namespace hardware
{
   namespace usb
   {
      std::vector<boost::shared_ptr<IDevice>> CDevicesLister::fromRequest(const shared::CDataContainer& request) const
      {
         //TODO
         request.printToLog(YADOMS_LOG(debug));

         std::vector<boost::shared_ptr<IDevice>> todo;
         todo.push_back(boost::make_shared<CDevice>());
         return todo;
      }
   } // namespace usb
} // namespace hardware
