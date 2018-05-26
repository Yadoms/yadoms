#include "stdafx.h"
#include "DeviceManager.h"
#include "notification/Helpers.hpp"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/historization/DeviceStateMessage.h>

namespace dataAccessLayer
{
   CDeviceManager::CDeviceManager(boost::shared_ptr<database::IDeviceRequester> deviceRequester,
                                  boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                                  boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                                  boost::shared_ptr<IKeywordManager> keywordManager)
      : m_deviceRequester(deviceRequester),
        m_keywordRequester(keywordRequester),
        m_acquisitionRequester(acquisitionRequester),
        m_keywordManager(keywordManager)
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

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::createDevice(int pluginId, const std::string& name, const std::string& friendlyName, const std::string& type, const std::string& model, const shared::CDataContainer& details)
   {
      //create the device
      auto result = m_deviceRequester->createDevice(pluginId, name, friendlyName, type, model, details);

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

   void CDeviceManager::updateDeviceType(int deviceId, const std::string& type)
   {
      m_deviceRequester->updateDeviceType(deviceId, type);
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

   void CDeviceManager::updateDeviceState(int deviceId, const shared::plugin::yPluginApi::historization::EDeviceState& state, const std::string& customMessageId, const shared::CDataContainer &data) const
   {

      //if keywords do exist, create them
      shared::plugin::yPluginApi::historization::CDeviceState ds("deviceState");
      auto stateKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(deviceId, ds.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      if (stateKeywords.empty())
      {
         m_keywordManager->addKeyword(deviceId, ds);
      }

      shared::plugin::yPluginApi::historization::CDeviceStateMessage dsm("deviceStateMessage");
      auto stateMessageKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(deviceId, dsm.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      if (stateMessageKeywords.empty())
      {
         m_keywordManager->addKeyword(deviceId, dsm);
      }

      //update deviceSate
      ds.set(state);
     
      //update deviceMessageState
      dsm.set(data);

      auto currentDate = shared::currentTime::Provider().now();

      stateKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(deviceId, ds.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      for (auto i = stateKeywords.begin(); i != stateKeywords.end(); ++i)
         m_acquisitionRequester->saveData((*i)->Id, ds.formatValue(), currentDate);

      stateMessageKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(deviceId, dsm.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      for (auto i = stateMessageKeywords.begin(); i != stateMessageKeywords.end(); ++i)
         m_acquisitionRequester->saveData((*i)->Id, dsm.formatValue(), currentDate);
   }

   void CDeviceManager::removeDevice(int deviceId)
   {
      auto device = m_deviceRequester->getDevice(deviceId);

      cleanupDevice(deviceId);
      auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         m_keywordManager->removeKeyword((*keyword)->Id);

      m_deviceRequester->removeDevice(deviceId);

      //post notification
      notification::CHelpers::postChangeNotification(device, notification::change::EChangeType::kDelete);
   }

   void CDeviceManager::removeDevice(int pluginId, const std::string& deviceName)
   {
      removeDevice(m_deviceRequester->getDeviceInPlugin(pluginId, deviceName)->Id());
   }

   void CDeviceManager::removeAllDeviceForPlugin(int pluginId)
   {
      auto devicesToDelete = getDevicesForPluginInstance(pluginId);
      for (auto device = devicesToDelete.begin(); device != devicesToDelete.end(); ++device)
         removeDevice(pluginId, (*device));
   }

   void CDeviceManager::cleanupDevice(int deviceId)
   {
      auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         m_acquisitionRequester->removeKeywordData((*keyword)->Id);
   }
} //namespace dataAccessLayer 


