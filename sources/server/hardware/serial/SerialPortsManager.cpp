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
                                               boost::shared_ptr<database::ISerialPortRequester> serialPortDatabaseRequester,
                                               std::vector<std::string> reserverdPorts,
                                               boost::shared_ptr<IManuallyDefinedPortLister> manuallyDefinedPortLister)
         : m_serialPortLister(std::move(serialPortLister)),
           m_serialPortDatabaseRequester(std::move(serialPortDatabaseRequester)),
           m_reserverdPorts(std::move(reserverdPorts)),
           m_manuallyDefinedPortLister(std::move(manuallyDefinedPortLister))
      {
      }

      bool CSerialPortsManager::isPortIn(const std::string& port,
                                         const std::vector<boost::shared_ptr<ISerialPort>>& in)
      {
         return std::find(
            in.begin(),
            in.end(),
            [&port](const auto& item)
            {
               return item->lastKnownSerialPortPath() == port;
            }) != in.end();
      }

      bool CSerialPortsManager::isPortIn(const std::string& port,
                                         const std::vector<std::string>& in)
      {
         return std::find(in.begin(), in.end(), port) != in.end();
      }

      std::vector<std::string> CSerialPortsManager::portListAsString(const std::vector<boost::shared_ptr<ISerialPort>>& in) const
      {
         std::vector<std::string> availablePortsAsStrings;
         std::transform(in.begin(),
                        in.end(),
                        std::back_inserter(availablePortsAsStrings),
                        [](const auto& p)
                        {
                           return p->lastKnownSerialPortPath();
                        });
         return availablePortsAsStrings;
      }

      std::vector<std::string> CSerialPortsManager::requestPort(int pluginInstanceId,
                                                                const boost::shared_ptr<shared::CDataContainer> filter,
                                                                const std::string& manuallyDefinedPort)
      {
         YADOMS_LOG(debug) << "Plugin instance #" << pluginInstanceId << " requests a serial port...";
         if (!manuallyDefinedPort.empty())
            YADOMS_LOG(debug) << "... with the manually defined port " << manuallyDefinedPort;

         auto availablePorts = m_serialPortLister->listSerialPorts();

         // System reserved ports are not available for plugins
         std::remove_if(availablePorts.begin(),
                        availablePorts.end(),
                        [this](const auto& availablePort)
                        {
                           return isPortIn(availablePort->lastKnownSerialPortPath(),
                                           m_reserverdPorts);
                        });

         //TODO vérifier qu'il reste des ports dispos

         // Manage port forced by plugin instance configuration
         const auto manuallyDefinedPort = m_pluginManager->getManuallyDefinedSerialPort(pluginInstanceId);
         if (!manuallyDefinedPort.empty())
         {
            if (!isPortIn(manuallyDefinedPort, availablePorts))
            {
               YADOMS_LOG(error) << "Manually defined port " << manuallyDefinedPort << " is not available (reserved by system, see yadoms.ini)";
               YADOMS_LOG(error) << "No serial port available for plugin instance #" << pluginInstanceId;
               return std::vector<std::string>();
            }
            YADOMS_LOG(information) << "Return serial port " << manuallyDefinedPort << " for plugin instance #" << pluginInstanceId;
            return std::list<std::string>({manuallyDefinedPort});
         }

         // Manually defined ports (from all plugin instances) are not available
         const auto manuallyDefinedPorts = m_pluginManager->getAllManuallyDefinedSerialPorts();
         std::remove_if(availablePorts.begin(),
                        availablePorts.end(),
                        [this, &manuallyDefinedPorts](const auto& availablePort)
                        {
                           return isPortIn(availablePort->lastKnownSerialPortPath(),
                                           manuallyDefinedPorts);
                        });

         //TODO vérifier qu'il reste des ports dispos

         // Search for filter
         if (!filter)
         {
            //TODO modifier ce fonctionnel pour placer la requête en file d'attente
            //TODO L'idée est tous les plugins qui demandent un port série avec filtre soient
            //TODO servis en priorité, et seuls les plugins ne fournissant pas de filtre seront retardés

            // No filter defined, return all available ports. Plugin must now try all these ports, then call reservePort.

            const auto availablePortsAsStrings = portListAsString(availablePorts);
            YADOMS_LOG(information) << "No serial port filter defined for plugin instance #" << pluginInstanceId << ", return all available ports : ";
            for (const auto& ap : availablePortsAsStrings)
               YADOMS_LOG(information) << "  - " << ap;
            return availablePortsAsStrings;
         }

         // A filter is defined, try to auto-detect port
         std::remove_if(availablePorts.begin(),
                        availablePorts.end(),
                        [this, &manuallyDefinedPorts](const auto& availablePort)
                        {
                           return matchFilter(availablePort,
                                              filter);
                        });

         // No more need serial port detailed information
         const auto availablePortsAsStrings = portListAsString(availablePorts);

         if (availablePortsAsStrings.empty())
         {
            YADOMS_LOG(error) << "No compatible serial port was found for plugin instance #" << pluginInstanceId;
            return std::vector<std::string>();
         }

         if (availablePortsAsStrings.size() == 1)
         {
            YADOMS_LOG(information) << "Serial port found for plugin instance #" << pluginInstanceId << " : " << availablePortsAsStrings[0];
            return availablePortsAsStrings;
         }

         // Despite filter, several ports still match
         // Request database to build a list beginning by the last port used
         const auto lastSerialPortUsed = m_serialPortDatabaseRequester->getLastSerialPortUsedForPlugin(pluginInstanceId);
         if (isPortIn(lastSerialPortUsed, availablePortsAsStrings) && lastSerialPortUsed != availablePortsAsStrings.front())
            moveToFirst(availablePortsAsStrings, lastSerialPortUsed);

         //TODO mettre en file d'attente ? Des fois que y'ait plus le choix plus tard (un seul port restant)
         YADOMS_LOG(information) << "Several serial ports found for plugin instance #" << pluginInstanceId << " : ";
         for (const auto& ap : availablePortsAsStrings)
            YADOMS_LOG(information) << "  - " << ap;
         return availablePortsAsStrings;
      }

      void CSerialPortsManager::releasePort(int pluginId)
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
            m_serialPortDatabaseRequester->updateSerialPort(hardwareSerialPort->lastKnownSerialPortPath(),
                                                            fromHardwareSerialPort(*hardwareSerialPort));
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
