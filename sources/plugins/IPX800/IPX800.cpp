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
#include "http/invalidHTTPResultException.hpp"

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
   bool initializationOk = false;

   std::cout << "IPX800 is starting..." << std::endl;
      
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
      std::cout << "IPX800 plugin is running..." << std::endl;
      initializationOk = true;
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      std::cerr << "IPX800 plugin initialization error..." << std::endl;
      initializationOk = false;
   }

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            std::cout << "Stop requested" << std::endl;
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
		    std::cerr << "No answer received, try to reconnect in a while..." << std::endl;
		    api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
		    break;
	     }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            try {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
            catch (...)
            {
               std::cout << "Wrong configuration update" << std::endl;
            }
            break;
         }
      case kRefreshStatesReceived:
      {
         std::cout << "Timer received" << std::endl;

         try {
            if (initializationOk)
            {
               auto forceRefresh = false;

               // Retrieve event information, if any !
               try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
               catch (shared::exception::CBadConversion&) { }

               m_ioManager->readAllIOFromDevice(api, forceRefresh);
            }
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
            std::cout << "Unknow error : " << e.what() << std::endl;
         }
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         // Yadoms asks for device creation
         if (initializationOk)
         {
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
            std::cout << "Manually device creation request received for device :" << request->getData().getDeviceName() << std::endl;
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
               std::cout << "Unknow error : " << e.what() << std::endl;
            }
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceRemoved:
      {
         if (initializationOk)
         {
            try {
               auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
               m_factory->removeDevice(api, device->device());
               m_ioManager->removeDevice(api, device->device());
               std::cout << device->device() << " was removed" << std::endl;
            }
            catch (std::exception &e)
            {
               std::cout << "Unknow error : " << e.what() << std::endl;
            }
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
                std::cerr << errorMessage << std::endl;
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Unknow error : " << e.what() << std::endl;
        }
        break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());

         try {
            if (initializationOk)
               m_ioManager->onCommand(api, command);
         }
         catch (std::exception &e)
         {
            std::cout << "Unknow error : " << e.what() << std::endl;
         }
         break;
      }
      default:
         {
            std::cerr << "Unknown message id" << std::endl;
            break;
         }
      }
   }
}

void CIPX800::initIPX800(boost::shared_ptr<yApi::IYPluginApi> api)
{
	// Send reset command to the RfxCom
	std::cout << "Init the connexion ..." << std::endl;

	try
	{
		// first reading
		m_ioManager->readAllIOFromDevice(api, true);
		m_refreshTimer->start();
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
		std::cout << "Unknow error : " << e.what() << std::endl;
	}
}

void CIPX800::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
   m_ioManager->OnConfigurationUpdate(api, m_configuration);
}