#include "stdafx.h"

#include "../../shared/exception/NotImplemented.hpp"
#include "../../shared/Log.h"
#include "Peripherals.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>
#include <termios.h>

namespace shared
{

CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

boost::shared_ptr<CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
{
   boost::shared_ptr<CPeripherals::SerialPortsMap> serialPorts(new CPeripherals::SerialPortsMap);
   
	io_iterator_t matchingServices;
	mach_port_t         masterPort;
    CFMutableDictionaryRef  classesToMatch;
	io_object_t     modemService;
	char deviceFilePath[512];
	char deviceFriendly[1024];
    if (KERN_SUCCESS != IOMasterPort(MACH_PORT_NULL, &masterPort))
    {
        YADOMS_LOG(error) << "SerialPort::getSerialPortPaths : IOMasterPort failed";
		return serialPorts;
    }
    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch == NULL)
	{
		YADOMS_LOG(error) << "SerialPort::getSerialPortPaths : IOServiceMatching failed";
		return serialPorts;
	}
	CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));    
	if (KERN_SUCCESS != IOServiceGetMatchingServices(masterPort, classesToMatch, &matchingServices))
	{
		YADOMS_LOG(error) << "SerialPort::getSerialPortPaths : IOServiceGetMatchingServices failed";
		return serialPorts;
	}
	while ((modemService = IOIteratorNext(matchingServices)))
	{
		CFTypeRef   deviceFilePathAsCFString;
		CFTypeRef   deviceFriendlyAsCFString;
		deviceFilePathAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIODialinDeviceKey), kCFAllocatorDefault, 0);
		deviceFriendlyAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIOTTYDeviceKey), kCFAllocatorDefault, 0);
		if(deviceFilePathAsCFString)
		{
            //TODO: si type retour passe en map
			if(CFStringGetCString((const __CFString*)deviceFilePathAsCFString, deviceFilePath, 512, kCFStringEncodingASCII)
			&& CFStringGetCString((const __CFString*)deviceFriendlyAsCFString, deviceFriendly, 1024, kCFStringEncodingASCII) )
                serialPorts->insert(std::make_pair(std::string(deviceFilePath), std::string(deviceFriendly)));
				//SerialPortPaths.set(deviceFriendly, deviceFilePath);
			CFRelease(deviceFilePathAsCFString);
			CFRelease(deviceFriendlyAsCFString);
		}
	}
	IOObjectRelease(modemService);
	return serialPorts;
}

boost::system::error_code CPeripherals::flushSerialPort(boost::asio::serial_port& port)
{
   return tcflush(port.native(), TCIOFLUSH) ?
      boost::system::error_code(errno, boost::asio::error::get_system_category()) :
      boost::system::error_code();
}

} // namespace shared
