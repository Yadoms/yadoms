#include "stdafx.h"
#include "PluginGateway.h"
#include "pluginSystem/DeviceCommand.h"
#include "pluginSystem/ExtraCommand.h"
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

   void CPluginGateway::sendCommandAsync(int keywordId,
                                         const std::string& body)
   {
      auto keyword = m_dataProvider->getKeywordRequester()->getKeyword(keywordId);
      auto device = m_dataProvider->getDeviceRequester()->getDevice(keyword->DeviceId);

      // Create the command
      auto command(boost::make_shared<pluginSystem::CDeviceCommand>(device->Name, keyword->Name, body));

      // Dispatch command to the right plugin
      m_pluginManager->postCommand(device->PluginId, command);

      // Historize the command
      m_acquisitionHistorizer->saveData(keywordId, command->getHistorizableObject());
   }

   void CPluginGateway::sendExtraCommandAsync(int pluginId,
                                              const std::string& command,
                                              const shared::CDataContainer& data)
   {
      // Create the command
      boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand(boost::make_shared<pluginSystem::CExtraCommand>(command, data));

      // Dispatch command to the right plugin
      m_pluginManager->postExtraCommand(pluginId, extraCommand);
   }


   void CPluginGateway::sendManuallyDeviceCreationRequest(int pluginId,
                                                          const shared::plugin::yPluginApi::IManuallyDeviceCreationData& data,
                                                          callback::ISynchronousCallback<std::string>& callback)
   {
      // Create the request
      boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request(boost::make_shared<pluginSystem::CManuallyDeviceCreationRequest>(data, callback));

      // Dispatch command to the right plugin
      m_pluginManager->postManuallyDeviceCreationRequest(pluginId, request);
   }

   void CPluginGateway::sendBindingQueryRequest(int pluginId,
                                                const shared::plugin::yPluginApi::IBindingQueryData& data,
                                                callback::ISynchronousCallback<shared::CDataContainer>& callback)
   {
      // Create the request
      boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request(boost::make_shared<pluginSystem::CBindingQueryRequest>(data, callback));

      // Dispatch command to the right plugin
      m_pluginManager->postBindingQueryRequest(pluginId, request);
   }

   void CPluginGateway::sendDeviceConfigurationSchemaRequest(int deviceId,
                                                             communication::callback::ISynchronousCallback<shared::CDataContainer>& callback)
   {
      m_pluginManager->postDeviceConfigurationSchemaRequest(deviceId, callback);
   }
} //namespace communication


