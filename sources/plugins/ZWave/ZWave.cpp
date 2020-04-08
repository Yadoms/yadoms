#include "stdafx.h"
#include "ZWave.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/exception/Exception.hpp>
#include "ZWaveControllerFactory.h"
#include "KeywordContainer.h"
#include "ZWaveInternalState.h"
#include <Poco/Thread.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>
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

      //check for version upgrade (allow to cleanup data folder when a new version run)
      checkVersionUpgrade(api);

      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());
      m_configuration.setPath(api->getInformation()->getPath().string());
      m_configuration.setDataPath(api->getDataPath().string());

      bool stopRequested = false;
      while (!stopRequested)
      {
         //create the controller
         m_controller = CZWaveControllerFactory::Create();
         shared::event::CEventHandler init;
         m_controller->configure(&m_configuration, &init, api->getYadomsInformation()->developperMode());

         //start controller (and analyse network)
         const IZWaveController::E_StartResult initResult = m_controller->start([&] {
            int evt;
            do
            {
               evt = api->getEventHandler().waitForEvents(boost::posix_time::min_date_time);
               if (evt == yApi::IYPluginApi::kEventStopRequested)
                  stopRequested = true;
               else
               {
                  if (evt != shared::event::kNoEvent)
                     api->getEventHandler().transferLastEvent(init);
               }
            } while (evt != shared::event::kNoEvent && !stopRequested);
         });
         m_controller->configure(&m_configuration, &api->getEventHandler(), api->getYadomsInformation()->developperMode());
         switch (initResult)
         {
         case IZWaveController::E_StartResult::kSuccess:
            int evt;
            do
            {
               evt = init.waitForEvents(boost::posix_time::min_date_time);
               if(evt != shared::event::kNoEvent)
                  init.transferLastEvent(api->getEventHandler());
            } while (evt != shared::event::kNoEvent);

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
            int evt;
            
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

void CZWave::mainLoop(const boost::shared_ptr<yApi::IYPluginApi>& api)
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

      case kUserAlert:
         onUserAlert(api);
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

void CZWave::checkVersionUpgrade(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   /**
    * In case of version upgrade, OpenZWave data folder must be erased.
    *
    * To determine it, a file named "currentVersion.json" in data folder contains the plugin version of the last time it started.
    *
    * If version is obsolete (or file do not exist or do not contains a version entry) then erase data folder
    *
    * In all cases, just write the current verion in file; so at next start nothing will be done.
    * 
    */
   YADOMS_LOG(information) << "The ZWave plugin starting... checking version upgrade...";

   shared::versioning::CSemVer currentRunningVersion = api->getInformation()->getVersion();

   const auto dataVersionFile = api->getDataPath() / "currentVersion.json";
   shared::CDataContainer dvc;

   if(boost::filesystem::exists(dataVersionFile))
   {
      try
      {
         dvc.deserializeFromFile(dataVersionFile.string());
         if (dvc.containsValue("version"))
         {
            const auto versionAsString = dvc.get<std::string>("version");
            shared::versioning::CSemVer localVersion(versionAsString);
            if (currentRunningVersion > localVersion)
            {
               try
               {
                  YADOMS_LOG(information) << "The ZWave plugin started after an update. Cleaning " << api->getDataPath().string() << " folder...";
                  //an upgrade havve benn realized
                  //cleanup data folder
                  boost::filesystem::remove_all(api->getDataPath());
                  boost::filesystem::create_directory(api->getDataPath());

                  YADOMS_LOG(debug) << "The ZWave plugin started after an update. Cleaning " << api->getDataPath().string() << " folder... DONE";
               }
               catch (std::exception & ex)
               {
                  YADOMS_LOG(error) << "The ZWave plugin started after an update. Cleaning " << api->getDataPath().string() << " folder... FAIL";
                  YADOMS_LOG(error) << "The ZWave plugin fail to manage data folder after an update : " << ex.what();
               }
            }
            else
            {
               YADOMS_LOG(debug) << "The ZWave plugin starting... checking version upgrade... data folder already at last running version";
            }
         }
         else
         {
            YADOMS_LOG(debug) << "The ZWave plugin starting... checking version upgrade... cant find 'VERSION' in " << dataVersionFile << " : ";
            dvc.printToLog(YADOMS_LOG(debug));

         }

      }
      catch (std::exception & ex)
      {
         YADOMS_LOG(debug) << "Fail to parse " << dataVersionFile.string() << " : " << ex.what();
      }
   }
   else
   {
      YADOMS_LOG(debug) << "The ZWave plugin starting... checking version upgrade... cant find " << dataVersionFile << ". Consider new install, nothing to do";
   }

   try
   {
      YADOMS_LOG(debug) << "The ZWave plugin starting... checking version upgrade... writing current version " << currentRunningVersion;
      dvc.set("version", currentRunningVersion.toString());
      dvc.serializeToFile(dataVersionFile.string());

      YADOMS_LOG(debug) << "The ZWave plugin starting... checking version upgrade... DONE";
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(error) << "The ZWave plugin starting... checking version upgrade... writing current version FAILED " << ex.what();
   }
}


void CZWave::onEventDeviceCommand(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   // Command was received from Yadoms
   const auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

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

void CZWave::onDeviceConfigurationSchemaRequest(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   // Yadoms ask for device configuration schema
   // Schema can come from package.json, or built by code. In this example,
   // we just take the schema from package.json, in case of configuration is supported by device.
   auto deviceConfigurationSchemaRequest = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest> >();
   const shared::CDataContainerSharedPtr schema = m_controller->getNodeConfigurationSchema(deviceConfigurationSchemaRequest->device());
   deviceConfigurationSchemaRequest->sendSuccess(schema);
}

void CZWave::setDeviceConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
   const auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration> >();
   m_controller->setNodeConfiguration(deviceConfiguration->name(), deviceConfiguration->configuration());
}

void CZWave::onExtraQuery(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   // Command was received from Yadoms
   auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery> >();

   if (extraQuery)
   {
      const std::string targetDevice = extraQuery->getData()->device();
      if (targetDevice.empty())
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
            m_controller->testNetwork(extraQuery->getData()->data()->get<int>("frameCount"));
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
      else
      {
         YADOMS_LOG(information) << "device " << targetDevice << "  : extra command received : " << extraQuery->getData()->query();
         m_controller->onDeviceExtraQuery(targetDevice, extraQuery->getData()->query(), extraQuery->getData()->data());
      }
   }
   extraQuery->sendSuccess(new_CDataContainerSharedPtr());
}

void CZWave::onEventUpdateConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api)
{
   // Configuration was updated
   api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
   const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
   YADOMS_LOG(information) << "Update configuration...";
   if (!!newConfiguration->empty())
   {
      if (m_controller)
         m_controller->stop();

      m_configuration.initializeWith(newConfiguration);

      m_controller->configure(&m_configuration, &api->getEventHandler(), api->getYadomsInformation()->developperMode());

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

void CZWave::onDeclareDevice(const boost::shared_ptr<yApi::IYPluginApi>& api)
{
   shared::CDataContainerSharedPtr deviceData;
   try
   {
      deviceData = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
      if (!api->deviceExists(deviceData->get<std::string>("name")))
         api->declareDevice(deviceData->get<std::string>("name"), deviceData->get<std::string>("friendlyName"), deviceData->getWithDefault<std::string>("details.product", deviceData->get<std::string>("friendlyName")), std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(), deviceData->get<shared::CDataContainerSharedPtr>("details"));
      else
      {
         if (deviceData->containsChild("details"))
         {
            auto dc = deviceData->get<shared::CDataContainerSharedPtr>("details");
            api->updateDeviceDetails(deviceData->get<std::string>("name"), dc);
         }
         if (deviceData->containsValue("details.product"))
         {
            std::string s = deviceData->get<std::string>("details.product");
            if (!s.empty())
               api->updateDeviceModel(deviceData->get<std::string>("name"), s);
         }

      }
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Fail to declare device : " << ex.what();
   }
   catch (std::exception& ex)
   {
      try 
      { 
         deviceData->printToLog(YADOMS_LOG(error)); 
      }
      catch (std::exception &e)
      { 
         YADOMS_LOG(error) << "Unable to display deviceData content" << e.what(); 
      }
      catch (...) 
      { 
         YADOMS_LOG(error) << "Unable to display deviceData content"; 
      }

      YADOMS_LOG(error) << "Fail to declare device. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to declare device. unknown exception";
   }
}

void CZWave::onUpdateDeviceInfo(const boost::shared_ptr<yApi::IYPluginApi>& api)
{
   try
   {
      auto deviceData = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
      std::string deviceName = deviceData->get<std::string>("name");
      if (!api->deviceExists(deviceName))
         api->declareDevice(deviceName, deviceData->get<std::string>("friendlyName"), deviceData->getWithDefault<std::string>("details.product", deviceData->get<std::string>("friendlyName")), std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(), deviceData->get<shared::CDataContainerSharedPtr>("details"));
      else
      {
         if (deviceData->containsChild("details"))
         {
            auto dc = deviceData->get<shared::CDataContainerSharedPtr>("details");
            api->updateDeviceDetails(deviceData->get<std::string>("name"), dc);
         }
         if (deviceData->containsValue("details.product"))
         {
            std::string s = deviceData->get<std::string>("details.product");
            if (!s.empty())
               api->updateDeviceModel(deviceData->get<std::string>("name"), s);
         }
      }

      shared::CDataContainerSharedPtr details = api->getDeviceDetails(deviceName);
      shared::CDataContainerSharedPtr newDetails = deviceData->get<shared::CDataContainerSharedPtr>("details");
      auto allprops = newDetails->getAsMap<std::string>();
      for (auto i = allprops.begin(); i != allprops.end(); ++i)
      {
         if (!i->second.empty())
            details->set(i->first, i->second);
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

void CZWave::onUpdateDeviceState(const boost::shared_ptr<yApi::IYPluginApi>& api)
{
   try
   {
      auto deviceData = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
      std::string deviceName = deviceData->get<std::string>("name");
      shared::plugin::yPluginApi::historization::EDeviceState deviceState = deviceData->getWithDefault<shared::plugin::yPluginApi::historization::EDeviceState>("state", shared::plugin::yPluginApi::historization::EDeviceState::kUnknown);
      std::string deviceCustomMessage = deviceData->getWithDefault<std::string>("customMessage", "");
      shared::CDataContainerSharedPtr deviceCustomMessageDataParams = deviceData->getWithDefault<shared::CDataContainerSharedPtr>("customMessageDataParams", new_CDataContainerSharedPtr());
      api->updateDeviceState(deviceName, deviceState, deviceCustomMessage, deviceCustomMessageDataParams->getAsMap<std::string>());
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

void CZWave::onUpdateKeyword(const boost::shared_ptr<yApi::IYPluginApi>& api)
{
   try
   {
      const auto keywordData = api->getEventHandler().getEventData<boost::shared_ptr<CKeywordContainer> >();
      const auto deviceId = keywordData->getDeviceId();
      const auto keywordId = keywordData->getKeyword();

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

void CZWave::onUpdateConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   //This case is used, when the device itself send a new configuration value (maybe after reset,...)
   try
   {
      const auto keywordData = api->getEventHandler().getEventData<boost::shared_ptr<CKeywordContainer> >();
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

void CZWave::onInternalStateChange(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   const auto s = EZWaveInteralState(api->getEventHandler().getEventData<std::string>());

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


void CZWave::onUserAlert(const boost::shared_ptr<yApi::IYPluginApi>& api) const
{
   const auto userAlert = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
   const std::string alertType = userAlert->get("type");
   std::map<std::string, std::string> alertData = std::map<std::string, std::string>();
   if(userAlert->containsValue("data"))
   {
      alertData = userAlert->getAsMap<std::string>("data");
      
   }
   api->setPluginState(yApi::historization::EPluginState::kCustom, alertType, alertData);
}
