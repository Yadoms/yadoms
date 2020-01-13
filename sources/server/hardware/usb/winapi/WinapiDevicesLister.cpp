#include "stdafx.h"
#include "WinapiDevicesLister.h"
#include "WinapiDevice.h"
#include <shared/Log.h>
#include <Windows.h>
#include <SetupAPI.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <initguid.h>
#include <usbiodef.h>


namespace hardware
{
   namespace usb
   {
      struct DEVICE_INFO_NODE //TODO virer ?
      {
         HDEVINFO DeviceInfo;
         LIST_ENTRY ListEntry;
         SP_DEVINFO_DATA DeviceInfoData;
         SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
         PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceDetailData;
         PSTR DeviceDescName;
         ULONG DeviceDescNameLength;
         PSTR DeviceDriverName;
         ULONG DeviceDriverNameLength;
         DEVICE_POWER_STATE LatestDevicePowerState;
      };

      VOID FreeDeviceInfoNode(DEVICE_INFO_NODE** ppNode)
      {
         if (ppNode == nullptr)
            return;

         if (*ppNode == nullptr)
            return;

         if ((*ppNode)->DeviceDetailData != nullptr)
            GlobalFree((*ppNode)->DeviceDetailData);

         if ((*ppNode)->DeviceDescName != nullptr)
            GlobalFree((*ppNode)->DeviceDescName);

         if ((*ppNode)->DeviceDriverName != nullptr)
            GlobalFree((*ppNode)->DeviceDriverName);

         GlobalFree(*ppNode);
         *ppNode = nullptr;
      }

      std::string GetDeviceProperty(const HDEVINFO deviceInfoSet,
                                    const PSP_DEVINFO_DATA deviceInfoData,
                                    const DWORD property)
      {
         DWORD requiredLength = 0;
         if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,
                                              deviceInfoData,
                                              property,
                                              nullptr,
                                              nullptr,
                                              0,
                                              &requiredLength) != FALSE
            && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
         {
            throw std::runtime_error("SetupDiGetDeviceRegistryProperty failed with error " + GetLastError());
         }

         const auto buffer = static_cast<LPTSTR>(GlobalAlloc(GPTR, requiredLength));
         if (buffer == nullptr)
         {
            throw std::bad_alloc();
         }

         if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,
                                              deviceInfoData,
                                              property,
                                              nullptr,
                                              reinterpret_cast<PBYTE>(buffer),
                                              requiredLength,
                                              &requiredLength) == FALSE)
         {
            GlobalFree(buffer);
            throw std::runtime_error("SetupDiGetDeviceRegistryProperty failed with error " + GetLastError());
         }

         return std::string(buffer);
      }

      std::vector<boost::shared_ptr<IDevice>>
      listUsbDevices( //TODO mettre dans la classe
      )
      {
         const auto deviceInfo = SetupDiGetClassDevs(const_cast<LPGUID>(&GUID_DEVINTERFACE_USB_DEVICE),
                                                     nullptr,
                                                     nullptr,
                                                     (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

         if (deviceInfo == INVALID_HANDLE_VALUE)
         {
            YADOMS_LOG(error) << "Error listing USB devices, error " << GetLastError();
            return std::vector<boost::shared_ptr<IDevice>>();
         }

         DWORD index = 0;
         std::vector<boost::shared_ptr<IDevice>> deviceList;
         while (true)
         {
            auto pNode = static_cast<DEVICE_INFO_NODE*>(GlobalAlloc(GPTR, sizeof(DEVICE_INFO_NODE)));
            if (pNode == nullptr)
               throw std::bad_alloc();

            pNode->DeviceInfo = deviceInfo;
            pNode->DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
            pNode->DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

            if (SetupDiEnumDeviceInfo(pNode->DeviceInfo,
                                      index,
                                      &pNode->DeviceInfoData) == FALSE)
            {
               if (GetLastError() != ERROR_NO_MORE_ITEMS)
                  continue;

               FreeDeviceInfoNode(&pNode);
               return deviceList;
            }

            index++;

            try
            {
               const auto deviceName = GetDeviceProperty(pNode->DeviceInfo,
                                                         &pNode->DeviceInfoData,
                                                         SPDRP_DEVICEDESC);

               pNode->DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

               if (SetupDiEnumDeviceInterfaces(pNode->DeviceInfo,
                                               nullptr,
                                               const_cast<LPGUID>(&GUID_DEVINTERFACE_USB_DEVICE),
                                               index - 1,
                                               &pNode->DeviceInterfaceData) == FALSE)
               {
                  FreeDeviceInfoNode(&pNode);
                  continue;
               }

               ULONG requiredLength;
               if (SetupDiGetDeviceInterfaceDetail(pNode->DeviceInfo,
                                                   &pNode->DeviceInterfaceData,
                                                   nullptr,
                                                   0,
                                                   &requiredLength,
                                                   nullptr) == FALSE
                  && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
               {
                  FreeDeviceInfoNode(&pNode);
                  continue;
               }

               pNode->DeviceDetailData = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(GlobalAlloc(
                  GPTR, requiredLength));
               if (pNode->DeviceDetailData == nullptr)
               {
                  FreeDeviceInfoNode(&pNode);
                  continue;
               }

               pNode->DeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

               if (SetupDiGetDeviceInterfaceDetail(pNode->DeviceInfo,
                                                   &pNode->DeviceInterfaceData,
                                                   pNode->DeviceDetailData,
                                                   requiredLength,
                                                   &requiredLength,
                                                   nullptr) == FALSE)
               {
                  FreeDeviceInfoNode(&pNode);
                  continue;
               }

               const std::string devicePath(pNode->DeviceDetailData->DevicePath);

               boost::regex pattern(
                  R"(\\\\\?\\usb#vid_([0-9a-fA-F]{4})&pid_([0-9a-fA-F]{4})#([[:graph:]]+)#{[0-9a-fA-F-]+})");
               boost::smatch result;
               if (!boost::regex_search(devicePath, result, pattern))
               {
                  YADOMS_LOG(trace) << "invalid USB path " << devicePath;
                  continue;
               }

               const auto vid = std::stoul(std::string(result[1].first, result[1].second), nullptr, 16);
               const auto pid = std::stoul(std::string(result[2].first, result[2].second), nullptr, 16);
               const std::string serial(result[3].first, result[3].second);

               deviceList.emplace_back(boost::make_shared<CWinapiDevice>(deviceName,
                                                                         devicePath,
                                                                         vid,
                                                                         pid,
                                                                         serial));
            }
            catch (std::exception& exception)
            {
               FreeDeviceInfoNode(&pNode);
               YADOMS_LOG(trace) << "Unable to get USB device information, " << exception.what();
            }
         }
      }

      std::vector<boost::shared_ptr<IDevice>> CWinapiDevicesLister::fromRequest(
         const shared::CDataContainer& request) const
      {
         //TODO virer
         request.printToLog(YADOMS_LOG(debug));

         auto usbDevices = listUsbDevices();
         for (const auto& device : usbDevices)
         {
            YADOMS_LOG(trace) << "USB device " << device->yadomsFriendlyName()
               << ", path=" << device->yadomsConnectionId()
               << ", vid=" << device->vendorId()
               << ", pid=" << device->productId()
               << ", serial=" << device->serialNumber();
         }
         usbDevices.erase(std::remove_if(usbDevices.begin(),
                                         usbDevices.end(),
                                         [](const boost::shared_ptr<IDevice> device)
                                         {
                                            //TODO appliquer les filtres
                                            //if (device->productId())
                                            return false;
                                         }),
                          usbDevices.end());

         return usbDevices;
      }
   } // namespace usb
} // namespace hardware
