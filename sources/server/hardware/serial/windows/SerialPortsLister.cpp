#include "stdafx.h"
#include "SerialPortsLister.h"
#include <windows.h>
#include <SetupAPI.h>
#include "WinapiDevice.h"
#include <regex>
#include <utility>

namespace hardware
{
   namespace serial
   {
      CSerialPortsLister::CSerialPortsLister(boost::shared_ptr<usb::IDevicesLister> usbDeviceListers)
         : m_usbDeviceListers(std::move(usbDeviceListers))
      {
      }

      std::vector<boost::shared_ptr<ISerialPort>> CSerialPortsLister::listSerialPorts()
      {
         auto serialPorts = listAllSerialPorts();
         updatePortsKind(serialPorts);
         std::vector<boost::shared_ptr<ISerialPort>> iSerialPorts;
         std::copy(serialPorts.begin(), serialPorts.end(), std::back_inserter(iSerialPorts));
         return iSerialPorts;
         //TODO : LastKnownSerialPortPath contient le chemin USB en cas d'adaptateur USB. Le port série est perdu.
      }

      std::vector<boost::shared_ptr<CSerialPort>> CSerialPortsLister::listAllSerialPorts() const
      {
         HKEY serialCommKey;
         if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          "HARDWARE\\DEVICEMAP\\SERIALCOMM",
                          0,
                          KEY_QUERY_VALUE,
                          &serialCommKey) != ERROR_SUCCESS)
         {
            // Unable to access registry ==> HARDWARE\\DEVICEMAP\\SERIALCOMM key does not exist ==> no serial port on this machine
            return std::vector<boost::shared_ptr<CSerialPort>>();
         }

         DWORD valueIndex = 0;
         static const DWORD SerialPortNameMaxLength = 1000; // Should be enough
         TCHAR serialPortName[SerialPortNameMaxLength];
         auto serialPortNameLength = SerialPortNameMaxLength;
         DWORD dataType = 0;
         static const DWORD MountPointMaxLength = 1000; // Should be enough
         BYTE mountPoint[MountPointMaxLength];
         auto mountPointLength = MountPointMaxLength;
         std::vector<boost::shared_ptr<CSerialPort>> serialPorts;
         while (RegEnumValue(serialCommKey,
                             valueIndex,
                             serialPortName,
                             &serialPortNameLength,
                             nullptr,
                             &dataType,
                             mountPoint,
                             &mountPointLength) == ERROR_SUCCESS)
         {
            if (dataType == REG_SZ)
            {
               // Assure compatibility with serial port numbers > 9
               const auto portName((boost::format("\\\\.\\%1%") % reinterpret_cast<char*>(mountPoint)).str());

               //ex
               //portName : COM1
               //serialPortName : \Device\Serial0
               //=> friendlyName : COM1 (\Device\Serial0)
               const auto friendlyName(
                  (boost::format("%1% (%2%)") % reinterpret_cast<char*>(mountPoint) % serialPortName).str());

               auto port = boost::make_shared<CSerialPort>(portName,
                                                           friendlyName,
                                                           database::entities::ESerialPortAdapterKind::kUnknown);
               serialPorts.emplace_back(port);
            }

            // Increment index of key, and reset string length for next iteration
            valueIndex++;
            serialPortNameLength = SerialPortNameMaxLength;
            mountPointLength = MountPointMaxLength;
         }

         RegCloseKey(serialCommKey);
         return serialPorts;
      }

      void CSerialPortsLister::updatePortsKind(std::vector<boost::shared_ptr<CSerialPort>>& detectedSerialPorts) const
      {
         const auto usbDevices = m_usbDeviceListers->listUsbDevices();

         for (auto& detectedSerialPort : detectedSerialPorts)
         {
            const auto serialPortName = getSerialPortName(detectedSerialPort->lastKnownSerialPortPath());
            const auto correspondingUsbToSerialAdapter = std::find_if(
               usbDevices.begin(),
               usbDevices.end(),
               [&](const auto& usbDevice)
               {
                  // Find by serial connection path (ie "COM1", "tty2")
                  return getSerialPortNameFromUsbAdapter(usbDevice) == serialPortName;
               });

            if (correspondingUsbToSerialAdapter != usbDevices.end())
            {
               // USB to serial adapter
               detectedSerialPort->setAdapterKind(database::entities::ESerialPortAdapterKind::kUsbAdapter);
               // Override description as USB device provides a better one
               detectedSerialPort->setDescription((*correspondingUsbToSerialAdapter)->friendlyName());
               detectedSerialPort->setAdapterParameters((*correspondingUsbToSerialAdapter)->allParameters());
            }
            else
            {
               // Physical port
               detectedSerialPort->setAdapterKind(database::entities::ESerialPortAdapterKind::kPhysical);
            }
         }
      }

      std::string CSerialPortsLister::getSerialPortName(const std::string& serialPortConnectionPath) const
      {
         std::smatch match;
         if (std::regex_search(serialPortConnectionPath,
                               match,
                               std::regex(R"(^.*(COM\d+)$)")))
         {
            if (match.size() != 2)
               return std::string();
            return match[1];
         }
         return std::string();
      }

      std::string CSerialPortsLister::getSerialPortNameFromUsbAdapter(
         const boost::shared_ptr<usb::IDevice>& device) const
      {
         const auto* winapiUsbDevice = dynamic_cast<usb::CWinapiDevice*>(device.get());
         if (!winapiUsbDevice)
            throw std::runtime_error("Unable to dynamic cast IDevice to CWinapiDevice");

         if (winapiUsbDevice->getWindowsProperty(SPDRP_ENUMERATOR_NAME) == "USB")
         {
            const auto friendlyName = winapiUsbDevice->getWindowsProperty(SPDRP_FRIENDLYNAME);
            std::smatch match;
            if (std::regex_search(friendlyName,
                                  match,
                                  std::regex(R"(^.*\((COM\d+)\)$)")))
            {
               if (match.size() != 2)
                  return std::string();
               return match[1];
            }
         }

         return std::string();
      }
   } // namespace serial
} // namespace hardware
