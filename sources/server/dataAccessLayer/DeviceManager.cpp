#include "stdafx.h"
#include "DeviceManager.h"
#include "notification/Helpers.hpp"
#include <shared/plugin/yPluginApi/historization/DeviceStateMessage.h>

#include <utility>

namespace dataAccessLayer
{
   CDeviceManager::CDeviceManager(boost::shared_ptr<database::IDeviceRequester> deviceRequester,
                                  boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                                  boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                                  boost::shared_ptr<IKeywordManager> keywordManager)
      : m_deviceRequester(std::move(deviceRequester)),
        m_keywordRequester(std::move(keywordRequester)),
        m_acquisitionRequester(std::move(acquisitionRequester)),
        m_keywordManager(std::move(keywordManager))
   {
   }

   bool CDeviceManager::deviceExists(int deviceId) const
   {
      return m_deviceRequester->deviceExists(deviceId);
   }

   bool CDeviceManager::deviceExists(int pluginId, const std::string& deviceName) const
   {
      return m_deviceRequester->deviceExists(pluginId, deviceName);
   }

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::getDevice(int deviceId) const
   {
      return m_deviceRequester->getDevice(deviceId);
   }

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::getDeviceInPlugin(int pluginId,
                                                                                    const std::string& name,
                                                                                    bool includeBlacklistDevice) const
   {
      return m_deviceRequester->getDeviceInPlugin(pluginId, name, includeBlacklistDevice);
   }

   std::vector<boost::shared_ptr<database::entities::CDevice>> CDeviceManager::getDeviceWithCapacity(
      const std::string& capacityName,
      const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const
   {
      return m_deviceRequester->getDeviceWithCapacity(capacityName, capacityAccessMode);
   }

   std::vector<boost::shared_ptr<database::entities::CDevice>> CDeviceManager::getDeviceWithCapacityType(
      const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
      const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const
   {
      return m_deviceRequester->getDeviceWithCapacityType(capacityAccessMode, capacityType);
   }

   std::vector<boost::shared_ptr<database::entities::CDevice>> CDeviceManager::getDevicesMatchingCriteria(
      const std::vector<std::string>& expectedNames,
      bool blacklisted) const
   {
      return m_deviceRequester->getDevices(expectedNames,
                                           blacklisted);
   }

   boost::shared_ptr<database::entities::CDevice> CDeviceManager::createDevice(int pluginId,
                                                                               const std::string& name,
                                                                               const std::string& friendlyName,
                                                                               const std::string& type,
                                                                               const std::string& model,
                                                                               boost::shared_ptr<shared::CDataContainer> details)
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

   void CDeviceManager::updateDeviceFriendlyName(int deviceId,
                                                 const std::string& newFriendlyName)
   {
      m_deviceRequester->updateDeviceFriendlyName(deviceId, newFriendlyName);
   }

   void CDeviceManager::updateDeviceConfiguration(int deviceId,
                                                  boost::shared_ptr<shared::CDataContainer> configuration)
   {
      m_deviceRequester->updateDeviceConfiguration(deviceId,
                                                   configuration);

      const auto result = m_deviceRequester->getDevice(deviceId);

      //post notification
      notification::CHelpers::postChangeNotification(result,
                                                     notification::change::EChangeType::kUpdate);
   }

   void CDeviceManager::updateDeviceDetails(int deviceId,
                                            boost::shared_ptr<shared::CDataContainer> details)
   {
      m_deviceRequester->updateDeviceDetails(deviceId,
                                             details);
   }

   void CDeviceManager::updateDeviceModel(int deviceId,
                                          const std::string& model)
   {
      m_deviceRequester->updateDeviceModel(deviceId,
                                           model);
   }

   void CDeviceManager::updateDeviceType(int deviceId,
                                         const std::string& type)
   {
      m_deviceRequester->updateDeviceType(deviceId,
                                          type);
   }

   void CDeviceManager::updateDeviceBlacklistState(int deviceId,
                                                   bool blacklist)
   {
      //cleanup data
      if (blacklist)
         cleanupDevice(deviceId);

      //update blacklist state of all attached keywords
      const auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (auto& keyword : keywords)
         m_keywordRequester->updateKeywordBlacklistState(keyword->Id, blacklist);

      //update device blacklist state
      m_deviceRequester->updateDeviceBlacklistState(deviceId,
                                                    blacklist);

      //post notification
      if (blacklist)
      {
         const auto device = m_deviceRequester->getDevice(deviceId,
                                                          true);
         notification::CHelpers::postChangeNotification(device,
                                                        notification::change::EChangeType::kBlacklist);
      }
   }

   void CDeviceManager::updateDeviceState(int deviceId,
                                          const shared::plugin::yPluginApi::historization::EDeviceState& state,
                                          const std::string& customMessageId,
                                          boost::shared_ptr<shared::CDataContainer> data) const
   {
      //if keywords do exist, create them
      shared::plugin::yPluginApi::historization::CDeviceState ds("deviceState");
      auto stateKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(
         deviceId, ds.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      if (stateKeywords.empty())
      {
         m_keywordManager->addKeyword(deviceId, ds);
      }

      shared::plugin::yPluginApi::historization::CDeviceStateMessage dsm("deviceStateMessage");
      auto stateMessageKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(
         deviceId, dsm.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      if (stateMessageKeywords.empty())
      {
         m_keywordManager->addKeyword(deviceId, dsm);
      }

      //update deviceSate
      ds.set(state);

      //update deviceMessageState
      dsm.set(data);

      auto currentDate = shared::currentTime::Provider().now();

      stateKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(deviceId, ds.getCapacity().getName(),
                                                                        shared::plugin::yPluginApi::EKeywordAccessMode::
                                                                        kGet);
      for (const auto& stateKeyword : stateKeywords)
         m_acquisitionRequester->saveData(stateKeyword->Id, ds.formatValue(), currentDate);

      stateMessageKeywords = m_keywordRequester->getDeviceKeywordsWithCapacity(
         deviceId, dsm.getCapacity().getName(), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
      for (const auto& stateMessageKeyword : stateMessageKeywords)
         m_acquisitionRequester->saveData(stateMessageKeyword->Id, dsm.formatValue(), currentDate);
   }

   void CDeviceManager::updateDevice(const database::entities::CDevice& device) const
   {
      m_deviceRequester->updateDevice(device);
   }

   void CDeviceManager::removeDevice(int deviceId)
   {
      const auto device = m_deviceRequester->getDevice(deviceId, true);

      cleanupDevice(deviceId);
      const auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (const auto& keyword : keywords)
         m_keywordManager->removeKeyword(keyword->Id);

      m_deviceRequester->removeDevice(deviceId);

      //post notification
      notification::CHelpers::postChangeNotification(device,
                                                     notification::change::EChangeType::kDelete);
   }

   void CDeviceManager::removeDevice(int pluginId,
                                     const std::string& deviceName)
   {
      removeDevice(m_deviceRequester->getDeviceInPlugin(pluginId,
                                                        deviceName)->Id());
   }

   void CDeviceManager::removeAllDeviceForPlugin(int pluginId)
   {
      const auto devicesToDelete = getDevicesForPluginInstance(pluginId);
      for (const auto& device : devicesToDelete)
         removeDevice(pluginId, device);
   }

   void CDeviceManager::cleanupDevice(int deviceId)
   {
      const auto keywords = m_keywordRequester->getKeywords(deviceId);
      for (const auto& keyword : keywords)
         m_acquisitionRequester->removeKeywordData(keyword->Id);
   }
} //namespace dataAccessLayer 
