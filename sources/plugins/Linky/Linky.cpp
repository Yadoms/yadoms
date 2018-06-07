#include "stdafx.h"
#include "Linky.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include <shared/Log.h>
#include "GPIOManager.hpp"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CLinky)

CLinky::CLinky():
   m_configuration(boost::make_shared<CLinkyConfiguration>()),
   m_isDeveloperMode(false),
   m_runningState(ELinkyPluginState::kUndefined)
{}

CLinky::~CLinky()
{}

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
   m_configuration->initializeWith(api->getConfiguration());

   m_GPIOManager = boost::make_shared<CGPIOManager>(m_configuration);

   // For port 0 & 1
   m_protocolManager[0] = boost::make_shared<CProtocolManager>();
   m_protocolManager[1] = boost::make_shared<CProtocolManager>();

   // Create decoders
   m_decoder[0] = CLinkyFactory::constructDecoder(EProtocolType::Standard, api);
   m_decoder[1] = CLinkyFactory::constructDecoder(EProtocolType::Standard, api);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(5));

   m_periodicSamplingTimer = api->getEventHandler().createTimer(kSamplingTimer,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::seconds(30));

   m_waitForAnswerTimer->stop();

   // Create the connection
   createConnection(api);

   // For immediat sampling
   m_periodicSamplingTimer = api->getEventHandler().createTimer(kSamplingTimer,
                                                                shared::event::CEventTimer::kOneShot,
                                                                boost::posix_time::seconds(0));

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
            setPluginState(api, kConnecting);

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
          m_protocolManager[m_GPIOManager->getGPIO()-1]->validateProtocol();
          YADOMS_LOG(trace) << "Linky plugin :  DataReceived";

            processDataReceived(api,
                                api->getEventHandler().getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string>>>>());

            CLinkyFactory::FTDI_DisableGPIO(m_port);
            m_receiveBufferHandler->desactivate();

            if (!m_GPIOManager->isLast())
            {
               // Go to the next GPIO & fire it rapidly
               m_GPIOManager->next();
               api->getEventHandler().createTimer(kSamplingTimer,
                                                  shared::event::CEventTimer::kOneShot,
                                                  boost::posix_time::seconds(1));
            }
            else // Return to the beginning of the list
               m_GPIOManager->front();

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            setPluginState(api, kupdateConfiguration);
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            setPluginState(api, kRunning);
            break;
         }
      case kSamplingTimer:
      {
         auto detectedProtocol = m_protocolManager[m_GPIOManager->getGPIO() - 1]->getProtocol();
         CLinkyFactory::FTDI_setNewProtocol(m_port, detectedProtocol);
         CLinkyFactory::FTDI_ActivateGPIO(m_port, m_GPIOManager->getGPIO());

         m_receiveBufferHandler->changeProtocol(detectedProtocol);
         m_receiveBufferHandler->activate();

         //Lauch a new time the time out to detect connexion failure
         m_waitForAnswerTimer->start();
         break;
      }
      case kErrorRetryTimer:
         {
            try {
               createConnection(api);
               m_periodicSamplingTimer->start();
            }
            catch (std::exception &e)
            {
               YADOMS_LOG(trace) << e.what();
               errorProcess(api);
            }
            break;
         }
      case kAnswerTimeout:
         {
            m_periodicSamplingTimer->stop();
            CLinkyFactory::FTDI_DisableGPIO(m_port);
            YADOMS_LOG(information) << "No answer received, try to reconnect in a while..." ;

            if (!m_protocolManager[m_GPIOManager->getGPIO() - 1]->end())
               changeProtocol(api);
            else
            {
               errorProcess(api);

               //Reset the protocol manager of the selected GPIO
               if (!m_protocolManager[m_GPIOManager->getGPIO() - 1]->isValidated()) {
                  m_protocolManager[m_GPIOManager->getGPIO() - 1]->changeProtocol();
                  m_decoder[m_GPIOManager->getGPIO() - 1] = CLinkyFactory::constructDecoder(m_protocolManager[m_GPIOManager->getGPIO() - 1]->getProtocol(), api);
               }

               // We continue to scan the other port if any
               if (!m_GPIOManager->isLast())
                  m_GPIOManager->next();
               else
                  m_GPIOManager->front();
            }
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
   setPluginState(api, kConnecting);

   auto detectedProtocol = m_protocolManager[m_GPIOManager->getGPIO() - 1]->getProtocol();
   m_receiveBufferHandler = CLinkyFactory::GetBufferHandler(detectedProtocol,
                                                            api->getEventHandler(),
                                                            kEvtPortDataReceived,
                                                            m_isDeveloperMode);

   // Create the port instance
   m_port = CLinkyFactory::constructPort(detectedProtocol,
                                         m_configuration,
                                         api->getEventHandler(),
                                         m_receiveBufferHandler,
                                         kEvtPortConnection);

   m_port->start();
}

void CLinky::destroyConnection()
{
   m_periodicSamplingTimer->stop();
   m_waitForAnswerTimer->stop();
   m_port.reset();
}

void CLinky::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration..." ;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // If plugin instance is not running, just update configuration
   if (!m_port)
   {
      // Update configuration
      m_configuration->initializeWith(newConfigurationData);
      m_GPIOManager = boost::make_shared<CGPIOManager>(m_configuration);
      return;
   }
   else
   {
      // Port has changed, destroy and recreate connection
      destroyConnection();

      // Update configuration
      m_configuration->initializeWith(newConfigurationData);
      m_GPIOManager = boost::make_shared<CGPIOManager>(m_configuration);

      // Create a new time protocol manager, to reset them
      m_protocolManager[0] = boost::make_shared<CProtocolManager>();
      m_protocolManager[1] = boost::make_shared<CProtocolManager>();

      // Create new connection
      createConnection(api);
      m_periodicSamplingTimer->start();
   }
}

void CLinky::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages)
{
   auto position = m_GPIOManager->getGPIO() - 1;

   // Select the decoder from the protocol detected
   m_decoder[position]->decodeMessage(api, messages);

   if (!m_decoder[position]->isERDFCounterDesactivated())
      setPluginState(api, kRunning);
   else
      setPluginState(api, kErDFCounterdesactivated);
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
   if (notification)
      setPluginErrorState(api, notification->getErrorMessageI18n(), notification->getErrorMessageI18nParameters());
   else
      setPluginState(api, kConnectionLost);

   destroyConnection();
}

void CLinky::changeProtocol(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_protocolManager[m_GPIOManager->getGPIO() - 1]->changeProtocol();
   m_decoder[m_GPIOManager->getGPIO() - 1] = CLinkyFactory::constructDecoder(m_protocolManager[m_GPIOManager->getGPIO() - 1]->getProtocol(), api);
   destroyConnection();
   api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
}

void CLinky::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   destroyConnection();
   setPluginState(api, kConnectionLost);
   api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
}

void CLinky::initLinkyReceiver() const
{
   YADOMS_LOG(information) << "Reset the Linky..." ;

   // Flush receive buffer
   m_port->flush();
}

void CLinky::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, ELinkyPluginState newState)
{
   if (m_runningState != newState)
   {
      switch (newState)
      {
      case ELinkyPluginState::kErDFCounterdesactivated:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "ErDFCounterdesactivated");
         break;
      case ELinkyPluginState::kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case ELinkyPluginState::kConnectionLost:
         api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
         break;
      case ELinkyPluginState::kConnecting:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
         break;
      case ELinkyPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case ELinkyPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
         break;
      }

      m_runningState = newState;
   }
}

void CLinky::setPluginErrorState(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& ErrorMessageI18n,
                                 const std::map<std::string, std::string>& ErrorMessageI18nParameters)
{
   if (m_runningState != kError)
   {
      m_runningState = kError;
      api->setPluginState(yApi::historization::EPluginState::kError, ErrorMessageI18n, ErrorMessageI18nParameters);
   }
}