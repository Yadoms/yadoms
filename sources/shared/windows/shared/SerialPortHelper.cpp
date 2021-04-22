#include "stdafx.h"
#include "SerialPortHelper.h"

namespace shared
{
   boost::system::error_code CSerialPortHelper::flushSerialPort(boost::asio::serial_port& port)
   {
      return PurgeComm(port.native_handle(),
         PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR)
         ? boost::system::error_code()
         : boost::system::error_code(GetLastError(),
            boost::asio::error::get_system_category());
   }
} // namespace shared
