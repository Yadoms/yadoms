#include "stdafx.h"
#include "SerialPortHelper.h"
#include <termios.h>

namespace shared
{
   boost::system::error_code CSerialPortHelper::flushSerialPort(boost::asio::serial_port& port)
   {
      // Linux seems to have a bug on flushing serial port too early after open it.
      // See http://www.linuxforums.org/forum/miscellaneous/130106-tcflush-not-flushing-serial-port.html
      // As workaround, wait a bit.
      usleep(200000);

      return tcflush(port.native_handle(), TCIOFLUSH) ?
         boost::system::error_code(errno, boost::asio::error::get_system_category()) :
         boost::system::error_code();
   }
} // namespace shared
