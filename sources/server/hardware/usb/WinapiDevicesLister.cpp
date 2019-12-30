#include "stdafx.h"
#include "WinapiDevicesLister.h"
//TODO remettre #include "WinapiDevice.h"
#include <shared/Log.h>
#include <SetupAPI.h>

namespace hardware
{
   namespace usb
   {
      std::vector<boost::shared_ptr<IDevice>> CWinapiDevicesLister::fromRequest(
         const shared::CDataContainer& request) const
      {
         //TODO virer
         request.printToLog(YADOMS_LOG(debug));

         //TODO appliquer les filtres

         HDEVINFO DeviceInfoSet = SetupDiGetClassDevsW(nullptr,
                                                       L"USB",
                                                       nullptr,
                                                       DIGCF_ALLCLASSES | DIGCF_PRESENT);

         SP_DEVINFO_DATA DeviceInfoData;
         ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
         DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
         DWORD DeviceIndex = 0;

         while (SetupDiEnumDeviceInfo(DeviceInfoSet,
                                      DeviceIndex,
                                      &DeviceInfoData))
         {
            DeviceIndex++;

            DEVPROPKEY DEVPKEY_Device_Class;
            DEVPROPTYPE PropType;
            DWORD Size;
            BYTE DevGuid; // TODO appeler la fonction avec DevGuid pour obtenir la taille nécessaire
            if (!SetupDiGetDevicePropertyW(DeviceInfoSet,
                                           &DeviceInfoData,
                                           &DEVPKEY_Device_Class,
                                           &PropType,
                                           static_cast<PBYTE>(&DevGuid),
                                           sizeof(GUID),
                                           &Size,
                                           0) || PropType != DEVPROP_TYPE_GUID)
            {
               DWORD Error = GetLastError();

               if (Error == ERROR_NOT_FOUND)
               {
                  //TODO                   This device has an unknown device setup class
               }
            }
         }

         if (DeviceInfoSet)
         {
            SetupDiDestroyDeviceInfoList(DeviceInfoSet);
         }


         //const auto devices = LibUSB::LibUSB::FindAllDevices();
         std::vector<boost::shared_ptr<IDevice>> devicesList;
         //for(const auto& device: devices)
         //   devicesList.push_back(boost::make_shared<CLibusbppDevice>(device));
         return devicesList;
      }
   } // namespace usb
} // namespace hardware
