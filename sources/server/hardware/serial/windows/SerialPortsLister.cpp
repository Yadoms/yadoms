#include "stdafx.h"
#include "SerialPortsLister.h"
#include <windows.h>

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSerialPorts()
      {
         auto serialPorts = boost::make_shared<SerialPortsMap>();

         HKEY serialCommKey;
         if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          "HARDWARE\\DEVICEMAP\\SERIALCOMM",
                          0,
                          KEY_QUERY_VALUE,
                          &serialCommKey) != ERROR_SUCCESS)
         {
            // Unable to access registry ==> HARDWARE\\DEVICEMAP\\SERIALCOMM key does not exist ==> no serial port on this machine
            return serialPorts;
         }

         DWORD valueIndex = 0;
         static const DWORD SerialPortNameMaxLength = 1000; // Should be enough
         TCHAR serialPortName[SerialPortNameMaxLength];
         auto serialPortNameLength = SerialPortNameMaxLength;
         DWORD dataType = 0;
         static const DWORD MountPointMaxLength = 1000; // Should be enough
         BYTE mountPoint[MountPointMaxLength];
         auto mountPointLength = MountPointMaxLength;
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
               auto portName((boost::format("\\\\.\\%1%") % reinterpret_cast<char*>(mountPoint)).str());

               //ex
               //portname : COM1
               //serialPortName : \Device\Serial0
               //=> friendlyName : COM1 (\Device\Serial0)
               const auto friendlyName(
                  (boost::format("%1% (%2%)") % reinterpret_cast<char*>(mountPoint) % serialPortName).str());
               (*serialPorts)[portName] = friendlyName;
            }

            // Increment index of key, and reset string length for next iteration
            valueIndex++;
            serialPortNameLength = SerialPortNameMaxLength;
            mountPointLength = MountPointMaxLength;
         }

         RegCloseKey(serialCommKey);
         return serialPorts;
      }
   } // namespace serial
} // namespace hardware
