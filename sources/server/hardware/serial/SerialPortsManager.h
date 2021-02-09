#pragma once
#include "ISerialPortsManager.h"
#include "ISerialPortsLister.h"
#include "database/ISerialPortRequester.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsManager : public ISerialPortsManager
      {
      public:
         explicit CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister,
                                      boost::shared_ptr<database::ISerialPortRequester> serialPortDatabaseRequester);
         virtual ~CSerialPortsManager() = default;

         // ISerialPortsManager Implementation
         std::vector<boost::shared_ptr<ISerialPort>> getSerialPorts() override;
         // [END] ISerialPortsManager Implementation

      private:
         static database::entities::CSerialPort fromHardwareSerialPort(const ISerialPort& hardwareSerialPort);
         void updateDatabase(const std::vector<boost::shared_ptr<ISerialPort>>& hardwareSerialPorts) const;
         void updatePhysicalPortInDatabase(const boost::shared_ptr<ISerialPort>& hardwareSerialPort) const;
         void updateUsbAdapterSerialPortInDatabase(const boost::shared_ptr<ISerialPort>& hardwareSerialPort) const;

         boost::shared_ptr<ISerialPortsLister> m_serialPortLister;
         boost::shared_ptr<database::ISerialPortRequester> m_serialPortDatabaseRequester;
      };
   } // namespace serial
} // namespace hardware
