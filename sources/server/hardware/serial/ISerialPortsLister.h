#pragma once
#include "ISerialPort.h"

namespace hardware
{
   namespace serial
   {
      class ISerialPortsLister
      {
      public:
         virtual ~ISerialPortsLister() = default;

         //--------------------------------------------------------------
         /// \brief			List all machine serial ports
         /// \return       The serial ports names
         //--------------------------------------------------------------
         virtual std::vector<boost::shared_ptr<ISerialPort>> listSerialPorts() = 0;
      };
   } // namespace serial
} // namespace hardware
