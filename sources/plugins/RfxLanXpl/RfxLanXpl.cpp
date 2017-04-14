#include "stdafx.h"
#include "RfxLanXpl.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/exception/Exception.hpp>
#include "xplcore/XplService.h"
#include "xplcore/XplHub.h"
#include "xplcore/XplConstants.h"
#include "xplrules/IReadRule.h"
#include "xplrules/ICommandRule.h"
#include "xplrules/rfxLanXpl/DeviceManager.h"
#include "xplrules/ISupportManuallyDeviceCreationRule.h"
#include "xplrules/DeviceContainer.h"
#include <shared/Log.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CRfxLanXpl)

std::string CRfxLanXpl::m_xpl_gateway_id = "xplplug";

CRfxLanXpl::CRfxLanXpl()
   : m_deviceManager(boost::make_shared<xplrules::rfxLanXpl::CDeviceManager>())
{
}

CRfxLanXpl::~CRfxLanXpl()
{
}

// Event IDs
enum
{
   kXplMessageReceived = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kXplHubFound,
};

void print(shared::CDataContainer const& pt)
{
   YADOMS_LOG(information) << pt.serialize() ;
}

void CRfxLanXpl::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      YADOMS_LOG(information) << "################ RfxLanXpl : PRINT CONFIG" ;

      print(api->getConfiguration());

      // Load configuration values (provided by database)
      YADOMS_LOG(information) << "################ RfxLanXpl : SET CONFIG" ;
      m_configuration.initializeWith(api->getConfiguration());

      //start ioservice
      YADOMS_LOG(information) << "################ RfxLanXpl : NETWORK IFACE" ;
      auto interface = m_configuration.getXplNetworkInterface();

      YADOMS_LOG(information) << "RfxLanXpl : create xpl service" ;
      xplcore::CXplService xplService(interface, m_xpl_gateway_id, "1", &api->getEventHandler(), kXplHubFound);
      YADOMS_LOG(information) << "RfxLanXpl : subscribe for xpl messages" ;
      xplService.subscribeForAllMessages(&api->getEventHandler(), kXplMessageReceived);

      //manage xpl hub
      xplcore::CXplHub hub(m_configuration.getXplNetworkInterface().address().toString());
      if (m_configuration.getStartXplhub())
      {
         startHub(hub);
      }

      // the main loop
      YADOMS_LOG(information) << "RfxLanXpl plugin is running..." ;
      while (1)
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents(boost::posix_time::minutes(10)))
         {
         case yApi::IYPluginApi::kEventStopRequested:
            {
               YADOMS_LOG(information) << "Stop requested" ;
               api->setPluginState(yApi::historization::EPluginState::kStopped);
               return;
            }
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command was received from Yadoms
               auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               OnSendDeviceCommand(command, api, xplService);
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(information) << "Update configuration..." ;

               //read new conf and
               CRfxLanXplConfiguration newConf;
               newConf.initializeWith(newConfiguration);

               if (m_configuration.getStartXplhub() != newConf.getStartXplhub())
               {
                  //the start hub changes
                  if (newConf.getStartXplhub())
                  {
                     //start a new hub
                     startHub(hub);
                  }
                  else
                  {
                     //stop hub
                     stopHub(hub);
                  }
               }

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_configuration.initializeWith(newConfiguration);
               api->setPluginState(yApi::historization::EPluginState::kRunning);
               break;
            }

         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> >();
               OnCreateDeviceRequest(data, api);
               break;
            }

         case yApi::IYPluginApi::kBindingQuery:
            {
               // Yadoms asks for device creation
               auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest> >();
               OnBindingQueryRequest(data, api);
               break;
            }

         case kXplMessageReceived:
            {
               // Xpl message was received
               auto xplMessage = api->getEventHandler().getEventData<xplcore::CXplMessage>();
               OnXplMessageReceived(xplMessage, api);
               break;
            }

         case kXplHubFound:
         case shared::event::kTimeout:
            {
               //when hub is found, or every 10 minutes, refresh rfxlan list
               StartPeripheralListing(xplService);
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id" ;
               break;
            }
         }
      }
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "The XPL plugin fails. Unknown exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Unknown exception : %1%") % ex.what()).str());
   }

   YADOMS_LOG(information) << "XPL plugin is now stopping" ;
   //ensure hub is stopped
   //   stopHub(hub);
}

void CRfxLanXpl::startHub(xplcore::CXplHub& hub)
{
   //new one !
   hub.start();
}

void CRfxLanXpl::stopHub(xplcore::CXplHub& hub)
{
   hub.stop();
}

void CRfxLanXpl::StartPeripheralListing(xplcore::CXplService& xplService)
{
   //send a hbeat.request to list all available rfxlan
   m_connectedRfxLans.clear();
   xplService.sendMessage(xplcore::CXplMessage::createHeartbeatRequest(xplService.getActor()));
}


//----------------------------------------------
///\brief Function handler when receiving XplMessage
///\param [in] The xpl message received
//----------------------------------------------
void CRfxLanXpl::OnXplMessageReceived(xplcore::CXplMessage& xplMessage,
                                      boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      YADOMS_LOG(information) << "Xpl Message received : " << xplMessage.toString() ;

      auto realSource = xplMessage.getSource().toString();
      //if incomming message has been sent from me, use target has real source
      if (xplMessage.getSource().getVendorId() == xplcore::CXplConstants::getYadomsVendorId() &&
         xplMessage.getSource().getDeviceId() == m_xpl_gateway_id)
         realSource = xplMessage.getTarget().toString();

      if (m_deviceManager->isHandled(realSource))
      {
         //list les passerelles xpl (rfxlan)
         if (std::find(m_connectedRfxLans.begin(), m_connectedRfxLans.end(), realSource) == m_connectedRfxLans.end())
            m_connectedRfxLans.push_back(realSource);

         boost::shared_ptr<xplrules::IRule> rule = m_deviceManager->identifyRule(xplMessage.getMessageSchemaIdentifier().toString(), m_instanceManager);
         if (rule.get() != nullptr)
         {
            //retreeive device identifier
            xplrules::CDeviceIdentifier deviceAddress = rule->getDeviceAddressFromMessage(xplMessage);

            if (!api->deviceExists(deviceAddress.getId()))
            {
               shared::CDataContainer details;
               details.set("readingProtocol", deviceAddress.getReadingXplProtocol().toString());
               details.set("writingProtocol", deviceAddress.getWritingXplProtocol().toString());
               details.set("innerDetails", deviceAddress.getInnerDetails());
               details.set("source", realSource);
               api->declareDevice(deviceAddress.getId(),
                                  deviceAddress.getCommercialName(),
                                  std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(),
                                  details);
            }

            //create message keywords in database
            auto allKeywords = rule->identifyKeywords(xplMessage);
            for (auto keyword = allKeywords.begin(); keyword != allKeywords.end(); ++keyword)
            {
               if (!api->keywordExists(deviceAddress.getId(), *keyword))
                  api->declareKeyword(deviceAddress.getId(), *keyword);
            }

            //check if the rule handle reading
            auto readRule = boost::dynamic_pointer_cast<xplrules::IReadRule>(rule);

            if (readRule)
            {
               //create message to insert in database
               auto data = readRule->extractMessageData(xplMessage);

               for (auto i = data.begin(); i != data.end(); ++i)
               {
                  api->historizeData(deviceAddress.getId(), *i);
               }
            }
         }
         else
         {
            auto errorMessage = (boost::format("Unsupported protocol = %1%") % xplMessage.getMessageSchemaIdentifier().toString()).str();
            YADOMS_LOG(error) << errorMessage ;
         }
      }
      else
      {
         auto errorMessage = (boost::format("Unknown xpl source = %1%") % realSource).str();
         YADOMS_LOG(error) << errorMessage ;
      }
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "xpl plugin fail to treat message : " << ex.what() ;
   }
}


//----------------------------------------------
///\brief Function handler used to send a command to a device
///\param [in] The command to send
//----------------------------------------------
void CRfxLanXpl::OnSendDeviceCommand(boost::shared_ptr<const yApi::IDeviceCommand> command,
                                     boost::shared_ptr<yApi::IYPluginApi> api,
                                     xplcore::CXplService& xplService)
{
   try
   {
      YADOMS_LOG(information) << "Sending message : " << yApi::IDeviceCommand::toString(command) ;
      if (api->deviceExists(command->getDevice()))
      {
         //get device details
         auto details = api->getDeviceDetails(command->getDevice());
         auto protocol = details.get<std::string>("writingProtocol");
         auto source = details.get<std::string>("source");

         auto innerDetails = shared::CDataContainer::EmptyContainer;
         if (details.exists("innerDetails"))
            innerDetails = details.get<shared::CDataContainer>("innerDetails");

         if (m_deviceManager->isHandled(source))
         {
            auto rule = m_deviceManager->identifyRule(protocol, m_instanceManager);

            if (rule)
            {
               //check if the rule handle reading
               auto commandRule = boost::dynamic_pointer_cast<xplrules::ICommandRule>(rule);

               if (commandRule)
               {
                  auto messageToSend = commandRule->createXplCommand(command, source, innerDetails);
                  if (messageToSend)
                  {
                     messageToSend->setSource(xplService.getActor());
                     xplService.sendMessage(*messageToSend.get());
                  }
                  else
                  {
                     //send result
                     YADOMS_LOG(error) << "Fail to create the Xpl message to send to the device" ;
                  }
               }
               else
               {
                  auto errorMessage = (boost::format("The protocol %1% do not support commands") % protocol).str();
                  YADOMS_LOG(error) << errorMessage ;
               }
            }
            else
            {
               auto errorMessage = (boost::format("Unsupported protocol = %1%") % protocol).str();
               YADOMS_LOG(error) << errorMessage ;
            }
         }
         else
         {
            auto errorMessage = (boost::format("Unknown xpl source = %1%") % source).str();
            YADOMS_LOG(error) << errorMessage ;
         }
      }
   }
   catch (std::exception& ex)
   {
      auto errorMessage = (boost::format("xpl plugin fail to send message : %1%") % ex.what()).str();
      YADOMS_LOG(error) << errorMessage ;
   }
}


void CRfxLanXpl::OnCreateDeviceRequest(boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data,
                                       boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      YADOMS_LOG(information) << "Create device request" ;

      const auto& deviceCfg = data->getData().getConfiguration();

      YADOMS_LOG(information) << deviceCfg.serialize() ;
      std::string internalProtocol;
      std::string deviceType = data->getData().getDeviceType();
      if (boost::istarts_with(deviceType, "x10"))
         internalProtocol = "x10.basic";
      if (boost::istarts_with(deviceType, "ac"))
         internalProtocol = "ac.basic";

      auto innerContent = data->getData().getConfiguration();


      auto rule = m_deviceManager->identifyRule(internalProtocol, m_instanceManager);
      if (rule)
      {
         //check if the rule handle reading
         auto deviceCreationRule = boost::dynamic_pointer_cast<const xplrules::ISupportManuallyDeviceCreationRule>(rule);

         if (deviceCreationRule)
         {
            //retreeive device identifier
            auto deviceContainer = deviceCreationRule->generateDeviceParameters(innerContent);
            const auto& deviceAddress = deviceContainer.getDeviceIdentifier();
            if (!api->deviceExists(deviceAddress.getId()))
            {
               shared::CDataContainer details;
               details.set("readingProtocol", deviceAddress.getReadingXplProtocol().toString());
               details.set("writingProtocol", deviceAddress.getWritingXplProtocol().toString());
               details.set("source", std::string("yadomssource!"));
               api->declareDevice(deviceAddress.getId(),
                                  deviceAddress.getCommercialName(),
                                  std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(),
                                  details);
            }

            //create message keywords in database
            for (auto keyword = deviceContainer.getKeywords().begin(); keyword != deviceContainer.getKeywords().end(); ++keyword)
            {
               if (!api->keywordExists(deviceAddress.getId(), *keyword))
                  api->declareKeyword(deviceAddress.getId(), *keyword);
            }

            //send created device
            data->sendSuccess(deviceAddress.getId());
         }
         else
         {
            auto errorMessage = (boost::format("The protocol %1% do not support device creation") % internalProtocol).str();
            data->sendError(errorMessage);
            YADOMS_LOG(error) << errorMessage ;
         }
      }
      else
      {
         auto errorMessage = (boost::format("Unsupported protocol = %1%") % deviceType).str();
         data->sendError(errorMessage);
         YADOMS_LOG(error) << errorMessage ;
      }
   }
   catch (std::exception& ex)
   {
      auto errorMessage = (boost::format("xpl plugin fail to create device : %1%") % ex.what()).str();
      data->sendError(errorMessage);
      YADOMS_LOG(error) << errorMessage ;
   }
}


void CRfxLanXpl::OnBindingQueryRequest(boost::shared_ptr<yApi::IBindingQueryRequest> data,
                                       boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      YADOMS_LOG(information) << "Custom query request" ;


      if (data->getData().getQuery() == "RfxLanList")
      {
         //send created device
         shared::CDataContainer result;
         for (auto i = m_connectedRfxLans.begin(); i != m_connectedRfxLans.end(); ++i)
            result.set(*i, *i, 0);

         data->sendSuccess(result);
      }
      else
      {
         auto errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
         data->sendError(errorMessage);
         YADOMS_LOG(error) << errorMessage ;
      }
   }
   catch (std::exception& ex)
   {
      auto errorMessage = (boost::format("xpl plugin fail to create device : %1%") % ex.what()).str();
      data->sendError(errorMessage);
      YADOMS_LOG(error) << errorMessage ;
   }
}

