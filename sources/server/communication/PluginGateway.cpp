#include "stdafx.h"
#include "PluginGateway.h"
#include "pluginSystem/DeviceCommand.h"
#include "pluginSystem/ExtraQuery.h"
#include "pluginSystem/ManuallyDeviceCreationRequest.h"
#include "pluginSystem/BindingQueryRequest.h"

namespace communication
{
   enum
   {
      kStartEvent = shared::event::kUserFirstId
   };

   CPluginGateway::CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider,
                                  boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
                                  boost::shared_ptr<pluginSystem::CManager> pluginManager)
      : m_dataProvider(dataProvider),
        m_pluginManager(pluginManager),
        m_acquisitionHistorizer(acquisitionHistorizer)
   {
   }

   CPluginGateway::~CPluginGateway()
   {
   }

   void CPluginGateway::sendKeywordCommandAsync(int keywordId,
                                                const std::string& body)
   {
      auto keyword = m_dataProvider->getKeywordRequester()->getKeyword(keywordId);
      auto device = m_dataProvider->getDeviceRequester()->getDevice(keyword->DeviceId);

      if (keyword->AccessMode() != shared::plugin::yPluginApi::EKeywordAccessMode::kGetSetValue)
      {
         YADOMS_LOG(error) << "Can not send command to not writable keyword (" << keywordId << ")";
         throw std::runtime_error("Keyword " + std::to_string(keywordId) + " is not writable");
      }

      // Create the command
      const auto command(boost::make_shared<pluginSystem::CDeviceCommand>(device->Name, keyword, body));

      // Dispatch command to the right plugin
      m_pluginManager->postCommand(device->PluginId, command);

      // Historize the command
      m_acquisitionHistorizer->saveData(keywordId, command->getHistorizableObject());
   }


   std::string CPluginGateway::sendExtraQueryAsync(int pluginId, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> data)
   {
      // Create the query
      const boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery(boost::make_shared<pluginSystem::CExtraQuery>(data));

      // Dispatch query to the right plugin
      return m_pluginManager->postExtraQuery(pluginId, extraQuery);
   }


   void CPluginGateway::sendManuallyDeviceCreationRequest(int pluginId,
                                                          const shared::plugin::yPluginApi::IManuallyDeviceCreationData& data,
                                                          callback::ISynchronousCallback<std::string>& callback)
   {
      // Create the request
      boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request(boost::make_shared<pluginSystem::CManuallyDeviceCreationRequest>(data,
                                                                                                                                                             callback));

      // Dispatch command to the right plugin
      m_pluginManager->postManuallyDeviceCreationRequest(pluginId, request);
   }

   void CPluginGateway::sendBindingQueryRequest(int pluginId,
                                                const shared::plugin::yPluginApi::IBindingQueryData& data,
                                                callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback)
   {
      // Create the request
      boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request(boost::make_shared<pluginSystem::CBindingQueryRequest>(data,
                                                                                                                                         callback));

      // Dispatch command to the right plugin
      m_pluginManager->postBindingQueryRequest(pluginId, request);
   }

   void CPluginGateway::sendDeviceConfigurationSchemaRequest(int deviceId,
                                                             callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback)
   {
      m_pluginManager->postDeviceConfigurationSchemaRequest(deviceId,
                                                            callback);
   }

   void CPluginGateway::sendSetDeviceConfiguration(int deviceId,
                                                   const boost::shared_ptr<shared::CDataContainer>& configuration)
   {
      m_pluginManager->postSetDeviceConfiguration(deviceId,
                                                  configuration);
   }
} //namespace communication


