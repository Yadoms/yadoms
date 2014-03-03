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
         (*serialPorts)[portName] = std::string(serialPortName);
      }

      // Increment index of key, and reset string length for next iteration
      valueIndex++;
      serialPortNameLength = serialPortNameMaxLength;
      mountPointLength = mountPointMaxLength;
   }

   RegCloseKey(serialcommKey);
   return serialPorts;
}

} // namespace shared