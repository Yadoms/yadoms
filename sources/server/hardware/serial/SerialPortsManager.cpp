#include "stdafx.h"
#include "SerialPortsManager.h"

namespace hardware
{
   namespace serial
   {
      CSerialPortsManager::CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister)
         : m_serialPortLister(serialPortLister)
      {
      }

      std::vector<boost::shared_ptr<database::entities::CSerialPort>> CSerialPortsManager::listSerialPorts()
      {
         //TODO éventuellement ajouter un filtre si déjà appelé récemment

         //TODO gérer la maj de la base de données

         return m_serialPortLister->listSerialPorts();
      }
   } // namespace serial
} // namespace hardware
