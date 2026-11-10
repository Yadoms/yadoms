#include "stdafx.h"
#include "WinapiDevicesLister.h"
#include "WinapiDevice.h"
#include <shared/Log.h>
#include <Windows.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <initguid.h>
#include <usbiodef.h>
#include <codecvt>


using namespace hardware::usb;

class CDeviceInterfaceDetailDataContainer final
{
public:
   explicit CDeviceInterfaceDetailDataContainer(size_t requiredLength)
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
   explicit CLpstrBuffer(size_t requiredLength)
   {
      m_buffer = static_cast<wchar_t*>(GlobalAlloc(GPTR, requiredLength));
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

   wchar_t* get() const
   {
      return m_buffer;
   }

private:
   wchar_t* m_buffer;
};

std::wstring CWinapiDevicesLister::toUtf8WideChar(const char* src)
{
   if (!src)
      throw std::runtime_error("Fail to convert string, src is null");

   const int srcLen = static_cast<int>(strlen(src));
   if (srcLen == 0)
      return {};

   const auto requiredSize = MultiByteToWideChar(GetACP(),
                                                 0,
                                                 src,
                                                 srcLen,
                                                 nullptr,
                                                 0);

   if (requiredSize == 0)
      return {};

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
      return {};
   }

   std::wstring str(wideCharArray);
   delete[] wideCharArray;
   return str;
}

std::string CWinapiDevicesLister::toUtf8(const wchar_t* buffer)
{
   if (!buffer)
      return {};

   int sizeNeeded = WideCharToMultiByte(CP_UTF8,
                                        0,
                                        buffer,
                                        -1, // Nuul-terminated string
                                        nullptr,
                                        0,
                                        nullptr,
                                        nullptr);

   if (sizeNeeded <= 0)
      throw std::runtime_error("WideCharToMultiByte failed (size calculation)");

   std::string result(sizeNeeded - 1, '\0'); // -1 to remove null terminator

   int converted = WideCharToMultiByte(CP_UTF8,
                                       0,
                                       buffer,
                                       -1,
                                       result.data(),
                                       sizeNeeded,
                                       nullptr,
                                       nullptr);

   if (converted <= 0)
      throw std::runtime_error("WideCharToMultiByte failed (conversion)");

   return result;
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
      std::wstring wBuffer(buffer.get());
      return {wBuffer.begin(), wBuffer.end()};
   }
}

std::vector<boost::shared_ptr<IDevice>> CWinapiDevicesLister::listUsbDevices()
{
   const auto deviceInfo = SetupDiGetClassDevs(const_cast<LPGUID>(&GUID_DEVINTERFACE_USB_DEVICE),
                                               nullptr,
                                               nullptr,
                                               (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

   if (deviceInfo == INVALID_HANDLE_VALUE)
   {
      YADOMS_LOG(error) << "Error listing USB devices, error " << GetLastError();
      return {};
   }

   DWORD index = 0;
   std::vector<boost::shared_ptr<IDevice>> deviceList;
   while (true)
   {
      SP_DEVINFO_DATA deviceInfoData = {};
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
         const auto deviceName = getDeviceProperty(deviceInfo,
                                                   &deviceInfoData,
                                                   SPDRP_DEVICEDESC);

         SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {};
         deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
         if (SetupDiEnumDeviceInterfaces(deviceInfo,
                                         nullptr,
                                         const_cast<LPGUID>(&GUID_DEVINTERFACE_USB_DEVICE),
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

         std::wstring wDevicePath(deviceDetailData.get()->DevicePath);
         const std::string devicePath(std::string(wDevicePath.begin(), wDevicePath.end()));

         boost::regex pattern(
            R"(\\\\\?\\usb#vid_([0-9a-fA-F]{4})&pid_([0-9a-fA-F]{4})#([[:graph:]]+)#{[0-9a-fA-F-]+})");
         boost::smatch result;
         if (!boost::regex_search(devicePath, result, pattern))
         {
            YADOMS_LOG(trace) << "invalid USB path " << devicePath;
            continue;
         }

         const auto vid = std::stoi(std::string(result[1].first, result[1].second), nullptr, 16);
         const auto pid = std::stoi(std::string(result[2].first, result[2].second), nullptr, 16);
         const std::string serial(result[3].first, result[3].second);

         deviceList.emplace_back(boost::make_shared<CWinapiDevice>(deviceName,
                                                                   devicePath,
                                                                   vid,
                                                                   pid,
                                                                   serial));
      }
      catch (std::exception& exception)
      {
         YADOMS_LOG(trace) << "Unable to get USB device information, " << exception.what();
      }
   }
}
