#include "stdafx.h"
#include "IOManager.h"
#include "urlManager.h"
#include "WESFactory.h"
#include <boost/regex.hpp>
#include <shared/Log.h>
#include <algorithm>

CIOManager::CIOManager(std::vector<boost::shared_ptr<equipments::IEquipment>>& deviceList,
                       std::vector<std::string>& deviceToRetry):
   m_deviceManager(deviceList),
   m_deviceToRetry(deviceToRetry)
{
}

void CIOManager::addEquipment(boost::shared_ptr<equipments::IEquipment> equipment)
{
   m_deviceManager.push_back(equipment);
}

void CIOManager::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceRemoved)
{
   for (unsigned char counter = 0; counter < m_deviceManager.size(); ++counter)
   {
      // Deletion from the list of the device
      if (m_deviceManager[counter]->getDeviceName() == deviceRemoved)
      {
         m_deviceManager[counter]->remove(api);
         m_deviceManager.erase(m_deviceManager.begin() + counter);
         return;
      }
   }
}

int CIOManager::getMasterEquipment() const
{
   return m_deviceManager.size();
}

int CIOManager::getWaitingEquipment() const
{
   return m_deviceToRetry.size();
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

void CIOManager::tryMissingEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const boost::shared_ptr<IWESConfiguration> pluginConfiguration)
{
   CWESFactory factory;

   auto deviceId = m_deviceToRetry.begin();
   while (deviceId != m_deviceToRetry.end()){
	   try {
		   m_deviceManager.push_back(factory.createEquipment(api, *deviceId, pluginConfiguration));
		   deviceId = m_deviceToRetry.erase(deviceId); // Remove the device from the device to retry if all is running well
	   }
	   catch (std::exception&)
	   {
		   deviceId++;
	   }
   }
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   const auto deviceName = command->getDevice();

   YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command) ;

   for (std::vector<boost::shared_ptr<equipments::IEquipment>>::const_iterator iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice){
      if (deviceName == (*iteratorDevice)->getDeviceName()){
         (*iteratorDevice)->sendCommand(api, command->getKeyword(), command);
         return;
      }
   }
}

void CIOManager::OnDeviceConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const std::string& deviceName,
                                             const shared::CDataContainer& newConfiguration,
                                             const int refreshEvent)
{
   for (std::vector<boost::shared_ptr<equipments::IEquipment>>::const_iterator iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice){
      if (deviceName == (*iteratorDevice)->getDeviceName()){
         (*iteratorDevice)->updateConfiguration(api, newConfiguration, refreshEvent);
      }
   }
}

shared::CDataContainer CIOManager::bindMasterDevice()
{
   shared::CDataContainer ev;
   const auto counter = 0;

   for (std::vector<boost::shared_ptr<equipments::IEquipment>>::const_iterator iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice){
      ev.set(boost::lexical_cast<std::string>(counter + 1), (*iteratorDevice)->getDeviceName());
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("values", ev);
   en.set("defaultValue", "1");

   return en;
}

std::vector<specificHistorizers::EWESdeviceStatus> CIOManager::getMasterdeviceStates()
{
   std::vector<specificHistorizers::EWESdeviceStatus> devicesStatus;

   for (std::vector<boost::shared_ptr<equipments::IEquipment>>::const_iterator iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice)
   {
      devicesStatus.push_back((*iteratorDevice)->getStatus());
   }

   return devicesStatus;
}

bool CIOManager::deviceAlreadyExist(const std::string& deviceName)
{
   std::vector<boost::shared_ptr<equipments::IEquipment>>::const_iterator iteratorDevice;

   for (iteratorDevice = m_deviceManager.begin(); (iteratorDevice != m_deviceManager.end() && (*iteratorDevice)->getDeviceName() != deviceName); ++iteratorDevice)
   {
   }

   if (iteratorDevice != m_deviceManager.end())
      return true;
   
   return false;
}

CIOManager::~CIOManager()
{
}

