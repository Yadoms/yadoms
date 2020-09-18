#pragma once
#include "ISerialPortsManager.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsManagerFactory
      {
      public:
         CSerialPortsManagerFactory() = delete;
         virtual ~CSerialPortsManagerFactory() = delete;

         static boost::shared_ptr<ISerialPortsManager> createSerialPortsManager();
      };
   } // namespace serial
} // namespace hardware
