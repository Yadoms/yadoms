#include "stdafx.h"
#include "IOManager.h"
#include "urlManager.h"
#include <boost/regex.hpp>
#include <shared/Log.h>

CIOManager::CIOManager(std::vector<boost::shared_ptr<equipments::IEquipment> >& deviceList,
                       std::vector<boost::shared_ptr<equipments::IEquipment> >& masterDeviceList)
{
   m_deviceManager = deviceList;
   m_masterDeviceManager = masterDeviceList;
}

void CIOManager::addEquipment(boost::shared_ptr<equipments::IEquipment> equipment)
{
   if (equipment->isMasterDevice())
      m_deviceManager.push_back(equipment);
   else
   { 
      // TODO : Create subdevice 
   }
}

void CIOManager::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved)
{
   for (unsigned char counter = 0; counter < m_deviceManager.size(); ++counter)
   {
      // Deletion from the list of the device
      if (m_deviceManager[counter]->getDeviceName() == deviceRemoved)
      {
         m_deviceManager.erase(m_deviceManager.begin()+counter);
      }
   }
}

int CIOManager::getMasterEquipment()
{
   return m_deviceManager.size();
}

void CIOManager::readAllDevices(boost::shared_ptr<yApi::IYPluginApi> api, 
                                const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                bool forceHistorization)
{
   for (unsigned char counter = 0; counter < m_deviceManager.size(); ++counter)
   {
      m_deviceManager[counter]->updateFromDevice(api, pluginConfiguration, forceHistorization);
   }
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   shared::CDataContainer parameters;
   std::string keywordName = command->getKeyword();
   std::string deviceName = command->getDevice();
   std::string commandSelected;

   YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command) ;

   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorDevice;

   for (iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice)
   {
      if (deviceName == (*iteratorDevice)->getDeviceName())
      {
         //shared::CDataContainer results = urlManager::setRelayState(m_socketAddress, (*iteratorExtension)->buildMessageToDevice(api, parameters, command));
         return;
      }
   }
   /*
   const auto& deviceDetails = api->getDeviceDetails(command->getDevice());
   auto deviceType = deviceDetails.get<std::string>("type");

   for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
   {
      if (deviceType == (*iteratorExtension)->getDeviceType())
      {
         if (m_isPasswordActivated)
            parameters.set("key", m_password);

		 shared::CDataContainer results = urlManager::sendCommand(m_socketAddress, (*iteratorExtension)->buildMessageToDevice(api, parameters, command));

         // If the answer is a success, we historize the data
         if (results.containsValue("Success"))
            (*iteratorExtension)->historizePendingCommand(api, command);
         else
         {
            YADOMS_LOG(error) << "Command is not executed by the WES" ;
            
            // if an error is return, we reset the pending operation
            (*iteratorExtension)->resetPendingCommand();
         }
      }
   }*/
}

void CIOManager::readIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                  const std::string& type,
                                  bool forceHistorization)
{
/*
   shared::CDataContainer parameters;
   shared::CDataContainer results;

   // add the password if activated
   if (m_isPasswordActivated)
      parameters.set("key", m_password);

   parameters.set("Get", type);

   results = urlManager::sendCommand( m_socketAddress, parameters);

   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension;

   for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
      (*iteratorExtension)->updateFromDevice(type, api, results, forceHistorization);
*/
}

void CIOManager::OnDeviceConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const std::string &deviceName,
                                             const shared::CDataContainer& newConfiguration)
{
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorDevice;

   for (iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice)
   {
      if (deviceName == (*iteratorDevice)->getDeviceName())
      {
         // delete it from the master device if it's an extension
         if (!(*iteratorDevice)->isMasterDevice())
         {
            //TODO : Delete the extension from the master
            (*iteratorDevice)->updateConfiguration(api, newConfiguration);
         }

         // Delete the device
         //m_deviceManager.erase(iteratorDevice); // TODO : Check the good working
      }
   }
}

shared::CDataContainer CIOManager::bindMasterDevice()
{
   shared::CDataContainer ev;
   int counter = 0;
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorMasterDevice;

   for (iteratorMasterDevice = m_masterDeviceManager.begin(); iteratorMasterDevice != m_masterDeviceManager.end(); ++iteratorMasterDevice)
   {
      ev.set(boost::lexical_cast<std::string>(counter + 1), (*iteratorMasterDevice)->getDeviceName());
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("values", ev);
   en.set("defaultValue", "1");

   return en;
}

CIOManager::~CIOManager()
{}