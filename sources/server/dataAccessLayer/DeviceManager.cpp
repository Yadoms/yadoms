#include "stdafx.h"
#include "DeviceManager.h"

namespace dataAccessLayer {

      CDeviceManager::CDeviceManager(boost::shared_ptr< database::IDeviceRequester > deviceRequester, boost::shared_ptr<notification::newDevice::INotifier> notifier)
         :m_deviceRequester(deviceRequester), m_notifier(notifier)
      {
      }
   
      CDeviceManager::~CDeviceManager()
      {
      }
      
      bool CDeviceManager::deviceExists(const int deviceId) const
      {
         return m_deviceRequester->deviceExists(deviceId);
      }

      bool CDeviceManager::deviceExists(const int pluginId, const std::string & deviceName) const
      {
         return m_deviceRequester->deviceExists(pluginId, deviceName);
      }      

      boost::shared_ptr<database::entities::CDevice> CDeviceManager::getDevice(int deviceId) const
      {
         return m_deviceRequester->getDevice(deviceId);
      }    
      
      boost::shared_ptr<database::entities::CDevice> CDeviceManager::getDevice(const int pluginId, const std::string & name) const
      {
         return m_deviceRequester->getDevice(pluginId, name);
      }    

      std::vector<boost::shared_ptr<database::entities::CDevice> > CDeviceManager::getDeviceWithCapacity(const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode) const
      {
         return m_deviceRequester->getDeviceWithCapacity(capacityName, capacityAccessMode);
      }    

      std::vector<boost::shared_ptr<database::entities::CDevice> > CDeviceManager::getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType & capacityType) const
      {
         return m_deviceRequester->getDeviceWithCapacityType(capacityAccessMode, capacityType);
      }    

      boost::shared_ptr<database::entities::CDevice> CDeviceManager::createDevice(int pluginId, const std::string & name, const std::string & friendlyName, const std::string & model, const shared::CDataContainer & details)
      {
         //create the device
         boost::shared_ptr<database::entities::CDevice> result = m_deviceRequester->createDevice(pluginId, name, friendlyName, model, details);

         //post notification
         m_notifier->post(result);

         return result;
      }    

      std::vector<boost::shared_ptr<database::entities::CDevice> > CDeviceManager::getDevices() const
      {
         return m_deviceRequester->getDevices();
      }    

     
      void CDeviceManager::updateDeviceFriendlyName(int deviceId, const std::string & newFriendlyName)
      {
         return m_deviceRequester->updateDeviceFriendlyName(deviceId, newFriendlyName);
      }    

      
      void CDeviceManager::removeDevice(int deviceId)
      {
         return m_deviceRequester->removeDevice(deviceId);
      }    

      void CDeviceManager::removeAllDeviceForPlugin(int pluginId)
      {
         return m_deviceRequester->removeAllDeviceForPlugin(pluginId);
      }    

 
} //namespace dataAccessLayer 
