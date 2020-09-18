#include "stdafx.h"
#include "../SerialPortsListerFactory.h"
#include "SerialPortsLister.h"


namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsLister> CSerialPortsListerFactory::createSerialPortsLister()
      {
         return boost::make_shared<CSerialPortsLister>();
      }
   } // namespace serial
} // namespace hardware
