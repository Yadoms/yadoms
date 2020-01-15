#include "stdafx.h"
#include "SerialPortHelper.h"
#include <termios.h>

namespace shared
{
   boost::system::error_code CSerialPortHelper::flushSerialPort(boost::asio::serial_port& port)
   {
      return tcflush(port.native_handle(), TCIOFLUSH) ?
         boost::system::error_code(errno, boost::asio::error::get_system_category()) :
         boost::system::error_code();
   }
} // namespace shared
