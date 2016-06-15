#include "stdafx.h"
#include "ZWave.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include "ZWaveControllerFactory.h"
#include "KeywordContainer.h"
#include "ZWaveInternalState.h"

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


void CZWave::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG_CONFIGURE("ZWave");

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      m_configuration.setPath(context->getPluginPath().string());

      // the main loop
      context->setPluginState(yApi::historization::EPluginState::kCustom, "Configuring");
      m_controller = CZWaveControllerFactory::Create();
      m_controller->configure(&m_configuration, &context->getEventHandler());

      YADOMS_LOG(debug) << "CZWave is initializing";
      context->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization");
      IZWaveController::E_StartResult initResult = m_controller->start();
      if (initResult == IZWaveController::kSuccess)
      {
         YADOMS_LOG(debug) << "CZWave is running";
         context->setPluginState(yApi::historization::EPluginState::kRunning);
         while (1)
         {
            // Wait for an event
            switch (context->getEventHandler().waitForEvents())
            {
            case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command was received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

               YADOMS_LOG(debug) << "Command received from Yadoms :" << command->toString();
               try
               {
                  m_controller->sendCommand(command->getTargetDevice(), command->getKeyword(), command->getBody());
               }
               catch (shared::exception::CException & ex)
               {
                  YADOMS_LOG(error) << "Fail to send command : " << ex.what();
               }
               catch (std::exception & ex)
               {
                  YADOMS_LOG(fatal) << "Fail to send command. exception : " << ex.what();
               }
               catch (...)
               {
                  YADOMS_LOG(fatal) << "Fail to send command. unknown exception";
               }
               break;
            }
            case yApi::IYPluginApi::kEventExtraCommand:
            {
               // Command was received from Yadoms
               boost::shared_ptr<const yApi::IExtraCommand> extraCommand = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IExtraCommand> >();

               if (extraCommand)
               {
                  YADOMS_LOG(debug) << "Extra command received : " << extraCommand->getCommand();

                  if (extraCommand->getCommand() == "inclusionMode")
                  {
                     m_controller->startInclusionMode();
                  } 
                  else if (extraCommand->getCommand() == "exclusionMode")
                  {
                     m_controller->startExclusionMode();
                  }
                  else if (extraCommand->getCommand() == "hardReset")
                  {
                     m_controller->hardResetController();
                  }
                  else if (extraCommand->getCommand() == "softReset")
                  {
                     m_controller->softResetController();
                  }
                  else if (extraCommand->getCommand() == "testNetwork")
                  {
                     m_controller->testNetwork(extraCommand->getData().get<int>("frameCount"));
                  }
                  else if (extraCommand->getCommand() == "healNetowrk")
                  {
                     m_controller->healNetwork();
                  }
                  else if (extraCommand->getCommand() == "cancelCommand")
                  {
                     m_controller->cancelCurrentCommand();
                  }
               }
               break;
            }

            case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               context->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Update configuration...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               if (m_controller)
                  m_controller->stop();

               m_configuration.initializeWith(newConfiguration);

               m_controller->configure(&m_configuration, &context->getEventHandler());

               if (m_controller->start())
                   context->setPluginState(yApi::historization::EPluginState::kRunning);
               else
                  context->setPluginState(yApi::historization::EPluginState::kError);
               break;
            }
            case kDeclareDevice:
            {
               try
               {
                  shared::CDataContainer deviceData = context->getEventHandler().getEventData<shared::CDataContainer>();
                  if (!context->deviceExists(deviceData.get<std::string>("name")))
                     context->declareDevice(deviceData.get<std::string>("name"), deviceData.get<std::string>("friendlyName"), deviceData.get<shared::CDataContainer>("details"));
               }
               catch (shared::exception::CException & ex)
               {
                  YADOMS_LOG(error) << "Fail to declare device : " << ex.what();
               }
               catch (std::exception & ex)
               {
                  YADOMS_LOG(fatal) << "Fail to declare device. exception : " << ex.what();
               }
               catch (...)
               {
                  YADOMS_LOG(fatal) << "Fail to declare device. unknown exception";
               }
               break;
            }

            case kUpdateKeyword:
            {
               try
               {

                  boost::shared_ptr<CKeywordContainer> keywordData = context->getEventHandler().getEventData< boost::shared_ptr<CKeywordContainer> >();

                  std::string deviceId = keywordData->getDeviceId();
                  std::string keywordId = keywordData->getKeyword().getKeyword();

                  if (!context->keywordExists(deviceId, keywordId))
                     context->declareKeyword(deviceId, keywordData->getKeyword());

                  context->historizeData(deviceId, keywordData->getKeyword());
               }
               catch (shared::exception::CException & ex)
               {
                  YADOMS_LOG(error) << "Fail to update keyword : " << ex.what();
               }
               catch (std::exception & ex)
               {
                  YADOMS_LOG(fatal) << "Fail to update keyword. exception : " << ex.what();
               }
               catch (...)
               {
                  YADOMS_LOG(fatal) << "Fail to update keyword. unknown exception";
               }
            }
            break;

            case kInternalStateChange:
            {
               EZWaveInteralState s = EZWaveInteralState(context->getEventHandler().getEventData<std::string>());

               switch (s)
               {
               case EZWaveInteralState::kCompletedValue:
               case EZWaveInteralState::kRunningValue:
                  context->setPluginState(yApi::historization::EPluginState::kRunning);
                  break;
               default:
                  context->setPluginState(yApi::historization::EPluginState::kCustom, s.toString());
                  break;
               }
               break;
            }
            default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
            }
         };
      }
      else
      {
         YADOMS_LOG(error) << "Cannot start ZWave controller. Plugin ends.";
         switch (initResult)
         {
         case IZWaveController::kSerialPortError:
            context->setPluginState(yApi::historization::EPluginState::kError, "failToAccessSerialPort");
            break;

         case IZWaveController::kControllerError:
            context->setPluginState(yApi::historization::EPluginState::kError, "failToStartController");
            break;

         default:
         case IZWaveController::kUnknownError:
            context->setPluginState(yApi::historization::EPluginState::kError, "failToStart");
            break;
         }
      }

   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
   }
   catch (shared::exception::CException & ex)
   {
      YADOMS_LOG(fatal) << "The ZWave plugin fails. shared exception : " << ex.what();
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(fatal) << "The ZWave plugin fails. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(fatal) << "The ZWave plugin fails. unknown exception";
   }

   YADOMS_LOG(information) << "Ending ZWave plugin";
   try
   {
      if(m_controller)
         m_controller->stop();
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(fatal) << "The ZWave plugin fail to stop. exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(fatal) << "The ZWave plugin fail to stop. unknown exception";
   }

}



