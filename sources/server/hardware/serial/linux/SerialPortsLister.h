#pragma once
#include "hardware/serial/ISerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsLister : public ISerialPortsLister
      {
      public:
         CSerialPortsLister() = default;
         virtual ~CSerialPortsLister() = default;

         // ISerialPortsLister Implementation
         std::vector<boost::shared_ptr<database::entities::CSerialPort>> listSerialPorts() override;
         // [END] ISerialPortsLister Implementation
      };
   } // namespace serial
} // namespace hardware
