#pragma once
#include "hardware/serial/ISerialPortsLister.h"
#include "hardware/usb/IDevicesLister.h"
#include "hardware/serial/SerialPort.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsLister : public ISerialPortsLister //TODO renommer en CWinapiSerialPortsLister ?
      {
      public:
         explicit CSerialPortsLister(boost::shared_ptr<usb::IDevicesLister> usbDeviceListers);
         virtual ~CSerialPortsLister() = default;

         // ISerialPortsLister Implementation
         std::vector<boost::shared_ptr<ISerialPort>> listSerialPorts() override;
         // [END] ISerialPortsLister Implementation

      private:
         std::vector<boost::shared_ptr<CSerialPort>> listAllSerialPorts() const;
         void updatePortsKind(std::vector<boost::shared_ptr<CSerialPort>>& detectedSerialPorts) const;
         std::string getSerialPortName(const std::string& serialPortConnectionPath) const;
         std::string getSerialPortNameFromUsbAdapter(const boost::shared_ptr<usb::IDevice>& device) const;

         boost::shared_ptr<usb::IDevicesLister> m_usbDeviceListers;
      };
   } // namespace serial
} // namespace hardware
