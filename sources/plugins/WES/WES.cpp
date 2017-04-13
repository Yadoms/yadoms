#include "stdafx.h"
#include "WES.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>

#include "equipments/manuallyDeviceCreationException.hpp"
#include "equipments/noInformationException.hpp"
#include <shared/Log.h>

#include <boost/property_tree/xml_parser.hpp>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWES)


CWES::CWES():
   m_factory(boost::make_shared<CWESFactory>()),
   m_configuration(boost::make_shared<CWESConfiguration>())
{}

CWES::~CWES()
{}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kRefreshStatesReceived,
   kConnectionRetryTimer,
   kAnswerTimeout
};

void CWES::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try {
      setPluginState(api, EWESPluginState::kInitialization);
      m_configuration->initializeWith(api->getConfiguration());
      m_ioManager = m_factory->loadConfiguration(api, m_configuration);

      if (m_ioManager->getMasterEquipment() == 0)
         setPluginState(api, EWESPluginState::kReady);
      else
      {
         m_ioManager->readAllDevices(api, true); // first reading
         setPluginState(api, EWESPluginState::kRunning);
         // Create the timer for refresh IOs
         m_refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));
      }
   }
   catch (std::exception &e)
   {
      setPluginState(api, EWESPluginState::kInitializationError);
   }

   // the main loop
   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
      {
         YADOMS_LOG(information) << "Stop requested";
         setPluginState(api, EWESPluginState::kStop);
         return;
      }
      case kConnectionRetryTimer:
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         setPluginState(api, EWESPluginState::kupdateConfiguration);
         onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
         setPluginState(api, EWESPluginState::kRunning);
         break;
      }
      case kRefreshStatesReceived:
      {
         YADOMS_LOG(information) << "Timer received";

         auto forceRefresh = false;

         try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
         catch (shared::exception::CBadConversion&) {}

         m_ioManager->readAllDevices(api, forceRefresh);
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         // Yadoms asks for device creation
         setPluginState(api, EWESPluginState::kupdateConfiguration);
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
         try
         {
            // Refuse the device if the name already exist

            // Creation of the device
            request->sendSuccess(m_factory->createDeviceManually(api, m_ioManager, request->getData(), m_configuration));

            // ask immediately for reading values
            m_refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            setPluginState(api, EWESPluginState::kRunning);
         }
         catch (CManuallyDeviceCreationException& e)
         {
            request->sendError(e.what());
            setPluginState(api, EWESPluginState::kInitializationError);
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
            setPluginState(api, EWESPluginState::kInitializationError);
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceRemoved:
      {
         try {
            auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
            m_ioManager->removeDevice(api, device->device());
            YADOMS_LOG(information) << device->device() << " is removed";
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
         try {
            auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest> >();

            if (data->getData().getQuery() == "wes")
            {
               data->sendSuccess(m_ioManager->bindMasterDevice());
            }
            else
            {
               std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
               data->sendError(errorMessage);
               YADOMS_LOG(error) << errorMessage;
            }
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kEventExtraQuery:
      {
         // TODO : To be developped when received directly information from the WES
         // TODO : Check the IP address to know how send us data

         // Extra-command was received from Yadoms
         auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();

         if (extraQuery)
         {
            YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData().query();

            if (extraQuery->getData().query() == "wes")
            {
               YADOMS_LOG(information) << "Simple command received";
            }
         }
         // Extra-query can return success or error indicator. In case of success, can also return data.
         // Return here a success without data (=empty container)
         extraQuery->sendSuccess(shared::CDataContainer());
         break;
      }
      case yApi::IYPluginApi::kSetDeviceConfiguration:
      {
         // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
         auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

         setPluginState(api, EWESPluginState::kupdateConfiguration);
         m_ioManager->OnDeviceConfigurationUpdate(api, 
                                                  deviceConfiguration->device(),
                                                  deviceConfiguration->configuration());

         setPluginState(api, EWESPluginState::kRunning);

         break;
      }
      default:
      {
         YADOMS_LOG(information) << "Unknown message id received";
         break;
      }
      }
   }
}

void CWES::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);
}

void CWES::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, EWESPluginState newState)
{
   if (m_pluginState != newState)
   {
      switch (newState)
      {
      case EWESPluginState::kInitialization:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "initialization");
         break;
      case EWESPluginState::kInitializationError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
         break;
      case EWESPluginState::kReady:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "ready");
         break;
      case EWESPluginState::kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case EWESPluginState::kAtLeastOneConnectionFaulty:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
         break;
      case EWESPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case EWESPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
         break;
      }

      m_pluginState = newState;
   }
}