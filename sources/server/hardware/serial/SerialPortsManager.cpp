#include "stdafx.h"
#include "SerialPortsManager.h"

#include <utility>
#include "PlatformSpecificFactory.h"
#include "SerialPort.h"

namespace hardware
{
   namespace serial
   {
      CSerialPortsManager::CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister,
                                               boost::shared_ptr<database::ISerialPortRequester> serialPortDatabaseRequester)
         : m_serialPortLister(std::move(serialPortLister)),
           m_serialPortDatabaseRequester(std::move(serialPortDatabaseRequester))
      {
      }

      std::vector<boost::shared_ptr<ISerialPort>> CSerialPortsManager::getSerialPorts() //TODO utile ?
      {
         //TODO éventuellement ajouter un filtre si déjà appelé récemment

         auto hardwareSerialPorts = m_serialPortLister->listSerialPorts();
         updateDatabase(hardwareSerialPorts);

         for (const auto& hardwareSerialPort : hardwareSerialPorts)
         {
            const auto databaseSerialPorts =
               m_serialPortDatabaseRequester->getSerialPortsFromLastKnownSerialPortPath(hardwareSerialPort->lastKnownSerialPortPath());
            if (databaseSerialPorts.empty())
            {
               YADOMS_LOG(error) << "No serial port from same path in database";
               continue;
            }
            if (databaseSerialPorts.size() != 1)
               YADOMS_LOG(error) << "More than one serial port from same path in database, use first";

            // Need to add database values to hardware serial port
            hardwareSerialPort->setId(databaseSerialPorts[0]->Id());
            hardwareSerialPort->setAdapterKind(databaseSerialPorts[0]->AdapterKind());
            hardwareSerialPort->setAdapterParameters(databaseSerialPorts[0]->AdapterParameters());
         }

         return hardwareSerialPorts;
      }

      void CSerialPortsManager::updateDatabase(const std::vector<boost::shared_ptr<ISerialPort>>& hardwareSerialPorts) const
      {
         // Find if exist in database
         for (const auto& hardwareSerialPort : hardwareSerialPorts)
         {
            switch (hardwareSerialPort->adapterKind())
            {
            case database::entities::ESerialPortAdapterKind::kPhysicalValue:
               updatePhysicalPortInDatabase(hardwareSerialPort);
               break;
            case database::entities::ESerialPortAdapterKind::kUsbAdapterValue:
               updateUsbAdapterSerialPortInDatabase(hardwareSerialPort);
               //TODO
               break;
            default:
               throw std::runtime_error("Invalid serial port kind : " + std::to_string(hardwareSerialPort->adapterKind()));
            }
         }
      }

      void CSerialPortsManager::updatePhysicalPortInDatabase(const boost::shared_ptr<ISerialPort>& hardwareSerialPort) const
      {
         const auto serialPortsInDatabase = m_serialPortDatabaseRequester->getSerialPortsFromLastKnownSerialPortPath(
            hardwareSerialPort->lastKnownSerialPortPath());

         if (serialPortsInDatabase.empty())
         {
            // New serial port, add it in database
            m_serialPortDatabaseRequester->addSerialPort(fromHardwareSerialPort(*hardwareSerialPort));
         }
         else if (serialPortsInDatabase.size() == 1)
         {
            // Already known serial port, nothing to do
            // TODO traiter le cas où serialPortsInDatabase->adapterKind() != hardwareSerialPort->adapterKind()
         }
         else
         {
            // Several serial ports with same path, should not occur
            YADOMS_LOG(warning) << "Several serial ports with path " << hardwareSerialPort->lastKnownSerialPortPath() <<
               " were found, it's not normal. Cleanup database keeping only the first one " << serialPortsInDatabase[0]->
               LastKnownSerialPortPath();
            for (auto serialPortInDatabaseIt = serialPortsInDatabase.begin();
                 serialPortInDatabaseIt != serialPortsInDatabase.end();
                 ++serialPortInDatabaseIt)
            {
               if (serialPortInDatabaseIt == serialPortsInDatabase.begin())
                  continue; // Keep first
               m_serialPortDatabaseRequester->removeSerialPort((*serialPortInDatabaseIt)->Id());
            }
            // TODO traiter le cas où serialPortsInDatabase->adapterKind() != hardwareSerialPort->adapterKind()
         }
      }

      void CSerialPortsManager::updateUsbAdapterSerialPortInDatabase(const boost::shared_ptr<ISerialPort>& hardwareSerialPort) const
      {
         const auto serialPortsInDatabase = m_serialPortDatabaseRequester->getSerialPortsFromLastKnownSerialPortPath(
            hardwareSerialPort->lastKnownSerialPortPath());

         if (std::find_if(
               serialPortsInDatabase.begin(),
               serialPortsInDatabase.end(),
               [hardwareSerialPort](const auto& serialPortInDatabase)
               {
                  // Same connection path + same USB parameters ==> Port already known and didn't move
                  return serialPortInDatabase->LastKnownSerialPortPath() == hardwareSerialPort->lastKnownSerialPortPath()
                     && *serialPortInDatabase->AdapterParameters() == *hardwareSerialPort->adapterParameters();
               })
            != serialPortsInDatabase.end())
         {
            // Port didn't move
            // ==> Nothing to do
            return;
         }

         if (std::find_if(
               serialPortsInDatabase.begin(),
               serialPortsInDatabase.end(),
               [hardwareSerialPort](const auto& serialPortInDatabase)
               {
                  // Same connection path ==> Port moved
                  return serialPortInDatabase->LastKnownSerialPortPath() == hardwareSerialPort->lastKnownSerialPortPath();
               })
            != serialPortsInDatabase.end())
         {
            // Port moved
            // ==> Update port
            m_serialPortDatabaseRequester->updateSerialPort(hardwareSerialPort->lastKnownSerialPortPath(), fromHardwareSerialPort(*hardwareSerialPort));
            return;
         }

         // New Port
         // ==> Add port
         m_serialPortDatabaseRequester->addSerialPort(fromHardwareSerialPort(*hardwareSerialPort));
      }

      database::entities::CSerialPort CSerialPortsManager::fromHardwareSerialPort(const ISerialPort& hardwareSerialPort)
      {
         database::entities::CSerialPort databaseSerialPort;

         databaseSerialPort.AdapterKind = hardwareSerialPort.adapterKind();
         databaseSerialPort.LastKnownSerialPortPath = hardwareSerialPort.lastKnownSerialPortPath();
         if (hardwareSerialPort.adapterParameters())
            databaseSerialPort.AdapterParameters = hardwareSerialPort.adapterParameters()->copy();

         return databaseSerialPort;
      }
   } // namespace serial
} // namespace hardware
