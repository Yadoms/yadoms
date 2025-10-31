#include "stdafx.h"
#include "SerialPortsLister.h"
#include <windows.h>

using namespace hardware::serial;

boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSerialPorts()
{
	auto serialPorts = boost::make_shared<SerialPortsMap>();

	HKEY serialCommKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					 L"HARDWARE\\DEVICEMAP\\SERIALCOMM",
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
			std::wstring wMountPoint(reinterpret_cast<wchar_t*>(mountPoint));
			std::string portName(wMountPoint.begin(), wMountPoint.end());

			//ex
			//portname : COM1
			//serialPortName : \Device\Serial0
			//=> friendlyName : COM1 (\Device\Serial0)
			std::wstring wSerialPortName(reinterpret_cast<wchar_t*>(serialPortName));
			const auto friendlyName(
				(boost::format("%1% (%2%)") % portName % std::string(wSerialPortName.begin(), wSerialPortName.end())).str());

			// Assure compatibility with serial port numbers > 9
			portName.insert(0, "\\\\.\\");

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
