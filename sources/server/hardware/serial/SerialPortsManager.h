#pragma once
#include "ISerialPortsManager.h"
#include "ISerialPortsLister.h"
#include "hardware/usb/IDevicesLister.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsManager : public ISerialPortsManager
      {
      public:
         explicit CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister,
                                      boost::shared_ptr<usb::IDevicesLister> usbDeviceListers);
         virtual ~CSerialPortsManager() = default;

         // ISerialPortsManager Implementation
         std::vector<boost::shared_ptr<database::entities::CSerialPort>> listSerialPorts() override;
         // [END] ISerialPortsManager Implementation

      private:
         void updateAdapterKinds(std::vector<boost::shared_ptr<database::entities::CSerialPort>>& detectedSerialPorts) const;

         boost::shared_ptr<ISerialPortsLister> m_serialPortLister;
         boost::shared_ptr<usb::IDevicesLister> m_usbDeviceListers;
      };
   } // namespace serial
} // namespace hardware
