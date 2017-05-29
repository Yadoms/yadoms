#include "stdafx.h"
#include "IOManager.h"
#include "urlManager.h"
#include <boost/regex.hpp>
#include <shared/Log.h>
#include <algorithm>

CIOManager::CIOManager(std::vector<boost::shared_ptr<equipments::IEquipment> >& deviceList,
                       std::vector<boost::shared_ptr<equipments::IEquipment> >& masterDeviceList):
   m_deviceManager(deviceList),
   m_masterDeviceManager(masterDeviceList)
{}

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
         m_deviceManager[counter]->remove(api);
         m_deviceManager.erase(m_deviceManager.begin()+counter); // TODO : Doesn't work properly
         return;
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
   std::string deviceName = command->getDevice();

   YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command) ;
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorDevice;

   for (iteratorDevice = m_deviceManager.begin(); iteratorDevice != m_deviceManager.end(); ++iteratorDevice)
   {
      if (deviceName == (*iteratorDevice)->getDeviceName())
      {
         (*iteratorDevice)->sendCommand(api, command->getKeyword(), command);
         return;
      }
   }
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
         (*iteratorDevice)->updateConfiguration(api, newConfiguration);
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

std::vector<specificHistorizers::EWESdeviceStatus> CIOManager::getMasterdeviceStates()
{
   std::vector<specificHistorizers::EWESdeviceStatus> devicesStatus;
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorMasterDevice;

   for (iteratorMasterDevice = m_masterDeviceManager.begin(); iteratorMasterDevice != m_masterDeviceManager.end(); ++iteratorMasterDevice)
   {
      devicesStatus.push_back((*iteratorMasterDevice)->getStatus());
   }

   return devicesStatus;
}

bool CIOManager::deviceAlreadyExist(const std::string& deviceName)
{
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorDevice;

   for (iteratorDevice = m_deviceManager.begin(); (iteratorDevice != m_deviceManager.end() && (*iteratorDevice)->getDeviceName() != deviceName); ++iteratorDevice)
   {}

   if (iteratorDevice != m_deviceManager.end())
      return true;
   else
      return false;
}

CIOManager::~CIOManager()
{}