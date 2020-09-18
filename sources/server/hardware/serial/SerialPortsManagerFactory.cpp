#include "stdafx.h"
#include "SerialPortsManagerFactory.h"
#include "SerialPortsManager.h"
#include "SerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsManager> CSerialPortsManagerFactory::createSerialPortsManager()
      {
         return boost::make_shared<CSerialPortsManager>(boost::make_shared<CSerialPortsLister>());
      }
   } // namespace serial
} // namespace hardware
