#include "stdafx.h"
#include "../PlatformSpecificFactory.h"
#include "SerialPortsLister.h"


namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsLister> CPlatformSpecificFactory::createSerialPortsLister()
      {
         return boost::make_shared<CSerialPortsLister>();
      }
   } // namespace serial
} // namespace hardware
