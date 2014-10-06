#include "stdafx.h"
#include "SerialPortHelper.h"

namespace tools {

   std::string CSerialPortHelper::formatSerialPort(const std::string & serialPort)
   {
      return (boost::format("/dev/%1%") % serialPort).str();
   }


} //namespace tools
