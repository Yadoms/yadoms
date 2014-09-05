#include "stdafx.h"

#include "DynamicLibrary.hpp"
#include "../../shared/exception/Exception.hpp"
#include "../../shared/Log.h"
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

const boost::shared_ptr<const CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
{
   boost::shared_ptr<SerialPortsMap> serialPorts(new SerialPortsMap);

   HKEY serialcommKey;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &serialcommKey) != ERROR_SUCCESS)
   {
      // Unable to access registry ==> HARDWARE\\DEVICEMAP\\SERIALCOMM key does not exist ==> no serial port on this machine
      return serialPorts;
   }

   DWORD valueIndex = 0;
   static const DWORD serialPortNameMaxLength = 1000;    // Should be enough
   TCHAR serialPortName[serialPortNameMaxLength];
   DWORD serialPortNameLength = serialPortNameMaxLength;
   DWORD dataType = 0;
   static const DWORD mountPointMaxLength = 1000;    // Should be enough
   BYTE mountPoint[mountPointMaxLength];
   DWORD mountPointLength = mountPointMaxLength;
   while (RegEnumValue(serialcommKey, valueIndex, serialPortName, &serialPortNameLength, NULL, &dataType, mountPoint, &mountPointLength) == ERROR_SUCCESS)
   {
      if (dataType == REG_SZ)
      {
         std::string portName((char*)mountPoint);

         //ex
         //portname : COM1
         //serialPortName : \Device\Serial0
         //=> friendlyName : COM1 (\Device\Serial0)
         std::string sFriendlyName((boost::format("%1% (%2%)") % portName % serialPortName).str()); 
         (*serialPorts)[portName] = sFriendlyName;
      }

      // Increment index of key, and reset string length for next iteration
      valueIndex++;
      serialPortNameLength = serialPortNameMaxLength;
      mountPointLength = mountPointMaxLength;
   }

   RegCloseKey(serialcommKey);
   return serialPorts;
}

void CPeripherals::flushSerialPort(boost::asio::serial_port& sp)
{
   ::PurgeComm(sp.native(), PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}

} // namespace shared