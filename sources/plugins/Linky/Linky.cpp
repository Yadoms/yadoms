#include "stdafx.h"
#include "Linky.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include "LinkyFactory.h"
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CLinky)

CLinky::CLinky():
   m_isDeveloperMode(false),
   m_runningState(ELinkyPluginState::kUndefined)
{
}

CLinky::~CLinky()
{
}

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtPortDataReceived,
   kErrorRetryTimer,
   kAnswerTimeout,
   kSamplingTimer
};

void CLinky::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Linky is starting..." ;

   m_isDeveloperMode = api->getYadomsInformation()->developperMode();

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the transceiver
   m_decoder = CLinkyFactory::constructDecoder(api);

   // Create the buffer handler
   m_receiveBufferHandler = CLinkyFactory::GetBufferHandler(api->getEventHandler(),
                                                            kEvtPortDataReceived,
                                                            m_isDeveloperMode);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(45));

   m_periodicSamplingTimer = api->getEventHandler().createTimer(kSamplingTimer,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::seconds(30));

   // Create the connection
   createConnection(api);

   // the main loop
   YADOMS_LOG(information) << "Linky plugin is running..." ;

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            m_runningState = kStop;
            return;
         }
      case kEvtPortConnection:
         {
            YADOMS_LOG(information) << "Linky plugin :  Port Connection" ;
            api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
            m_runningState = kConnecting;

            auto notif = api->getEventHandler().getEventData<boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>>();

            if (notif && notif->isConnected())
               processLinkyConnectionEvent(api);
            else
            {
               processLinkyUnConnectionEvent(api, notif);

               // attempt a new connection
               api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
            }
            break;
         }
      case kEvtPortDataReceived:
         {
			 m_waitForAnswerTimer->stop();

            if (m_isDeveloperMode) YADOMS_LOG(information) << "Linky plugin :  DataReceived" ;

            processDataReceived(api,
                                api->getEventHandler().getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string>>>>());

            m_receiveBufferHandler->desactivate();

			   //Lauch a new time the time out to detect connexion failure
			   m_waitForAnswerTimer->start();
            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            m_runningState = kupdateConfiguration;
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case kSamplingTimer:
      {
         m_receiveBufferHandler->activate();
         break;
      }
      case kErrorRetryTimer:
         {
            createConnection(api);
            break;
         }
      case kAnswerTimeout:
         {
            m_waitForAnswerTimer->stop();
            YADOMS_LOG(error) << "No answer received, try to reconnect in a while..." ;
            errorProcess(api);
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CLinky::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
   // Create the port instance
   m_port = CLinkyFactory::constructPort(m_configuration,
                                         api->getEventHandler(),
                                         m_receiveBufferHandler,
                                         kEvtPortConnection);
   m_port->start();
   m_waitForAnswerTimer->start();
}

void CLinky::destroyConnection()
{
   m_port.reset();

   m_waitForAnswerTimer->stop();
}

void CLinky::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const shared::CDataContainer& newConfigurationData)
{
   // Stop running timers, if any
   m_waitForAnswerTimer->stop();

   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration..." ;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // If plugin instance is not running, just update configuration
   if (!m_port)
   {
      // Update configuration
      m_configuration.initializeWith(newConfigurationData);
      return;
   }

   // Port has changed, destroy and recreate connection
   destroyConnection();

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);

   // Create new connection
   createConnection(api);
}

void CLinky::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages)
{
   m_decoder->decodeLinkyMessage(api, messages);

   if (m_runningState != kRunning)
   {
      api->setPluginState(yApi::historization::EPluginState::kRunning);
      m_runningState = kRunning;
   }
}

void CLinky::processLinkyConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   YADOMS_LOG(information) << "Linky port opened" ;

   try
   {
      // Init The Linky Receiver
      initLinkyReceiver();
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to Linky transceiver : " << e.what() ;
      // Disconnection will be notified, we just have to wait...
   }
}

void CLinky::processLinkyUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification)
{
   YADOMS_LOG(information) << "Linky connection was lost" ;
   if(notification)
      api->setPluginState(yApi::historization::EPluginState::kError, notification->getErrorMessageI18n(), notification->getErrorMessageI18nParameters());
   else
      api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
   m_runningState = kConnectionLost;

   destroyConnection();
}

void CLinky::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
   destroyConnection();
   m_runningState = kConnectionLost;
   api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CLinky::initLinkyReceiver() const
{
   YADOMS_LOG(information) << "Reset the Linky..." ;

   // Flush receive buffer
   m_port->flush();
}
