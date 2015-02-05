#include "stdafx.h"
#include "PluginGateway.h"
#include <shared/Log.h>
#include "pluginSystem/DeviceCommand.h"
#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "pluginSystem/ManuallyDeviceCreationRequest.h"
#include "pluginSystem/BindingQueryData.h"
#include "pluginSystem/BindingQueryRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace communication {


   enum { kStartEvent = shared::event::kUserFirstId };

   CPluginGateway::CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer, boost::shared_ptr<pluginSystem::CManager> pluginManager)
      :m_dataProvider(dataProvider), m_pluginManager(pluginManager), m_acquisitionHistorizer(acquisitionHistorizer)
   {
   }

   CPluginGateway::~CPluginGateway()
   {
   }

   void CPluginGateway::sendCommandAsync(int keywordId, const std::string& body)
   {
      // Find the device ID associated to keyword
      int deviceId = m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->DeviceId;

      // Create the command
      boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command(new pluginSystem::CDeviceCommand(m_dataProvider->getDeviceRequester()->getDevice(deviceId)->Name,
         m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->Name, body));

      // Dispatch command to the right plugin
      m_pluginManager->postCommand(m_dataProvider->getDeviceRequester()->getDevice(deviceId)->PluginId, command);

      // Historize the command
      m_acquisitionHistorizer->saveData(keywordId, command->getHistorizableObject());
   }

   void CPluginGateway::sendManuallyDeviceCreationRequest(int pluginId, const shared::plugin::yPluginApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback)
   {
      // Create the request
      boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request(new pluginSystem::CManuallyDeviceCreationRequest(data, callback));
      
      // Dispatch command to the right plugin
      m_pluginManager->postManuallyDeviceCreationRequest(pluginId, request);
   }

   void CPluginGateway::sendBindingQueryRequest(int pluginId, const shared::plugin::yPluginApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< shared::CDataContainer > & callback)
	{
		// Create the request
      boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request(new pluginSystem::CBindingQueryRequest(data, callback));

		// Dispatch command to the right plugin
		m_pluginManager->postBindingQueryRequest(pluginId, request);
	}

} //namespace communication
