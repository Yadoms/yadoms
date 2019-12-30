#pragma once
#include "IDevicesLister.h"

namespace hardware
{
   namespace usb
   {
      class CLibusbppDevicesLister : public IDevicesLister
      {
      public:
         virtual ~CLibusbppDevicesLister() = default;

         // IDevicesLister implementation
         std::vector<boost::shared_ptr<IDevice>> fromRequest(const shared::CDataContainer& request) const override;
         // [END] IDevicesLister implementation
      };
   } // namespace usb
} // namespace hardware
