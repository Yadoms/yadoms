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
         //TODO �ventuellement ajouter un filtre si d�j� appel� r�cemment

         auto serialPorts = m_serialPortLister->listSerialPorts();

         //TODO g�rer la maj de la base de donn�es

         return serialPorts;
      }
   } // namespace serial
} // namespace hardware
