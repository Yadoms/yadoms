#pragma once
#include "ISerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsListerFactory
      {
      public:
         virtual ~CSerialPortsListerFactory() = default;

         static boost::shared_ptr<ISerialPortsLister> createSerialPortsLister();
      };
   } // namespace serial
} // namespace hardware
