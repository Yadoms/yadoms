#include "stdafx.h"

#include "../../shared/Exceptions/NotImplementedException.hpp"
#include "../../shared/Log.h"
#include "Peripherals.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <IOKit/serial/IOSerialKeys.h>

CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getSerialPorts()
{
	std::vector<std::string> SerialPortPaths;
	io_iterator_t matchingServices;
	mach_port_t         masterPort;
    CFMutableDictionaryRef  classesToMatch;
	io_object_t     modemService;
	char deviceFilePath[512];
	char deviceFriendly[1024];
    if (KERN_SUCCESS != IOMasterPort(MACH_PORT_NULL, &masterPort))
    {
        DBG("SerialPort::getSerialPortPaths : IOMasterPort failed");
		return SerialPortPaths;
    }
    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch == NULL)
	{
		DBG("SerialPort::getSerialPortPaths : IOServiceMatching failed");
		return SerialPortPaths;
	}
	CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));    
	if (KERN_SUCCESS != IOServiceGetMatchingServices(masterPort, classesToMatch, &matchingServices))
	{
		DBG("SerialPort::getSerialPortPaths : IOServiceGetMatchingServices failed");
		return SerialPortPaths;
	}
	while ((modemService = IOIteratorNext(matchingServices)))
	{
		CFTypeRef   deviceFilePathAsCFString;
		CFTypeRef   deviceFriendlyAsCFString;
		deviceFilePathAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIODialinDeviceKey), kCFAllocatorDefault, 0);
		deviceFriendlyAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIOTTYDeviceKey), kCFAllocatorDefault, 0);
		if(deviceFilePathAsCFString)
		{
			if(CFStringGetCString((const __CFString*)deviceFilePathAsCFString, deviceFilePath, 512, kCFStringEncodingASCII)
			&& CFStringGetCString((const __CFString*)deviceFriendlyAsCFString, deviceFriendly, 1024, kCFStringEncodingASCII) )
         SerialPortPaths.push_back(std::string(deviceFriendly));
				//SerialPortPaths.set(deviceFriendly, deviceFilePath);
			CFRelease(deviceFilePathAsCFString);
			CFRelease(deviceFriendlyAsCFString);
		}
	}
	IOObjectRelease(modemService);
	return SerialPortPaths;

}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUnusedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   // Utiliser "throw CNotSupportedException();" si une de ces fonctions ne peut �tre support�e par la plateforme
   NOT_IMPLEMENTED;
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUsedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/8632558/macos-programmatically-finding-serial-ports
   // Utiliser "throw CNotSupportedException();" si une de ces fonctions ne peut �tre support�e par la plateforme
   NOT_IMPLEMENTED;
}
