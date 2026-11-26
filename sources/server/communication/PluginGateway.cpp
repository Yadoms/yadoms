#include "stdafx.h"
#include "PluginGateway.h"

#include <utility>
#include "pluginSystem/DeviceCommand.h"
#include "pluginSystem/ExtraQuery.h"
#include "pluginSystem/ManuallyDeviceCreationRequest.h"
#include "pluginSystem/BindingQueryRequest.h"

using namespace communication;

CPluginGateway::CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider,
                               boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
                               boost::shared_ptr<pluginSystem::CManager> pluginManager)
   : _dataProvider(std::move(dataProvider)),
     _pluginManager(std::move(pluginManager)),
     _acquisitionHistorizer(std::move(acquisitionHistorizer))
{
}

void CPluginGateway::sendKeywordCommandAsync(int keywordId,
                                             const std::string& body)
{
   auto keyword = _dataProvider->getKeywordRequester()->getKeyword(keywordId);
   const auto device = _dataProvider->getDeviceRequester()->getDevice(keyword->DeviceId);

   if (keyword->AccessMode() != shared::plugin::yPluginApi::EKeywordAccessMode::kGetSetValue)
   {
      YADOMS_LOG(error) << "Can not send command to not writable keyword (" << keywordId << ")";
      throw std::runtime_error("Keyword " + std::to_string(keywordId) + " is not writable");
   }

   // Create the command
   const auto command(boost::make_shared<pluginSystem::CDeviceCommand>(device->Name, keyword, body));

   // Dispatch command to the right plugin
   _pluginManager->postCommand(device->PluginId, command);

   // Historize the command
   _acquisitionHistorizer->saveData(keywordId, command->getHistorizableObject());
}


std::string CPluginGateway::sendExtraQueryAsync(int pluginInstanceId,
                                                boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> data)
{
   // Create the query
   const auto extraQuery(boost::make_shared<pluginSystem::CExtraQuery>(data));

   // Dispatch query to the right plugin
   return _pluginManager->postExtraQuery(pluginInstanceId, extraQuery);
}


void CPluginGateway::sendManuallyDeviceCreationRequest(int pluginInstanceId,
                                                       const shared::plugin::yPluginApi::IManuallyDeviceCreationData& data,
                                                       callback::ISynchronousCallback<std::string>& callback)
{
   // Create the request
   const boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request(
      boost::make_shared<pluginSystem::CManuallyDeviceCreationRequest>(data,
                                                                       callback));

   // Dispatch command to the right plugin
   _pluginManager->postManuallyDeviceCreationRequest(pluginInstanceId, request);
}

void CPluginGateway::sendBindingQueryRequest(int pluginInstanceId,
                                             const shared::plugin::yPluginApi::IBindingQueryData& data,
                                             callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback)
{
   // Create the request
   const boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request(boost::make_shared<pluginSystem::CBindingQueryRequest>(data,
      callback));

   // Dispatch command to the right plugin
   _pluginManager->postBindingQueryRequest(pluginInstanceId, request);
}

void CPluginGateway::sendDeviceConfigurationSchemaRequest(int deviceId,
                                                          callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback)
{
   _pluginManager->postDeviceConfigurationSchemaRequest(deviceId,
                                                         callback);
}

void CPluginGateway::sendSetDeviceConfiguration(int deviceId,
                                                const boost::shared_ptr<shared::CDataContainer>& configuration)
{
   _pluginManager->postSetDeviceConfiguration(deviceId,
                                               configuration);
}
