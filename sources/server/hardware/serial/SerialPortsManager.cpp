#include "stdafx.h"
#include "SerialPortsManager.h"
#include "PlatformSpecificFactory.h"

namespace hardware
{
   namespace serial
   {
      CSerialPortsManager::CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister)
         : m_serialPortLister(serialPortLister)
      {
      }

      std::vector<boost::shared_ptr<database::entities::CSerialPort>> CSerialPortsManager::listSerialPorts() //TODO utile ?
      {
         //TODO éventuellement ajouter un filtre si déjà appelé récemment

         auto serialPorts = m_serialPortLister->listSerialPorts();

         //TODO gérer la maj de la base de données

         return serialPorts;
      }
   } // namespace serial
} // namespace hardware
