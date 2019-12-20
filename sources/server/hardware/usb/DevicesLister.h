#pragma once
#include "IDevicesLister.h"

namespace hardware
{
   namespace usb
   {
      class CDevicesLister : public IDevicesLister // TODO renommer CDevicesLister en CLibusbppDevicesLister
      {
      public:
         virtual ~CDevicesLister() = default;

         // IDevicesLister implementation
         std::vector<boost::shared_ptr<IDevice>> fromRequest(const shared::CDataContainer& request) const override;
         // [END] IDevicesLister implementation
      };
   } // namespace usb
} // namespace hardware
