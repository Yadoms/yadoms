#pragma once

namespace tools {

   //--------------------------------------------------------------
   /// \brief     Class used to provide platform specific serial port formatting
   //--------------------------------------------------------------
   class CSerialPortHelper
   {
   public:

      static std::string formatSerialPort(const std::string & serialPort);
   };


} //namespace tools


