#include "stdafx.h"
#include "Peripherals.h"
#include <windows.h>

namespace shared
{
   CPeripherals::CPeripherals()
   {
   }

   CPeripherals::~CPeripherals()
   {
   }

   boost::shared_ptr<const CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
   {
      auto serialPorts = boost::make_shared<SerialPortsMap>();

      HKEY serialcommKey;
      if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                         "HARDWARE\\DEVICEMAP\\SERIALCOMM",
                                         0,
                                         KEY_QUERY_VALUE,
                                         &serialcommKey) != ERROR_SUCCESS)
      {
         // Unable to access registry ==> HARDWARE\\DEVICEMAP\\SERIALCOMM key does not exist ==> no serial port on this machine
         return serialPorts;
      }

      DWORD valueIndex = 0;
      static const DWORD serialPortNameMaxLength = 1000; // Should be enough
      TCHAR serialPortName[serialPortNameMaxLength];
      auto serialPortNameLength = serialPortNameMaxLength;
      DWORD dataType = 0;
      static const DWORD mountPointMaxLength = 1000; // Should be enough
      BYTE mountPoint[mountPointMaxLength];
      auto mountPointLength = mountPointMaxLength;
      while (RegEnumValue(serialcommKey,
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
            auto friendlyName((boost::format("%1% (%2%)") % reinterpret_cast<char*>(mountPoint) % serialPortName).str());
            (*serialPorts)[portName] = friendlyName;
         }

         // Increment index of key, and reset string length for next iteration
         valueIndex++;
         serialPortNameLength = serialPortNameMaxLength;
         mountPointLength = mountPointMaxLength;
      }

      RegCloseKey(serialcommKey);
      return serialPorts;
   }

   boost::system::error_code CPeripherals::flushSerialPort(boost::asio::serial_port& port)
   {
      return PurgeComm(port.native(),
                       PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR)
                ? boost::system::error_code()
                : boost::system::error_code(GetLastError(),
                                            boost::asio::error::get_system_category());
   }
} // namespace shared


