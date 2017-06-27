#include "stdafx.h"
#include "IPX800.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>

#include "equipments/manuallyDeviceCreationException.hpp"
#include "equipments/noInformationException.hpp"
#include "http/failedSendingException.hpp"
#include <shared/Log.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CIPX800)


CIPX800::CIPX800()
   : m_deviceName("IPX800")
{}

CIPX800::~CIPX800()
{}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kRefreshStatesReceived,
   kConnectionRetryTimer,
   kAnswerTimeout
};

void CIPX800::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "IPX800 is starting..." ;
      
   try {
      m_configuration.initializeWith(api->getConfiguration());

      //Factory : Creation of all the needed
      m_factory = boost::make_shared<CIPX800Factory>(api, m_deviceName, m_configuration);

      m_ioManager = m_factory->getIOManager();

	  // First connection
	  api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(00));
      
      // Timer used to read periodically IOs from the IPX800
	  m_refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));
	  m_refreshTimer->stop();

      api->setPluginState(yApi::historization::EPluginState::kRunning);
      YADOMS_LOG(information) << "IPX800 plugin is running..." ;
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      YADOMS_LOG(error) << "IPX800 plugin initialization error..." ;
   }

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
	  case kConnectionRetryTimer:
	     {
		    initIPX800(api);
		    break;
	     }
	  case kAnswerTimeout:
	     {
		    api->setPluginState(yApi::historization::EPluginState::kCustom, "noConnection");
		    m_refreshTimer->stop();
		    YADOMS_LOG(error) << "No answer received, try to reconnect in a while..." ;
		    api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
		    break;
	     }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            try {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
            }
            catch (...)
            {
               YADOMS_LOG(information) << "Wrong configuration update" ;
            }
            break;
         }
      case kRefreshStatesReceived:
      {
         YADOMS_LOG(information) << "Timer received" ;

         try {
            auto forceRefresh = false;

               try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
            catch (shared::exception::CBadConversion&) { }

            m_ioManager->readAllIOFromDevice(api, forceRefresh);
         }
         catch (CFailedSendingException&)
         {
			api->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         }
         catch (CNoInformationException&)
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "noInformation");
         }
         catch (std::exception &e) // final catch for other reason
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
         }
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         //TODO revoir la création de device
         // Yadoms asks for device creation
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         YADOMS_LOG(information) << "Manually device creation request received for device :" << request->getData().getDeviceName() ;
         try
         {
            // Creation of the device
            request->sendSuccess(m_factory->createDeviceManually(api, request->getData()));

            // Send an event to refresh all IOs
            api->getEventHandler().postEvent<bool>(kRefreshStatesReceived, true);
         }
         catch (CManuallyDeviceCreationException& e)
         {
            request->sendError(e.what());
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceRemoved:
      {
         try {
            auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
            m_factory->removeDevice(api, device->device());
            m_ioManager->removeDevice(api, device->device());
            YADOMS_LOG(information) << device->device() << " was removed" ;
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
         }
         break;
      }
      case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
        try {
            auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest> >();

            if (data->getData().getQuery() == "X8R")
                data->sendSuccess(m_factory->bindSlotsX8R());
            else if (data->getData().getQuery() == "X8D")
                data->sendSuccess(m_factory->bindSlotsX8D());
            else if (data->getData().getQuery() == "X24D")
                data->sendSuccess(m_factory->bindSlotsX24D());
            else
            {
                std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
                data->sendError(errorMessage);
                YADOMS_LOG(error) << errorMessage ;
            }
        }
        catch (std::exception &e)
        {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
        }
        break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());

         try
         {
            m_ioManager->onCommand(api, command);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kSetDeviceConfiguration:
      {
         // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
         auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

         m_factory->onDeviceConfigurationChange(deviceConfiguration->name(), deviceConfiguration->configuration());
         break;
      }
      default:
         {
            YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CIPX800::initIPX800(boost::shared_ptr<yApi::IYPluginApi> api) const
{
	YADOMS_LOG(information) << "Init the connexion ..." ;

	try
	{
		// first reading
		m_ioManager->readAllIOFromDevice(api, true);
		m_refreshTimer->start();
      api->setPluginState(yApi::historization::EPluginState::kRunning);
	}
	catch (CFailedSendingException&)
	{
		api->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
	}
	catch (CNoInformationException&)
	{
		api->setPluginState(yApi::historization::EPluginState::kCustom, "noInformation");
	}
	catch (std::exception &e) // final catch for other reason
	{
		YADOMS_LOG(information) << "Unknow error : " << e.what() ;
	}
}

void CIPX800::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration..." ;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
   m_ioManager->OnConfigurationUpdate(api, m_configuration);
}