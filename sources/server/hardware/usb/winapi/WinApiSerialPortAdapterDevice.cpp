#include "stdafx.h"
#include "WinApiSerialPortAdapterDevice.h"

namespace hardware
{
   namespace usb
   {
      CWinApiSerialPortAdapterDevice::CWinApiSerialPortAdapterDevice(boost::shared_ptr<const IDevice> usbDevice)
      {
         if (!isSerialPortAdapter(usbDevice))
            throw std::runtime_error("CWinApiSerialPortAdapterDevice : provided usb device \"" + usbDevice->yadomsFriendlyName() + "\" is not a serial port adapter");
      }

      std::string CWinApiSerialPortAdapterDevice::connectionPath() const
      {
         //TODO
         return std::string();
      }

      bool CWinApiSerialPortAdapterDevice::isSerialPortAdapter(boost::shared_ptr<const IDevice> device)
      {
         //TODO
         return false;
      }
   } // namespace usb
} // namespace hardware
