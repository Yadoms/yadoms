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
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization");

      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());

      m_configuration.setPath(api->getInformation()->getPath().string());

      // the main loop
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Configuring");
      m_controller = CZWaveControllerFactory::Create();
      m_controller->configure(&m_configuration, &api->getEventHandler());

      YADOMS_LOG(information) << "CZWave is initializing" ;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization");
      IZWaveController::E_StartResult initResult = m_controller->start();
      if (initResult == IZWaveController::kSuccess)
      {
         YADOMS_LOG(information) << "CZWave is running" ;
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         while (1)
         {
            // Wait for an event
            switch (api->getEventHandler().waitForEvents())
            {
            case yApi::IYPluginApi::kEventStopRequested:
            {
               YADOMS_LOG(information) << "Stop requested" ;
               api->setPluginState(yApi::historization::EPluginState::kStopped);
               StopController();
               return;
            }
            case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command was received from Yadoms
               auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

               YADOMS_LOG(information) << "Command received from Yadoms :" << yApi::IDeviceCommand::toString(command) ;
               try
               {
                  m_controller->sendCommand(command->getDevice(), command->getKeyword(), command->getBody());
               }
               catch (shared::exception::CException& ex)
               {
                  YADOMS_LOG(error) << "Fail to send command : " << ex.what() ;
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Fail to send command. exception : " << ex.what() ;
               }
               catch (...)
               {
                  YADOMS_LOG(error) << "Fail to send command. unknown exception" ;
               }
               break;
            }
            case yApi::IYPluginApi::kGetDeviceConfigurationSchemaRequest:
            {
               // Yadoms ask for device configuration schema
               // Schema can come from package.json, or built by code. In this example,
               // we just take the schema from package.json, in case of configuration is supported by device.
               auto deviceConfigurationSchemaRequest = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest> >();
               shared::CDataContainer schema = m_controller->getNodeConfigurationSchema(deviceConfigurationSchemaRequest->device());
               deviceConfigurationSchemaRequest->sendSuccess(schema);
               break;
            }

            case yApi::IYPluginApi::kSetDeviceConfiguration:
            {
               // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
               auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration> >();
               m_controller->setNodeConfiguration(deviceConfiguration->name(), deviceConfiguration->configuration());
               break;
            }

            case yApi::IYPluginApi::kEventExtraQuery:
            {
               // Command was received from Yadoms
               auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery> >();

               if (extraQuery)
               {
                  YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData().query() ;

                  if (extraQuery->getData().query() == "inclusionMode")
                  {
                     m_controller->startInclusionMode();
                  }
                  else if (extraQuery->getData().query() == "exclusionMode")
                  {
                     m_controller->startExclusionMode();
                  }
                  else if (extraQuery->getData().query() == "hardReset")
                  {
                     m_controller->hardResetController();
                  }
                  else if (extraQuery->getData().query() == "softReset")
                  {
                     m_controller->softResetController();
                  }
                  else if (extraQuery->getData().query() == "testNetwork")
                  {
                     m_controller->testNetwork(extraQuery->getData().data().get<int>("frameCount"));
                  }
                  else if (extraQuery->getData().query() == "healNetowrk")
                  {
                     m_controller->healNetwork();
                  }
                  else if (extraQuery->getData().query() == "cancelCommand")
                  {
                     m_controller->cancelCurrentCommand();
                  }
               }
               extraQuery->sendSuccess(shared::CDataContainer());
               break;
            }

            case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(information) << "Update configuration..." ;
               BOOST_ASSERT(!newConfiguration.empty()); // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               if (m_controller)
                  m_controller->stop();

               m_configuration.initializeWith(newConfiguration);

               m_controller->configure(&m_configuration, &api->getEventHandler());

               if (m_controller->start())
                  api->setPluginState(yApi::historization::EPluginState::kRunning);
               else
                  api->setPluginState(yApi::historization::EPluginState::kError);
               break;
            }

            case kDeclareDevice:
            {
               try
               {
                  auto deviceData = api->getEventHandler().getEventData<shared::CDataContainer>();
                  if (!api->deviceExists(deviceData.get<std::string>("name")))
                     api->declareDevice(deviceData.get<std::string>("name"), deviceData.get<std::string>("friendlyName"), deviceData.get<std::string>("friendlyName"), std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(), deviceData.get<shared::CDataContainer>("details"));
               }
               catch (shared::exception::CException& ex)
               {
                  YADOMS_LOG(error) << "Fail to declare device : " << ex.what() ;
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Fail to declare device. exception : " << ex.what() ;
               }
               catch (...)
               {
                  YADOMS_LOG(error) << "Fail to declare device. unknown exception" ;
               }
               break;
            }

            case kUpdateKeyword:
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
                  YADOMS_LOG(error) << "Fail to update keyword : " << ex.what() ;
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Fail to update keyword. exception : " << ex.what() ;
               }
               catch (...)
               {
                  YADOMS_LOG(error) << "Fail to update keyword. unknown exception" ;
               }
            }
            break;

            case kUpdateConfiguration:
            {
               //This case is used, when the device itself send a new configuration value (maybe after reset,...)
               try
               {
                  auto keywordData = api->getEventHandler().getEventData<boost::shared_ptr<CKeywordContainer> >();
                  YADOMS_LOG(information) << "Update configuration..." << keywordData->getKeyword()->getKeyword() << " : " << keywordData->getKeyword()->formatValue() ;
                  
                  //get the acual device configuration
                  auto config = api->getDeviceConfiguration(keywordData->getDeviceId());

                  //update the value of the event keyword
                  m_controller->updateNodeConfiguration(keywordData->getDeviceId(), keywordData->getKeyword()->getKeyword(), keywordData->getKeyword()->formatValue(), config);

                  //update device config in database
                  api->updateDeviceConfiguration(keywordData->getDeviceId(), config);

               }
               catch (shared::exception::CException& ex)
               {
                  YADOMS_LOG(error) << "Fail to update keyword : " << ex.what() ;
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Fail to update keyword. exception : " << ex.what() ;
               }
               catch (...)
               {
                  YADOMS_LOG(error) << "Fail to update keyword. unknown exception" ;
               }
            }
            break;


            case kInternalStateChange:
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
               break;
            }
            default:
            {
               YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
               break;
            }
            }
         }
         switch (initResult)
         {
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
      }

      YADOMS_LOG(error) << "Cannot start ZWave controller. Plugin ends." ;
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "The ZWave plugin fails. shared exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Shared exception : %1%") % ex.what()).str());
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "The ZWave plugin fails. exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Exception : %1%") % ex.what()).str());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "The ZWave plugin fails. unknown exception" ;
      api->setPluginState(yApi::historization::EPluginState::kError, "Unknown exception");
   }

   StopController();
}

void CZWave::StopController() const
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

