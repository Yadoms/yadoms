#include "stdafx.h"
#include <shared/Log.h>
#include "SerialPortsLister.h"
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

namespace hardware
{
	namespace serial
	{
		std::vector<boost::shared_ptr<database::entities::CSerialPort>> CSerialPortsLister::listSerialPorts()
      {         
	      io_iterator_t matchingServices;
	      mach_port_t         masterPort;
         CFMutableDictionaryRef  classesToMatch;
	      io_object_t     modemService;
	      char deviceFilePath[512];
	      char deviceFriendly[1024];

         if (KERN_SUCCESS != IOMasterPort(MACH_PORT_NULL, &masterPort))
         {
            YADOMS_LOG(error) << "SerialPort::getSerialPortPaths : IOMasterPort failed";
		      return std::vector<boost::shared_ptr<database::entities::CSerialPort>>();
         }
         classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
         if (classesToMatch == NULL)
	      {
		      YADOMS_LOG(error) << "SerialPort::getSerialPortPaths : IOServiceMatching failed";
		      return std::vector<boost::shared_ptr<database::entities::CSerialPort>>();
	      }
	      CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));    
	      if (KERN_SUCCESS != IOServiceGetMatchingServices(masterPort, classesToMatch, &matchingServices))
	      {
		      YADOMS_LOG(error) << "SerialPort::getSerialPortPaths : IOServiceGetMatchingServices failed";
		      return std::vector<boost::shared_ptr<database::entities::CSerialPort>>();
	      }

			std::vector<boost::shared_ptr<database::entities::CSerialPort>> serialPorts;
	      while ((modemService = IOIteratorNext(matchingServices)))
	      {
		      CFTypeRef   deviceFilePathAsCFString;
		      CFTypeRef   deviceFriendlyAsCFString;
		      deviceFilePathAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIODialinDeviceKey), kCFAllocatorDefault, 0);
		      deviceFriendlyAsCFString = IORegistryEntryCreateCFProperty(modemService,CFSTR(kIOTTYDeviceKey), kCFAllocatorDefault, 0);
				if (deviceFilePathAsCFString)
				{
					if (CFStringGetCString((const __CFString*)deviceFilePathAsCFString, deviceFilePath, 512, kCFStringEncodingASCII)
						&& CFStringGetCString((const __CFString*)deviceFriendlyAsCFString, deviceFriendly, 1024, kCFStringEncodingASCII))
					{
						auto port = boost::make_shared<database::entities::CSerialPort>();
					   port->AdapterKind = database::entities::ESerialPortAdapterKind::kUnknown;
					   port->AdapterParameters = std::string(deviceFriendly);
					   port->LastKnownSerialPortPath = std::string(deviceFilePath);
						serialPorts.emplace_back(port);
				   }

			      CFRelease(deviceFilePathAsCFString);
			      CFRelease(deviceFriendlyAsCFString);
		      }
	      }
	      IOObjectRelease(modemService);
	      return serialPorts;
      }
	} // namespace serial
} // namespace hardware
