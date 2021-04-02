#include "stdafx.h"
#include "Factory.h"
#include "SerialPortsManager.h"
#include "SerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsManager> CFactory::createSerialPortsManager(
         boost::shared_ptr<usb::IDevicesLister> usbDeviceListers,
         boost::shared_ptr<database::ISerialPortRequester> serialPortDatabaseRequester)
      {
         return boost::make_shared<CSerialPortsManager>(boost::make_shared<CSerialPortsLister>(usbDeviceListers),
                                                        serialPortDatabaseRequester,
            std::vector<std::string>() /*TODO récupérer cette liste de yadoms.ini*/,
            boost::shared_ptr<IManuallyDefinedPortLister>() /*TODO lister les);
      }
   } // namespace serial
} // namespace hardware
