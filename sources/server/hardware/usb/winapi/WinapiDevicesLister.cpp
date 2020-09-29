#include "stdafx.h"
#include "WinapiDevicesLister.h"
#include "WinapiDevice.h"
#include <shared/Log.h>
#include <Windows.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <initguid.h>
#include <usbiodef.h>
#include <codecvt>


namespace hardware
{
   namespace usb
   {
      class CDeviceInterfaceDetailDataContainer final
      {
      public:
         explicit CDeviceInterfaceDetailDataContainer(SIZE_T requiredLength)
         {
            m_deviceDetailData = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(GlobalAlloc(
               GPTR, requiredLength));
            if (m_deviceDetailData == nullptr)
               throw std::bad_alloc();
            m_deviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
         }

         CDeviceInterfaceDetailDataContainer(const CDeviceInterfaceDetailDataContainer&) = delete;
         CDeviceInterfaceDetailDataContainer(const CDeviceInterfaceDetailDataContainer&&) = delete;
         CDeviceInterfaceDetailDataContainer& operator=(const CDeviceInterfaceDetailDataContainer&) = delete;
         CDeviceInterfaceDetailDataContainer& operator=(const CDeviceInterfaceDetailDataContainer&&) = delete;

         ~CDeviceInterfaceDetailDataContainer()
         {
            if (m_deviceDetailData != nullptr)
               GlobalFree(m_deviceDetailData);
         }

         PSP_DEVICE_INTERFACE_DETAIL_DATA get() const
         {
            return m_deviceDetailData;
         }

      private:
         PSP_DEVICE_INTERFACE_DETAIL_DATA m_deviceDetailData;
      };

      class CLpstrBuffer final
      {
      public:
         explicit CLpstrBuffer(SIZE_T requiredLength)
         {
            m_buffer = static_cast<LPTSTR>(GlobalAlloc(GPTR, requiredLength));
            if (m_buffer == nullptr)
               throw std::bad_alloc();
         }

         CLpstrBuffer(const CLpstrBuffer&) = delete;
         CLpstrBuffer(const CLpstrBuffer&&) = delete;
         CLpstrBuffer& operator=(const CLpstrBuffer&) = delete;
         CLpstrBuffer& operator=(const CLpstrBuffer&&) = delete;

         ~CLpstrBuffer()
         {
            if (m_buffer != nullptr)
               GlobalFree(m_buffer);
         }

         LPTSTR get() const
         {
            return m_buffer;
         }

      private:
         LPTSTR m_buffer;
      };

      std::wstring CWinapiDevicesLister::toUtf8WideChar(const char* src)
      {
         if (!src)
            throw std::runtime_error("Fail to convert string, src is null");

         const int srcLen = strlen(src);
         if (srcLen == 0)
            return std::wstring();

         const auto requiredSize = MultiByteToWideChar(GetACP(),
                                                       0,
                                                       src,
                                                       srcLen,
                                                       nullptr,
                                                       0);

         if (requiredSize == 0)
            return std::wstring();

         const auto wideCharArray = new wchar_t[requiredSize + 1];
         wideCharArray[requiredSize] = 0;

         if (!MultiByteToWideChar(GetACP(),
                                  0,
                                  src,
                                  srcLen,
                                  wideCharArray,
                                  requiredSize))
         {
            delete[] wideCharArray;
            return std::wstring();
         }

         std::wstring str(wideCharArray);
         delete[] wideCharArray;
         return str;
      }

      std::string CWinapiDevicesLister::toUtf8(const char* buffer)
      {
         return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(toUtf8WideChar(buffer));
      }

      std::string CWinapiDevicesLister::getDeviceProperty(const HDEVINFO deviceInfoSet,
                                                          PSP_DEVINFO_DATA deviceInfoData,
                                                          const DWORD property)
      {
         DWORD requiredLength = 0;
         if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,
                                              deviceInfoData,
                                              property,
                                              nullptr,
                                              nullptr,
                                              0,
                                              &requiredLength) != FALSE && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            throw std::runtime_error(
               "SetupDiGetDeviceRegistryProperty failed with error " + std::to_string(GetLastError()));


         const CLpstrBuffer buffer(requiredLength);
         if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,
                                              deviceInfoData,
                                              property,
                                              nullptr,
                                              reinterpret_cast<PBYTE>(buffer.get()),
                                              requiredLength,
                                              &requiredLength) == FALSE)
            throw std::runtime_error(
               "SetupDiGetDeviceRegistryProperty failed with error " + std::to_string(GetLastError()));

         try
         {
            return toUtf8(buffer.get());
         }
         catch (std::exception&)
         {
            //Default conversion
            return std::string(buffer.get());
         }
      }

      std::vector<boost::shared_ptr<IDevice>> CWinapiDevicesLister::listUsbDevices()
      {
         const auto guid = &GUID_DEVINTERFACE_USB_DEVICE;

         const auto deviceInfo = SetupDiGetClassDevs(guid,
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
            SP_DEVINFO_DATA deviceInfoData = {0};
            deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
            if (SetupDiEnumDeviceInfo(deviceInfo,
                                      index,
                                      &deviceInfoData) == FALSE)
            {
               if (GetLastError() != ERROR_NO_MORE_ITEMS)
                  continue;

               return deviceList;
            }

            index++;

            try
            {
               auto windowsPropertyMap = boost::make_shared<std::map<unsigned int, std::string>>();
               for (unsigned int spdrp = 0; spdrp < SPDRP_MAXIMUM_PROPERTY; ++spdrp)
               {
                  try
                  {
                     (*windowsPropertyMap)[spdrp] = getDeviceProperty(deviceInfo,
                                                                      &deviceInfoData,
                                                                      spdrp);
                  }
                  catch (...)
                  {
                     // Ignore unset values
                  }
               }

               SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {0};
               deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
               if (SetupDiEnumDeviceInterfaces(deviceInfo,
                                               nullptr,
                                               guid,
                                               index - 1,
                                               &deviceInterfaceData) == FALSE)
               {
                  continue;
               }

               ULONG requiredLength;
               if (SetupDiGetDeviceInterfaceDetail(deviceInfo,
                                                   &deviceInterfaceData,
                                                   nullptr,
                                                   0,
                                                   &requiredLength,
                                                   nullptr) == FALSE
                  && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
               {
                  continue;
               }

               CDeviceInterfaceDetailDataContainer deviceDetailData(requiredLength);
               if (SetupDiGetDeviceInterfaceDetail(deviceInfo,
                                                   &deviceInterfaceData,
                                                   deviceDetailData.get(),
                                                   requiredLength,
                                                   &requiredLength,
                                                   nullptr) == FALSE)
               {
                  continue;
               }

               const std::string devicePath(deviceDetailData.get()->DevicePath);

               boost::regex pattern(
                  R"(\\\\\?\\usb#vid_([0-9a-fA-F]{4})&pid_([0-9a-fA-F]{4})#([[:graph:]]+)#{[0-9a-fA-F-]+})");
               boost::smatch result;
               if (!regex_search(devicePath, result, pattern))
               {
                  YADOMS_LOG(trace) << "invalid USB path " << devicePath;
                  continue;
               }

               const auto vid = std::stoi(std::string(result[1].first, result[1].second), nullptr, 16);
               const auto pid = std::stoi(std::string(result[2].first, result[2].second), nullptr, 16);
               const std::string serial(result[3].first, result[3].second);

               deviceList.emplace_back(boost::make_shared<CWinapiDevice>(devicePath,
                                                                         vid,
                                                                         pid,
                                                                         serial,
                                                                         windowsPropertyMap));
            }
            catch (std::exception& exception)
            {
               YADOMS_LOG(trace) << "Unable to get USB device information, " << exception.what();
            }
         }
      }
   } // namespace usb
} // namespace hardware
