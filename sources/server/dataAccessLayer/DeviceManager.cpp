#include "stdafx.h"
#include "DeviceManager.h"
#include "notification/Helpers.hpp"
#include <shared/Log.h>

namespace dataAccessLayer
{
   CDeviceManager::CDeviceManager(boost::shared_ptr<database::IDeviceRequester> deviceRequester,
                                  boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                                  boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester)
      : m_deviceRequester(deviceRequester),
        m_keywordRequester(keywordRequester),
        m_acquisitionRequester(acquisitionRequester)
   {
   }

   CDeviceManager::~CDeviceManager()
   {
   }

   bool CDeviceManager::deviceExists(const int deviceId) const
   {
      return m_deviceRequester->deviceExists(deviceId);
   }

   bool CDeviceManager::deviceExists(const int pluginId, const std::string& deviceName) const
   {
      return m_deviceRequester->deviceExists(pluginId, deviceName);
   }

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::getDevice(int deviceId) const
   {
      return m_deviceRequester->getDevice(deviceId);
   }

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::getDeviceInPlugin(const int pluginId, const std::string& name, bool includeBlacklistDevice) const
   {
      return m_deviceRequester->getDeviceInPlugin(pluginId, name, includeBlacklistDevice);
   }

   std::vector<boost::shared_ptr<database::entities::CDevice>> CDeviceManager::getDeviceWithCapacity(const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const
   {
      return m_deviceRequester->getDeviceWithCapacity(capacityName, capacityAccessMode);
   }

   std::vector<boost::shared_ptr<database::entities::CDevice>> CDeviceManager::getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const
   {
      return m_deviceRequester->getDeviceWithCapacityType(capacityAccessMode, capacityType);
   }

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::createDevice(int pluginId, const std::string& name, const std::string& friendlyName, const std::string& model, const shared::CDataContainer& details)
   {
      //create the device
      auto result = m_deviceRequester->createDevice(pluginId, name, friendlyName, model, details);

      //post notification
      notification::CHelpers::postChangeNotification(result, notification::change::EChangeType::kCreate);
      return result;
   }

   std::vector<boost::shared_ptr<database::entities::CDevice>> CDeviceManager::getDevices() const
   {
      return m_deviceRequester->getDevices();
   }

   std::vector<std::string> CDeviceManager::getDevicesForPluginInstance(int pluginId) const
   {
      return m_deviceRequester->getDevicesNames(pluginId);
   }

   void CDeviceManager::updateDeviceFriendlyName(int deviceId, const std::string& newFriendlyName)
   {
      m_deviceRequester->updateDeviceFriendlyName(deviceId, newFriendlyName);
   }

   void CDeviceManager::updateDeviceConfiguration(int deviceId, const shared::CDataContainer& configuration)
   {
      m_deviceRequester->updateDeviceConfiguration(deviceId, configuration);

      auto result = m_deviceRequester->getDevice(deviceId);

      //post notification
      notification::CHelpers::postChangeNotification(result, notification::change::EChangeType::kUpdate);
   }

   void CDeviceManager::updateDeviceDetails(int deviceId, const shared::CDataContainer& details)
   {
      m_deviceRequester->updateDeviceDetails(deviceId, details);
   }

   void CDeviceManager::updateDeviceModel(int deviceId, const std::string& model)
   {
      m_deviceRequester->updateDeviceModel(deviceId, model);
   }

   void CDeviceManager::updateDeviceBlacklistState(int deviceId, const bool blacklist)
   {
      //cleanup data
      if(blacklist)
         cleanupDevice(deviceId);

      //update blacklist state of all attached keywords
      auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         m_keywordRequester->updateKeywordBlacklistState((*keyword)->Id, blacklist);

      //update device blacklist state
      m_deviceRequester->updateDeviceBlacklistState(deviceId, blacklist);
   }

   void CDeviceManager::removeDevice(int deviceId)
   {
      cleanupDevice(deviceId);

      auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         m_keywordRequester->removeKeyword((*keyword)->Id);

      m_deviceRequester->removeDevice(deviceId);
   }

   void CDeviceManager::removeDevice(int pluginId, const std::string& deviceName)
   {
      removeDevice(m_deviceRequester->getDeviceInPlugin(pluginId, deviceName)->Id());
   }

   void CDeviceManager::removeAllDeviceForPlugin(int pluginId)
   {
      return m_deviceRequester->removeAllDeviceForPlugin(pluginId);
   }

   void CDeviceManager::cleanupDevice(int deviceId)
   {
      auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         m_acquisitionRequester->removeKeywordData((*keyword)->Id);
   }
} //namespace dataAccessLayer 


