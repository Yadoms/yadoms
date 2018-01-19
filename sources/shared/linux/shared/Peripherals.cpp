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

boost::shared_ptr<CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
{
   boost::filesystem::path ttyDir("/sys/class/tty");

   auto serialPorts(boost::make_shared<SerialPortsMap>());

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

boost::system::error_code CPeripherals::flushSerialPort(boost::asio::serial_port& port)
{
   // Linux seems to have a bug on flushing serial port too early after open it.
   // See http://www.linuxforums.org/forum/miscellaneous/130106-tcflush-not-flushing-serial-port.html
   // As workaround, wait a bit.
   usleep(200000);
   
   return tcflush(port.native(), TCIOFLUSH) ?
      boost::system::error_code(errno, boost::asio::error::get_system_category()) :
      boost::system::error_code();
}

} // namespace shared
