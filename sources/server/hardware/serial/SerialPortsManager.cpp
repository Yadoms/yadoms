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
         //TODO �ventuellement ajouter un filtre si d�j� appel� r�cemment

         //TODO g�rer la maj de la base de donn�es

         return m_serialPortLister->listSerialPorts();
      }
   } // namespace serial
} // namespace hardware
