#pragma once
#include "ISerialPort.h"

namespace hardware
{
   namespace serial
   {
      class ISerialPortsManager
      {
      public:
         virtual ~ISerialPortsManager() = default;

         //--------------------------------------------------------------
         /// \brief			List machine serial ports (all)
         /// \return       The serial ports names (keys are real name, values are common name)
         //--------------------------------------------------------------
         virtual std::vector<boost::shared_ptr<ISerialPort>> getSerialPorts() = 0;
      };
   } // namespace serial
} // namespace hardware
