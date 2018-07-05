#include "stdafx.h"
#include "ZWave.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/exception/Exception.hpp>
#include "ZWaveControllerFactory.h"
#include "KeywordContainer.h"
#include "ZWaveInternalState.h"
#include <Poco/Thread.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>
#include "OpenZWaveHelpers.h"
#include <shared/Log.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CZWave)


CZWave::CZWave()
{
}

CZWave::~CZWave()
{
}


void CZWave::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initialization");
      
      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());
      m_configuration.setPath(api->getInformation()->getPath().string());
      m_configuration.setDataPath(api->getDataPath().string());

      bool stopRequested = false;
      while (!stopRequested)
      {
         //create the controller
         m_controller = CZWaveControllerFactory::Create();
         m_controller->configure(&m_configuration, &api->getEventHandler());

         //start controller (and analyse network)
         IZWaveController::E_StartResult initResult = m_controller->start([&] {
            //int evt;
            //do
            //{
            //   evt = api->getEventHandler().waitForEvents(boost::posix_time::min_date_time);
            //   if (evt == yApi::IYPluginApi::kEventStopRequested)
            //      stopRequested = true;
            //} while (evt != shared::event::kNoEvent && !stopRequested);
         });
         switch (initResult)
         {
         case IZWaveController::E_StartResult::kSuccess:
            mainLoop(api); //should never ends, only on stopRequested
            stopRequested = true;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            break;

         case IZWaveController::kSerialPortError:
            api->setPluginState(yApi::historization::EPluginState::kError, "failToAccessSerialPort");
            break;

         case IZWaveController::kControllerError:
            api->setPluginState(yApi::historization::EPluginState::kError, "failToStartController");
            break;

         default:
         case IZWaveController::kUnknownError:
            api->setPluginState(yApi::historization::EPluginState::kError, "failToStart");
            break;
         }

         //if an error occurs, then cleanup events, and look for stopRequest
         if (!stopRequested)
         {
            int evt = 0;
            
            do
            {
               evt = api->getEventHandler().waitForEvents(boost::posix_time::min_date_time);
               if (evt == 105)
               {
                  auto s = EZWaveInteralState(api->getEventHandler().getEventData<std::string>());
                  YADOMS_LOG(debug) << s;
               }

               if (evt == yApi::IYPluginApi::kEventStopRequested)
                  stopRequested = true;
            } while (evt != shared::event::kNoEvent && !stopRequested);

            //destroy, because will be created on next loop
            m_controller->stop();
         }
      }
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "The ZWave plugin fails. shared exception : " << ex.what();
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Shared exception : %1%") % ex.what()).str());
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "The ZWave plugin fails. exception : " << ex.what();
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Exception : %1%") % ex.what()).str());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "The ZWave plugin fails. unknown exception";
      api->setPluginState(yApi::historization::EPluginState::kError, "Unknown exception");
   }

   stopController();
}

void CZWave::mainLoop(boost::shared_ptr<yApi::IYPluginApi> api)
{
   bool stopRequested = false;
   while (!stopRequested)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         YADOMS_LOG(information) << "Stop requested" ;
         stopRequested = true;
         break;

      case yApi::IYPluginApi::kEventDeviceCommand:
         onEventDeviceCommand(api);
         break;

      case yApi::IYPluginApi::kGetDeviceConfigurationSchemaRequest:
         onDeviceConfigurationSchemaRequest(api);
         break;

      case yApi::IYPluginApi::kSetDeviceConfiguration:
         setDeviceConfiguration(api);
         break;

      case yApi::IYPluginApi::kEventExtraQuery:
         onExtraQuery(api);
         break;

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         onEventUpdateConfiguration(api);
         break;

      case kDeclareDevice:
         onDeclareDevice(api);
         break;

      case kUpdateDeviceInfo:
         onUpdateDeviceInfo(api);
         break;

      case kUpdateDeviceState:
         onUpdateDeviceState(api);
         break;

      case kUpdateKeyword:
         onUpdateKeyword(api);
         break;

      case kUpdateConfiguration:
         onUpdateConfiguration(api);
         break;

      case kInternalStateChange:
         onInternalStateChange(api);
         break;

      default:
         YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
         break;
      }
   }
}

void CZWave::stopController() const
{
   YADOMS_LOG(information) << "Ending ZWave plugin" ;
   try
   {
      if (m_controller)
         m_controller->stop();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "The ZWave plugin fail to stop. exception : " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "The ZWave plugin fail to stop. unknown exception" ;
   }
}

void CZWave::onEventDeviceCommand(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Command was received from Yadoms
   auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

   YADOMS_LOG(information) << "Command received from Yadoms :" << yApi::IDeviceCommand::toString(command);
   try
   {
      m_controller->sendCommand(command->getDevice(), command->getKeyword(), command->getBody());
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command. unknown exception";
   }
}

void CZWave::onDeviceConfigurationSchemaRequest(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Yadoms ask for device configuration schema
   // Schema can come from package.json, or built by code. In this example,
   // we just take the schema from package.json, in case of configuration is supported by device.
   auto deviceConfigurationSchemaRequest = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest> >();
   shared::CDataContainer schema = m_controller->getNodeConfigurationSchema(deviceConfigurationSchemaRequest->device());
   deviceConfigurationSchemaRequest->sendSuccess(schema);
}

void CZWave::setDeviceConfiguration(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
   auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration> >();
   m_controller->setNodeConfiguration(deviceConfiguration->name(), deviceConfiguration->configuration());
}

void CZWave::onExtraQuery(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Command was received from Yadoms
   auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery> >();

   if (extraQuery)
   {
      YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData()->query();

      if (extraQuery->getData()->query() == "inclusionMode")
      {
         m_controller->startInclusionMode();
      }
      else if (extraQuery->getData()->query() == "exclusionMode")
      {
         m_controller->startExclusionMode();
      }
      else if (extraQuery->getData()->query() == "hardReset")
      {
         m_controller->hardResetController();
      }
      else if (extraQuery->getData()->query() == "softReset")
      {
         m_controller->softResetController();
      }
      else if (extraQuery->getData()->query() == "testNetwork")
      {
         m_controller->testNetwork(extraQuery->getData()->data().get<int>("frameCount"));
      }
      else if (extraQuery->getData()->query() == "healNetowrk")
      {
         m_controller->healNetwork();
      }
      else if (extraQuery->getData()->query() == "cancelCommand")
      {
         m_controller->cancelCurrentCommand();
      }
   }
   extraQuery->sendSuccess(shared::CDataContainer());
}

void CZWave::onEventUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Configuration was updated
   api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
   auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
   YADOMS_LOG(information) << "Update configuration...";
   if (!!newConfiguration.empty())
   {
      if (m_controller)
         m_controller->stop();

      m_configuration.initializeWith(newConfiguration);

      m_controller->configure(&m_configuration, &api->getEventHandler());

      if (m_controller->start([&]{}))
      {
         api->setPluginState(yApi::historization::EPluginState::kRunning);
      }
      else
      {
         api->setPluginState(yApi::historization::EPluginState::kError);
      }
   }
   else
   {
      YADOMS_LOG(warning) << "Update configuration... failed. Configuration values are empty !";
      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }
}

void CZWave::onDeclareDevice(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      auto deviceData = api->getEventHandler().getEventData<shared::CDataContainer>();
      if (!api->deviceExists(deviceData.get<std::string>("name")))
         api->declareDevice(deviceData.get<std::string>("name"), deviceData.get<std::string>("friendlyName"), deviceData.getWithDefault<std::string>("details.product", deviceData.get<std::string>("friendlyName")), std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(), deviceData.get<shared::CDataContainer>("details"));
      else
      {
         if (deviceData.containsChild("details"))
         {
            auto dc = deviceData.get<shared::CDataContainer>("details");
            api->updateDeviceDetails(deviceData.get<std::string>("name"), dc);
         }
         if (deviceData.containsValue("details.product"))
         {
            std::string s = deviceData.get<std::string>("details.product");
            if (!s.empty())
               api->updateDeviceModel(deviceData.get<std::string>("name"), s);
         }

      }
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to declare device : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to declare device. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to declare device. unknown exception";
   }
}

void CZWave::onUpdateDeviceInfo(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      auto deviceData = api->getEventHandler().getEventData<shared::CDataContainer>();
      std::string deviceName = deviceData.get<std::string>("name");
      if (!api->deviceExists(deviceName))
         api->declareDevice(deviceName, deviceData.get<std::string>("friendlyName"), deviceData.getWithDefault<std::string>("details.product", deviceData.get<std::string>("friendlyName")), std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(), deviceData.get<shared::CDataContainer>("details"));
      else
      {
         if (deviceData.containsChild("details"))
         {
            auto dc = deviceData.get<shared::CDataContainer>("details");
            api->updateDeviceDetails(deviceData.get<std::string>("name"), dc);
         }
         if (deviceData.containsValue("details.product"))
         {
            std::string s = deviceData.get<std::string>("details.product");
            if (!s.empty())
               api->updateDeviceModel(deviceData.get<std::string>("name"), s);
         }
      }

      shared::CDataContainer details = api->getDeviceDetails(deviceName);
      shared::CDataContainer newDetails = deviceData.get<shared::CDataContainer>("details");
      auto allprops = newDetails.getAsMap();
      for (auto i = allprops.begin(); i != allprops.end(); ++i)
      {
         if (!i->second.empty())
            details.set(i->first, i->second);
      }
      api->updateDeviceDetails(deviceName, details);
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to update device : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to update device. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to update device. unknown exception";
   }
}

void CZWave::onUpdateDeviceState(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      auto deviceData = api->getEventHandler().getEventData<shared::CDataContainer>();
      std::string deviceName = deviceData.get<std::string>("name");
      shared::plugin::yPluginApi::historization::EDeviceState deviceState = deviceData.getWithDefault<shared::plugin::yPluginApi::historization::EDeviceState>("state", shared::plugin::yPluginApi::historization::EDeviceState::kUnknown);
      std::string deviceCustomMessage = deviceData.getWithDefault<std::string>("customMessage", "");
      shared::CDataContainer deviceCustomMessageDataParams = deviceData.getWithDefault<shared::CDataContainer>("customMessageDataParams", shared::CDataContainer::EmptyContainer);
      api->updateDeviceState(deviceName, deviceState, deviceCustomMessage, deviceCustomMessageDataParams.getAsMap());
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to update device state: " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to update device state. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to update device state. unknown exception";
   }
}

void CZWave::onUpdateKeyword(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      auto keywordData = api->getEventHandler().getEventData<boost::shared_ptr<CKeywordContainer> >();

      auto deviceId = keywordData->getDeviceId();
      auto keywordId = keywordData->getKeyword();

      if (!api->keywordExists(deviceId, keywordId))
         api->declareKeyword(deviceId, keywordId);

      api->historizeData(deviceId, keywordId);
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to update keyword : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to update keyword. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to update keyword. unknown exception";
   }
}

void CZWave::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api)
{
   //This case is used, when the device itself send a new configuration value (maybe after reset,...)
   try
   {
      auto keywordData = api->getEventHandler().getEventData<boost::shared_ptr<CKeywordContainer> >();
      YADOMS_LOG(information) << "Update configuration..." << keywordData->getKeyword()->getKeyword() << " : " << keywordData->getKeyword()->formatValue();

      //get the acual device configuration
      auto config = api->getDeviceConfiguration(keywordData->getDeviceId());

      //update the value of the event keyword
      m_controller->updateNodeConfiguration(keywordData->getDeviceId(), keywordData->getKeyword()->getKeyword(), keywordData->getKeyword()->formatValue(), config);

      //update device config in database
      api->updateDeviceConfiguration(keywordData->getDeviceId(), config);

   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to update keyword : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to update keyword. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to update keyword. unknown exception";
   }
}

void CZWave::onInternalStateChange(boost::shared_ptr<yApi::IYPluginApi> api)
{
   auto s = EZWaveInteralState(api->getEventHandler().getEventData<std::string>());

   switch (s)
   {
   case EZWaveInteralState::kCompletedValue:
   case EZWaveInteralState::kRunningValue:
   {
      api->setPluginState(yApi::historization::EPluginState::kRunning);

      auto & list = m_controller->getNodeList();
      for (auto i = list.begin(); i != list.end(); ++i)
      {
         std::string device = COpenZWaveHelpers::GenerateDeviceId((*i)->getHomeId(), (*i)->getNodeId());
         api->updateDeviceConfiguration(device, (*i)->getConfigurationValues());
      }
   }
   break;
   default:
      api->setPluginState(yApi::historization::EPluginState::kCustom, s.toString());
      break;
   }
}