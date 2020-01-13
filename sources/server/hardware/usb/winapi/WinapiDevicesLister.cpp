#include "stdafx.h"
#include "WinapiDevicesLister.h"
//TODO remettre #include "WinapiDevice.h"
#include <shared/Log.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <initguid.h>
#include <usbiodef.h>
#include <Winusb.h>


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<GUID> CWinapiDevicesLister::readUsbClassGuid() const
      {
         static const TCHAR DeviceClassName[] = "USB";
         const auto guid = boost::make_shared<GUID>();
         DWORD requiredSize;
         if (SetupDiClassGuidsFromName(DeviceClassName,
                                       guid.get(),
                                       1,
                                       &requiredSize) == FALSE)
         {
            throw std::invalid_argument("Unable to get USB device info, incorrect class name");
         }
         return guid;
      }

      std::string CWinapiDevicesLister::getUsbDevice(boost::shared_ptr<const GUID> usbClassGuid,
                                                     int index)
      {
         //get device info set for our device class
         const auto deviceInfoSetHandler = SetupDiGetClassDevs(usbClassGuid.get(),
                                                               nullptr,
                                                               nullptr,
                                                               DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
         if (deviceInfoSetHandler == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Unable to get USB device info, information is unavailable");
         /*
         SP_DEVINFO_DATA deviceInfoData = {0};
         deviceInfoData.cbSize = 28;
         if (SetupDiEnumDeviceInfo(deviceInfoSetHandler,
                                   index,
                                   &deviceInfoData) == FALSE)
         {
            // End of devices list
            SetupDiDestroyDeviceInfoList(deviceInfoSetHandler);
            return std::string();
         }

         // Device name
         static const auto MaxDevLen = 1000;
         BYTE deviceName[MaxDevLen];
         if (SetupDiGetDeviceRegistryProperty(deviceInfoSetHandler,
                                              &deviceInfoData,
                                              SPDRP_FRIENDLYNAME,
                                              nullptr,
                                              deviceName,
                                              MaxDevLen,
                                              nullptr) == FALSE)
         {
            if (SetupDiGetDeviceRegistryProperty(deviceInfoSetHandler,
                                                 &deviceInfoData,
                                                 SPDRP_DEVICEDESC,
                                                 nullptr,
                                                 deviceName,
                                                 MaxDevLen,
                                                 nullptr) == FALSE)
            {
               SetupDiDestroyDeviceInfoList(deviceInfoSetHandler);
               throw std::runtime_error("Unable to get USB device info, incorrect device name");
            }
         }

         // Manufacturer name
         BYTE manufacturerName[MaxDevLen];
         if (SetupDiGetDeviceRegistryProperty(deviceInfoSetHandler,
                                              &deviceInfoData,
                                              SPDRP_MFG,
                                              nullptr,
                                              manufacturerName,
                                              MaxDevLen,
                                              nullptr) == FALSE)
         {
            SetupDiDestroyDeviceInfoList(deviceInfoSetHandler);
            throw std::runtime_error("Unable to get USB device info, incorrect device name");
         }
         YADOMS_LOG(trace) << "Manufacturer : " << manufacturerName;
         //TODO manufacturerName non encore utilisé
         */
         // Others device data
         SP_INTERFACE_DEVICE_DATA deviceInterfaceData = {0};
         deviceInterfaceData.cbSize = sizeof(deviceInterfaceData);
         if (SetupDiEnumDeviceInterfaces(deviceInfoSetHandler,
                                         nullptr,
                                         usbClassGuid.get(),
                                         0,
                                         &deviceInterfaceData) == FALSE)
         {
            YADOMS_LOG(trace) << GetLastError();
            SetupDiDestroyDeviceInfoList(deviceInfoSetHandler);
            throw std::runtime_error("Unable to get USB device info, no access to interface");
         }

         SP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDataDetails = {0};
         deviceInterfaceDataDetails.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
         const auto deviceInterfaceDataDetailsSize = deviceInterfaceDataDetails.cbSize + 256; //TODO 256 ==> moche
         if (SetupDiGetDeviceInterfaceDetail(deviceInfoSetHandler,
                                             &deviceInterfaceData,
                                             &deviceInterfaceDataDetails,
                                             deviceInterfaceDataDetailsSize,
                                             nullptr,
                                             nullptr) == FALSE)
         {
            SetupDiDestroyDeviceInfoList(deviceInfoSetHandler);
            throw std::runtime_error("Unable to get USB device info, no access to interface");
         }

         YADOMS_LOG(trace) << deviceInterfaceDataDetails.DevicePath;

         SetupDiDestroyDeviceInfoList(deviceInfoSetHandler);
         return std::string("device");
         //return std::string(reinterpret_cast<TCHAR*>(deviceName)); //TODO remettre
      }

      typedef struct _DEVICE_GUID_LIST {
         HDEVINFO   DeviceInfo;
         LIST_ENTRY ListHead;
      } DEVICE_GUID_LIST, * PDEVICE_GUID_LIST;

      typedef struct _DEVICE_INFO_NODE {
         HDEVINFO                         DeviceInfo;
         LIST_ENTRY                       ListEntry;
         SP_DEVINFO_DATA                  DeviceInfoData;
         SP_DEVICE_INTERFACE_DATA         DeviceInterfaceData;
         PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceDetailData;
         PSTR                             DeviceDescName;
         ULONG                            DeviceDescNameLength;
         PSTR                             DeviceDriverName;
         ULONG                            DeviceDriverNameLength;
         DEVICE_POWER_STATE               LatestDevicePowerState;
      } DEVICE_INFO_NODE, * PDEVICE_INFO_NODE;//TODO virer ?

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {RemoveEntryList((ListHead)->Flink)}

#define RemoveEntryList(Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

#define FREE(hMem)  GlobalFree((hMem))

      VOID
         FreeDeviceInfoNode(
            _In_ PDEVICE_INFO_NODE* ppNode
         )
      {
         if (ppNode == NULL)
         {
            return;
         }

         if (*ppNode == NULL)
         {
            return;
         }

         if ((*ppNode)->DeviceDetailData != NULL)
         {
            FREE((*ppNode)->DeviceDetailData);
         }

         if ((*ppNode)->DeviceDescName != NULL)
         {
            FREE((*ppNode)->DeviceDescName);
         }

         if ((*ppNode)->DeviceDriverName != NULL)
         {
            FREE((*ppNode)->DeviceDriverName);
         }

         FREE(*ppNode);
         *ppNode = NULL;
      }

      FORCEINLINE
         VOID
         InitializeListHead(
            _Out_ PLIST_ENTRY ListHead
         )
      {
         ListHead->Flink = ListHead->Blink = ListHead;
      }

      void
         ClearDeviceList(
            PDEVICE_GUID_LIST DeviceList
         )
      {
         if (DeviceList->DeviceInfo != INVALID_HANDLE_VALUE)
         {
            SetupDiDestroyDeviceInfoList(DeviceList->DeviceInfo);
            DeviceList->DeviceInfo = INVALID_HANDLE_VALUE;
         }

         while (!IsListEmpty(&DeviceList->ListHead))
         {
            PDEVICE_INFO_NODE pNode = NULL;
            PLIST_ENTRY pEntry;

            pEntry = RemoveHeadList(&DeviceList->ListHead);

            pNode = CONTAINING_RECORD(pEntry,
               DEVICE_INFO_NODE,
               ListEntry);

            FreeDeviceInfoNode(&pNode);
         }
      }

      #define ALLOC(dwBytes) GlobalAlloc(GPTR, (dwBytes))

#define  OOPS()//TODO gérer les erreurs

      BOOL
         GetDeviceProperty(
            _In_    HDEVINFO         DeviceInfoSet,
            _In_    PSP_DEVINFO_DATA DeviceInfoData,
            _In_    DWORD            Property,
            _Outptr_  LPTSTR* ppBuffer
         )
      {
         BOOL bResult;
         DWORD requiredLength = 0;
         DWORD lastError;

         if (ppBuffer == NULL)
         {
            return FALSE;
         }

         *ppBuffer = NULL;

         bResult = SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
            DeviceInfoData,
            Property,
            NULL,
            NULL,
            0,
            &requiredLength);
         lastError = GetLastError();

         if ((requiredLength == 0) || (bResult != FALSE && lastError != ERROR_INSUFFICIENT_BUFFER))
         {
            return FALSE;
         }

         *ppBuffer = (LPTSTR)ALLOC(requiredLength);

         if (*ppBuffer == NULL)
         {
            return FALSE;
         }

         bResult = SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
            DeviceInfoData,
            Property,
            NULL,
            (PBYTE)*ppBuffer,
            requiredLength,
            &requiredLength);
         if (bResult == FALSE)
         {
            FREE(*ppBuffer);
            *ppBuffer = NULL;
            return FALSE;
         }

         return TRUE;
      }


#define InsertTailList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }

      void
         EnumerateAllDevicesWithGuid(
            PDEVICE_GUID_LIST DeviceList,
            LPGUID Guid
         )
      {
         if (DeviceList->DeviceInfo != INVALID_HANDLE_VALUE)
         {
            ClearDeviceList(DeviceList);
         }

         DeviceList->DeviceInfo = SetupDiGetClassDevs(Guid,
            NULL,
            NULL,
            (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

         if (DeviceList->DeviceInfo != INVALID_HANDLE_VALUE)
         {
            ULONG                    index;
            DWORD error;

            error = 0;
            index = 0;

            while (error != ERROR_NO_MORE_ITEMS)
            {
               BOOL success;
               PDEVICE_INFO_NODE pNode;

               pNode = (PDEVICE_INFO_NODE)ALLOC(sizeof(DEVICE_INFO_NODE));
               if (pNode == NULL)
               {
                  OOPS();
                  break;
               }
               pNode->DeviceInfo = DeviceList->DeviceInfo;
               pNode->DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
               pNode->DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

               success = SetupDiEnumDeviceInfo(DeviceList->DeviceInfo,
                  index,
                  &pNode->DeviceInfoData);

               index++;

               if (success == FALSE)
               {
                  error = GetLastError();

                  if (error != ERROR_NO_MORE_ITEMS)
                  {
                     OOPS();
                  }

                  FreeDeviceInfoNode(&pNode);
               }
               else
               {
                  BOOL   bResult;
                  ULONG  requiredLength;

                  bResult = GetDeviceProperty(DeviceList->DeviceInfo,
                     &pNode->DeviceInfoData,
                     SPDRP_DEVICEDESC,
                     &pNode->DeviceDescName);
                  if (bResult == FALSE)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  bResult = GetDeviceProperty(DeviceList->DeviceInfo,
                     &pNode->DeviceInfoData,
                     SPDRP_DRIVER,
                     &pNode->DeviceDriverName);
                  if (bResult == FALSE)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  pNode->DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

                  success = SetupDiEnumDeviceInterfaces(DeviceList->DeviceInfo,
                     0,
                     Guid,
                     index - 1,
                     &pNode->DeviceInterfaceData);
                  if (!success)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  success = SetupDiGetDeviceInterfaceDetail(DeviceList->DeviceInfo,
                     &pNode->DeviceInterfaceData,
                     NULL,
                     0,
                     &requiredLength,
                     NULL);

                  error = GetLastError();

                  if (!success && error != ERROR_INSUFFICIENT_BUFFER)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  pNode->DeviceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)ALLOC(requiredLength);

                  if (pNode->DeviceDetailData == NULL)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  pNode->DeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

                  success = SetupDiGetDeviceInterfaceDetail(DeviceList->DeviceInfo,
                     &pNode->DeviceInterfaceData,
                     pNode->DeviceDetailData,
                     requiredLength,
                     &requiredLength,
                     NULL);
                  if (!success)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  InsertTailList(&DeviceList->ListHead, &pNode->ListEntry);
                  YADOMS_LOG(trace) << pNode->DeviceDescName;
                  YADOMS_LOG(trace) << pNode->DeviceDetailData->DevicePath;

                  const std::string devicePath(pNode->DeviceDetailData->DevicePath);

                  boost::regex pattern(R"(\\\\\?\\usb#vid_([0-9a-fA-F]{4})&pid_([0-9a-fA-F]{4})#([[:graph:]]+)#{[0-9a-fA-F-]+})");
                  boost::smatch result;
                  if (!boost::regex_search(devicePath, result, pattern))
                  {
                     YADOMS_LOG(trace) << "invalid USB path " << devicePath;
                  }

                  const std::string vid (result[1].first, result[1].second);
                  const std::string pid (result[2].first, result[2].second);
                  const std::string serial(result[3].first, result[3].second);

                  YADOMS_LOG(trace) << "USB device " << vid << ", " << pid << ", " << serial;


                  continue;



                  PSTR hardwareID;
                  success = GetDeviceProperty(DeviceList->DeviceInfo,
                     &pNode->DeviceInfoData,
                     SPDRP_HARDWAREID,
                     &hardwareID);
                  if (!success)
                  {
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  YADOMS_LOG(trace) << hardwareID;

                  // Open the device
                  HANDLE deviceHandle = CreateFileA(pNode->DeviceDetailData->DevicePath,
                     GENERIC_WRITE | GENERIC_READ,
                     FILE_SHARE_WRITE | FILE_SHARE_READ,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                     NULL);

                  if (deviceHandle ==INVALID_HANDLE_VALUE)
                  {
                     auto lastError = GetLastError();
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  //TODO ne pas oublier CloseHandle(usbHandle);
                  continue;

                  WINUSB_INTERFACE_HANDLE usbHandle;
                  if (WinUsb_Initialize(deviceHandle,
                     &usbHandle) == FALSE)
                  {
                     auto lastError = GetLastError();
                     CloseHandle(deviceHandle);
                     FreeDeviceInfoNode(&pNode);
                     OOPS();
                     continue;
                  }

                  USB_DEVICE_DESCRIPTOR udd;
                  memset(&udd, 0, sizeof(udd));
                  ULONG LengthTransferred = 0;

                  WinUsb_GetDescriptor(
                     usbHandle, // returned by WinUsbInitialize
                     URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE,
                     0,     // not sure if we need this
                     0x409, // not sure if we need this
                     (PUCHAR)&udd,
                     sizeof(udd),
                     &LengthTransferred);

                  //WinUsb_Free(usbHandle);
               }
            }
         }
      }

      std::vector<boost::shared_ptr<IDevice>> CWinapiDevicesLister::fromRequest(
         const shared::CDataContainer& request) const
      {
         //TODO virer
         request.printToLog(YADOMS_LOG(debug));

         //TODO appliquer les filtres

         DEVICE_GUID_LIST gDeviceList;
         gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
         InitializeListHead(&gDeviceList.ListHead);
         EnumerateAllDevicesWithGuid(&gDeviceList,
            (LPGUID)&GUID_DEVINTERFACE_USB_DEVICE);
//TODO         ClearDeviceList(&gDeviceList);


         PDEVICE_INFO_NODE pNode = NULL;
         PDEVICE_GUID_LIST pList = NULL;
         PLIST_ENTRY       pEntry = NULL;

         pList = &gDeviceList;

         pEntry = pList->ListHead.Flink;

         while (pEntry != &pList->ListHead)
         {
            pNode = CONTAINING_RECORD(pEntry,
               DEVICE_INFO_NODE,
               ListEntry);
            
            YADOMS_LOG(trace) << pNode->DeviceDescName;

            pEntry = pEntry->Flink;
         }

         return std::vector<boost::shared_ptr<IDevice>>(); //TODO



         ////////////////

         auto index = 0;
         while (true)
         {
            try
            {
               const auto usbDevice = getUsbDevice(readUsbClassGuid(),
                                                   index++);
               if (usbDevice.empty())
                  break;

               YADOMS_LOG(debug) << "USB device found : " << usbDevice;
            }
            catch (std::exception& exception)
            {
               YADOMS_LOG(trace) << exception.what();
            }
         }
         return std::vector<boost::shared_ptr<IDevice>>(); //TODO
      }
   } // namespace usb
} // namespace hardware
