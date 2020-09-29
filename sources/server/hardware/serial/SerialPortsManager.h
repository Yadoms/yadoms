#pragma once
#include "ISerialPortsManager.h"
#include "ISerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsManager : public ISerialPortsManager
      {
      public:
         explicit CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister);
         virtual ~CSerialPortsManager() = default;

         // ISerialPortsManager Implementation
         std::vector<boost::shared_ptr<database::entities::CSerialPort>> listSerialPorts() override;
         // [END] ISerialPortsManager Implementation

      private:
         boost::shared_ptr<ISerialPortsLister> m_serialPortLister;
      };
   } // namespace serial
} // namespace hardware
