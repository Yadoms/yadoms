#include "stdafx.h"
#include "SerialPortHelper.h"

namespace tools {

   std::string CSerialPortHelper::formatSerialPort(const std::string & serialPort)
   {
      return (boost::format("\\\\.\\%1%") % serialPort).str();
   }


} //namespace tools
