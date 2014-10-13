#include "stdafx.h"

#include "../../shared/exception/NotSupported.hpp"
#include "../../shared/Log.h"
#include "Peripherals.h"

#include <termios.h>

namespace shared
{

CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
{
   boost::filesystem::path ttyDir("/sys/class/tty");

   boost::shared_ptr<SerialPortsMap> serialPorts(new SerialPortsMap);

   if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
   {
      boost::filesystem::directory_iterator endIter;
      for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
      {
         if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
         {
            std::string friendlyName((*dirIter).path().leaf().string());            // friendlyName is something like "tty0"
            std::string portName((boost::format("/dev/%1%") % friendlyName).str()); // portName is "/dev/tty0"
            (*serialPorts)[portName]=friendlyName;
         }
      }
   }

   return serialPorts;
}

} // namespace shared
