﻿#include "stdafx.h"
#include "PluginGateway.h"
#include <shared/Log.h>
#include "pluginSystem/DeviceCommand.h"
#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "pluginSystem/ManuallyDeviceCreationRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace communication {


   enum { kStartEvent = shared::event::kUserFirstId };

   CPluginGateway::CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer, boost::shared_ptr<pluginSystem::CManager> pluginManager)
      :CThreadBase("PluginGateway"), m_dataProvider(dataProvider), m_pluginManager(pluginManager), m_acquisitionHistorizer(acquisitionHistorizer)
   {
   }

   CPluginGateway::~CPluginGateway()
   {
   }

   void CPluginGateway::start()
   {
      CThreadBase::start();

      waitForstarted();
   }

   void CPluginGateway::waitForstarted()
   {
      switch (m_startEventHandler.waitForEvents(boost::posix_time::seconds(10)))
      {
      case kStartEvent: // Gateway started
         return;
      case shared::event::kTimeout:
         BOOST_ASSERT_MSG(false, "Unable to start plugin gateway");
         throw shared::exception::CException("Unable to start plugin gateway");
         break;
      default:
         BOOST_ASSERT_MSG(false, "Plugin gateway : unknown event");
         throw shared::exception::CException("Plugin gateway : unknown event");
         break;
      }
   }

   void CPluginGateway::sendCommandAsync(int deviceId, int keywordId, const shared::CDataContainer& body)
   {
      // Create the command
      boost::shared_ptr<const shared::plugin::yadomsApi::IDeviceCommand> command(new pluginSystem::CDeviceCommand(m_dataProvider->getDeviceRequester()->getDevice(deviceId)->Name,
         m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->Name, body));

      // Dispatch command to the right plugin
      m_pluginManager->postCommand(m_dataProvider->getDeviceRequester()->getDevice(deviceId)->PluginId, command);

      // Historize the command
      m_acquisitionHistorizer->saveData(keywordId, command->getHistorizableObject());
   }

   void CPluginGateway::sendManuallyDeviceCreationRequest(int pluginId, const shared::plugin::yadomsApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback)
   {
      // Create the request
      //can not use "new shared::plugin::yadomsApi::CManuallyDeviceCreationRequest"
      boost::shared_ptr<pluginSystem::CManuallyDeviceCreationRequest> request(new pluginSystem::CManuallyDeviceCreationRequest(data, callback));

      
      // Dispatch command to the right plugin
      m_pluginManager->postManuallyDeviceCreationRequest(pluginId, boost::dynamic_pointer_cast<shared::plugin::yadomsApi::IManuallyDeviceCreationRequest>(request));
   }

   //TODO ajouter sendManuallyDeviceCreationTestRequestAsync

   void CPluginGateway::doWork()
   {
      try
      {
         YADOMS_LOG_CONFIGURE("PluginGateway");
         YADOMS_LOG(debug) << "PluginGateway is starting...";

         // Signal that gateway is fully started
         m_startEventHandler.postEvent(kStartEvent);

         while(1)
         {
            // Wait for an event
            switch(m_mainEventHandler.waitForEvents())
            {
            case shared::event::kNoEvent:
               YADOMS_LOG(warning) << "CPluginGateway::doWork, unknown event received";
               //do nothing
               break;

            default:
               YADOMS_LOG(error) << "CPluginGateway : Unknown message id";
               break;
            }
         };
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "CPluginGateway is stopping..."  << std::endl;
      }
      catch(...)
      {
      }
   }







} //namespace communication
