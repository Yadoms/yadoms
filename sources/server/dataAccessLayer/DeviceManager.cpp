#include "stdafx.h"
#include "DeviceManager.h"
#include "notifications/AsyncNotificationCenter.h"
#include "notifications/NewDeviceNotification.h"

namespace dataAccessLayer {

      CDeviceManager::CDeviceManager(boost::shared_ptr< database::IDeviceRequester > deviceRequester, boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
         :m_deviceRequester(deviceRequester), m_notificationCenter(notificationCenter)
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

      std::vector<boost::shared_ptr<database::entities::CDevice> > CDeviceManager::getDeviceWithCapacity(const std::string & capacityName, const shared::plugin::yadomsApi::EKeywordAccessMode & capacityAccessMode) const
      {
         return m_deviceRequester->getDeviceWithCapacity(capacityName, capacityAccessMode);
      }    

      std::vector<boost::shared_ptr<database::entities::CDevice> > CDeviceManager::getDeviceWithCapacityType(const shared::plugin::yadomsApi::EKeywordAccessMode & capacityAccessMode, const shared::plugin::yadomsApi::EKeywordDataType & capacityType) const
      {
         return m_deviceRequester->getDeviceWithCapacityType(capacityAccessMode, capacityType);
      }    

      boost::shared_ptr<database::entities::CDevice> CDeviceManager::createDevice(int pluginId, const std::string & name, const std::string & friendlyName, const std::string & model, const shared::CDataContainer & details)
      {
         //create the device
         boost::shared_ptr<database::entities::CDevice> result = m_deviceRequester->createDevice(pluginId, name, friendlyName, model, details);

         //post notification
         try
         {
            boost::shared_ptr< notifications::CNewDeviceNotification > notificationData(new notifications::CNewDeviceNotification(result));
            m_notificationCenter->postNotification(notificationData);
         }
         catch (shared::exception::CException & ex)
         {
            YADOMS_LOG(error) << "Fail to retreive new acquisition : " << ex.what();
         }

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
