#include "stdafx.h"
#include "yPluginApiImplementation.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>
#include <startupOptions/IStartupOptions.h>
#include <shared/ServiceLocator.h>

namespace pluginSystem
{
   CYPluginApiImplementation::CYPluginApiImplementation(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
                                                        const boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                        const boost::filesystem::path& dataPath,
                                                        boost::shared_ptr<IInstanceStateHandler> instanceStateHandler,
                                                        boost::shared_ptr<database::IDataProvider> dataProvider,
                                                        boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                                                        boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordDataAccessLayer,
                                                        boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer)
      : m_informations(pluginInformations),
        m_dataPath(dataPath),
        m_instanceData(instanceData),
        m_instanceStateHandler(instanceStateHandler),
        m_deviceManager(deviceManager),
        m_keywordDataAccessLayer(keywordDataAccessLayer),
        m_recipientRequester(dataProvider->getRecipientRequester()),
        m_acquisitionHistorizer(acquisitionHistorizer)
   {
   }

   CYPluginApiImplementation::~CYPluginApiImplementation()
   {
   }

   void CYPluginApiImplementation::setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state,
                                                  const std::string& customMessageId,
                                                  const std::map<std::string, std::string>& customMessageDataParams)
   {
      //convert map to dataContainer
      shared::CDataContainer dc;
      std::map<std::string, std::string>::const_iterator i;
      for (i = customMessageDataParams.begin(); i != customMessageDataParams.end(); ++i)
         dc.set(i->first, i->second);

      auto customMessageDataSerialized = dc.serialize(); //use variable to allow use of reference parameter
      m_instanceStateHandler->setState(state, customMessageId, customMessageDataSerialized);
   }

   void CYPluginApiImplementation::declareDevice(const std::string& device,
                                                 const std::string& model,
                                                 boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                                                 const shared::CDataContainer& details)
   {
      if (!deviceExists(device))
         m_deviceManager->createDevice(getPluginId(), device, device, model, details);

      if (!keywordExists(device, keyword))
         declareKeyword(device, keyword);
   }

   void CYPluginApiImplementation::declareDevice(const std::string& device,
                                                 const std::string& model,
                                                 const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords,
                                                 const shared::CDataContainer& details)
   {
      if (!deviceExists(device))
         m_deviceManager->createDevice(getPluginId(), device, device, model, details);

      declareKeywords(device, keywords);
   }

   std::vector<std::string> CYPluginApiImplementation::getAllDevices() const
   {
      return m_deviceManager->getDevicesForPluginInstance(getPluginId());
   }

   bool CYPluginApiImplementation::deviceExists(const std::string& device) const
   {
      return m_deviceManager->deviceExists(getPluginId(), device);
   }

   shared::CDataContainer CYPluginApiImplementation::getDeviceConfiguration(const std::string& device) const
   {
      return m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Configuration;
   }

   void CYPluginApiImplementation::updateDeviceConfiguration(const std::string& device,
                                                             const shared::CDataContainer& configuration) const
   {
      if (!deviceExists(device))
         throw shared::exception::CEmptyResult("Fail to update device configuration : device doesn't exist.");

      m_deviceManager->updateDeviceConfiguration(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id(),
                                                 configuration);
   }

   shared::CDataContainer CYPluginApiImplementation::getDeviceDetails(const std::string& device) const
   {
      return m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Details;
   }

   void CYPluginApiImplementation::updateDeviceDetails(const std::string& device,
                                                       const shared::CDataContainer& details) const
   {
      if (!deviceExists(device))
         throw shared::exception::CEmptyResult("Fail to update device details : device doesn't exist.");

      m_deviceManager->updateDeviceDetails(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id(),
                                           details);
   }

   std::string CYPluginApiImplementation::getDeviceModel(const std::string& device) const
   {
      return m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Model;
   }

   void CYPluginApiImplementation::updateDeviceModel(const std::string& device,
                                                     const std::string& model) const
   {
      if (!deviceExists(device))
         throw shared::exception::CEmptyResult("Fail to update device model : device doesn't exist.");

      m_deviceManager->updateDeviceModel(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id(),
                                         model);
   }

   void CYPluginApiImplementation::removeDevice(const std::string& device)
   {
      if (!deviceExists(device))
         throw shared::exception::CEmptyResult("Fail to remove device : device doesn't exist.");

      m_deviceManager->removeDevice(getPluginId(), device);
   }

   void CYPluginApiImplementation::declareKeyword(const std::string& device,
                                                  boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                                                  const shared::CDataContainer& details)
   {
      if (keywordExists(device, keyword))
         throw shared::exception::CEmptyResult((boost::format("Fail to declare %1% keyword : keyword already exists") % keyword->getKeyword()).str());

      m_keywordDataAccessLayer->addKeyword(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id(),
                                           *keyword,
                                           details);
   }

   void CYPluginApiImplementation::declareKeywords(const std::string& device,
                                                   const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords)
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywordsToDeclare;

      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         if (!keywordExists(device, *keyword))
            keywordsToDeclare.push_back(*keyword);

      if (keywordsToDeclare.empty())
         return;

      m_keywordDataAccessLayer->addKeywords(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id(),
                                            keywordsToDeclare);
   }

   bool CYPluginApiImplementation::keywordExists(const std::string& device,
                                                 const std::string& keyword) const
   {
      if (!deviceExists(device))
         throw shared::exception::CEmptyResult("Fail to check if keyword exists : owner device doesn't exist.");

      return m_keywordDataAccessLayer->keywordExists((m_deviceManager->getDeviceInPlugin(getPluginId(), device))->Id, keyword);
   }

   bool CYPluginApiImplementation::keywordExists(const std::string& device,
                                                 boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword) const
   {
      return keywordExists(device, keyword->getKeyword());
   }

   std::vector<std::string> CYPluginApiImplementation::getAllKeywords(const std::string& device) const
   {
      std::vector<std::string> keywordNames;
      for (const auto& keyword : m_keywordDataAccessLayer->getKeywords(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id()))
         keywordNames.push_back(keyword->Name());
      return keywordNames;
   }

   void CYPluginApiImplementation::removeKeyword(const std::string& device,
                                                 const std::string& keyword)
   {
      if (!keywordExists(device, keyword))
         throw shared::exception::CEmptyResult((boost::format("Fail to remove %1% keyword : keyword doesn't exists") % keyword).str());

      m_keywordDataAccessLayer->removeKeyword(m_deviceManager->getDeviceInPlugin(getPluginId(), device)->Id(),
                                              keyword);
   }

   std::string CYPluginApiImplementation::getRecipientValue(int recipientId,
                                                            const std::string& fieldName) const
   {
      boost::shared_ptr<const database::entities::CRecipient> recipient = m_recipientRequester->getRecipient(recipientId);

      // Search for from plugin fields
      for (auto itField = recipient->Fields().begin(); itField != recipient->Fields().end(); ++itField)
         if ((*itField)->PluginType == m_instanceData->Type && (*itField)->FieldName == fieldName)
            return (*itField)->Value;

      // If not found from plugin fields, looking for general fields
      for (auto itField = recipient->Fields().begin(); itField != recipient->Fields().end(); ++itField)
         if ((*itField)->PluginType == "system" && (*itField)->FieldName == fieldName)
            return (*itField)->Value;

      throw shared::exception::CEmptyResult((boost::format("Cannot retrieve field %1% for recipient Id %2% in database") % fieldName % recipientId).str());
   }

   std::vector<int> CYPluginApiImplementation::findRecipientsFromField(const std::string& fieldName,
                                                                       const std::string& expectedFieldValue) const
   {
      auto recipients = m_recipientRequester->findRecipientsFromField(fieldName, expectedFieldValue);
      std::vector<int> recipientIds;

      for (auto itRecipient = recipients.begin(); itRecipient != recipients.end(); ++itRecipient)
         recipientIds.push_back((*itRecipient)->Id);

      return recipientIds;
   }

   bool CYPluginApiImplementation::recipientFieldExists(const std::string& fieldName) const
   {
      return m_recipientRequester->fieldExists(fieldName, getInformation()->getType());
   }


   void CYPluginApiImplementation::historizeData(const std::string& device,
                                                 boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> data)
   {
      try
      {
         boost::shared_ptr<const database::entities::CDevice> deviceEntity = m_deviceManager->getDeviceInPlugin(getPluginId(), device);
         boost::shared_ptr<const database::entities::CKeyword> keywordEntity = m_keywordDataAccessLayer->getKeyword(deviceEntity->Id, data->getKeyword());

         m_acquisitionHistorizer->saveData(keywordEntity->Id, *data);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         YADOMS_LOG(error) << "Error historizing data, device or keyword not found : " << e.what();
      }
   }

   void CYPluginApiImplementation::historizeData(const std::string& device,
                                                 const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect)
   {
      try
      {
         std::vector<int> keywordIdList;

         boost::shared_ptr<const database::entities::CDevice> deviceEntity = m_deviceManager->getDeviceInPlugin(getPluginId(), device);
         for (auto iter = dataVect.begin(); iter != dataVect.end(); ++iter)
         {
            boost::shared_ptr<const database::entities::CKeyword> keywordEntity = m_keywordDataAccessLayer->getKeyword(deviceEntity->Id, (*iter)->getKeyword());
            keywordIdList.push_back(keywordEntity->Id);
         }
         m_acquisitionHistorizer->saveData(keywordIdList, dataVect);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         YADOMS_LOG(error) << "Error historizing data, device or keyword not found : " << e.what();
      }
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CYPluginApiImplementation::getInformation() const
   {
      return m_informations;
   }

   shared::CDataContainer CYPluginApiImplementation::getConfiguration()
   {
      return m_instanceData->Configuration;
   }

   const boost::filesystem::path& CYPluginApiImplementation::getDataPath() const
   {
      return m_dataPath;
   }

   shared::event::CEventHandler& CYPluginApiImplementation::getEventHandler()
   {
      BOOST_ASSERT(false); // No event handler required here
      static shared::event::CEventHandler evtHandler;
      return evtHandler;
   }

   bool CYPluginApiImplementation::isDeveloperMode() const
   {
      return shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>()->getDeveloperMode();
   }

   int CYPluginApiImplementation::getPluginId() const
   {
      return m_instanceData->Id;
   }
} // namespace pluginSystem	


