#pragma once
#include "../IDevicesLister.h"
#include <SetupAPI.h>

namespace hardware
{
   namespace usb
   {
      class CWinapiDevicesLister : public IDevicesLister
      {
      public:
         virtual ~CWinapiDevicesLister() = default;

         // IDevicesLister implementation
         std::vector<boost::shared_ptr<IDevice>> listUsbDevices() override;
         // [END] IDevicesLister implementation

      private:
         static std::wstring toUtf8WideChar(const char* src);
         static std::string toUtf8(const char* buffer);
         static std::string getDeviceProperty(const HDEVINFO deviceInfoSet,
                                              PSP_DEVINFO_DATA deviceInfoData,
                                              const DWORD property);
      };
   } // namespace usb
} // namespace hardware
