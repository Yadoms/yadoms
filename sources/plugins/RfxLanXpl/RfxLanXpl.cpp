#include "stdafx.h"
#include "RfxLanXpl.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include "xplcore/XplService.h"
#include "xplcore/XplHub.h"
#include "xplcore/XplConstants.h"
#include "xplrules/IReadRule.h"
#include "xplrules/ICommandRule.h"
#include "xplrules/rfxLanXpl/DeviceManager.h"
#include "xplrules/ISupportManuallyDeviceCreationRule.h"
#include "xplrules/DeviceContainer.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CRfxLanXpl)

std::string CRfxLanXpl::m_xpl_gateway_id = "xplplug";

CRfxLanXpl::CRfxLanXpl()
   :m_deviceManager(new xplrules::rfxLanXpl::CDeviceManager())
{
}

CRfxLanXpl::~CRfxLanXpl()
{
}

// Event IDs
enum
{
   kXplMessageReceived = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kXplHubFound,
};

void print(shared::CDataContainer const& pt)
{
   YADOMS_LOG(debug) << pt.serialize();
}

void CRfxLanXpl::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   
   try
   {
      YADOMS_LOG(debug) << "################ RfxLanXpl : PRINT CONFIG";

      print(context->getConfiguration());

      // Load configuration values (provided by database)
      YADOMS_LOG(debug) << "################ RfxLanXpl : SET CONFIG";
      m_configuration.initializeWith(context->getConfiguration());

      //start ioservice
      YADOMS_LOG(debug) << "################ RfxLanXpl : NETWORK IFACE";
      Poco::Net::NetworkInterface interface = m_configuration.getXplNetworkInterface();

      YADOMS_LOG(debug) << "RfxLanXpl : create xpl service";
      xplcore::CXplService xplService(interface, m_xpl_gateway_id, "1", &context->getEventHandler(), kXplHubFound);
      YADOMS_LOG(debug) << "RfxLanXpl : subscribe for xpl messages";
      xplService.subscribeForAllMessages(&context->getEventHandler(), kXplMessageReceived);

      //manage xpl hub
      xplcore::CXplHub hub(m_configuration.getXplNetworkInterface().address().toString());
      if (m_configuration.getStartXplhub())
      {
         startHub(hub, "");
      }

      // the main loop
      YADOMS_LOG(debug) << "RfxLanXpl plugin is running...";
      while (1)
      {
         // Wait for an event
         switch (context->getEventHandler().waitForEvents(boost::posix_time::minutes(10)))
         {
         case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command was received from Yadoms
            boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
            OnSendDeviceCommand(command, context, xplService);
            break;
         }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(debug) << "configuration was updated...";

            //read new conf and
            CRfxLanXplConfiguration newConf;
            newConf.initializeWith(newConfiguration);

            if (m_configuration.getStartXplhub() != newConf.getStartXplhub())
            {
               //the start hub changes
               if (newConf.getStartXplhub())
               {
                  //start a new hub
                  startHub(hub, newConf.getXplNetworkInterface().address().toString());
               }
               else
               {
                  //stop hub
                  stopHub(hub);
               }
            }
            else
            {
               //if (!boost::iequals(m_configuration.getHubLocalIp(), newConf.getHubLocalIp()))
               //{
               //   //the filter chages, just update hub
               //   if (m_hub)
               //      m_hub->updateHubFilter(newConf.getHubLocalIp());
               //}
            }

            // Take into account the new configuration
            // - Restart the plugin if necessary,
            // - Update some resources,
            // - etc...
            m_configuration.initializeWith(newConfiguration);
            break;
         }

         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
         {
            // Yadoms asks for device creation
            boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data = context->getEventHandler().getEventData< boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> >();
            OnCreateDeviceRequest(data, context);
            break;
         }

			case yApi::IYPluginApi::kBindingQuery:
         {
            // Yadoms asks for device creation
            boost::shared_ptr<yApi::IBindingQueryRequest> data = context->getEventHandler().getEventData< boost::shared_ptr<yApi::IBindingQueryRequest> >();
            OnBindingQueryRequest(data, context);
            break;
         }

         case kXplMessageReceived:
         {
            // Xpl message was received
            xplcore::CXplMessage xplMessage = context->getEventHandler().getEventData<xplcore::CXplMessage>();
            OnXplMessageReceived(xplMessage, context);
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
            YADOMS_LOG(error) << "Unknown message id";
            break;
         }
         }
      };
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
   }
   catch (shared::exception::CException & ex)
   {
      YADOMS_LOG(fatal) << "The XPL plugin fails. Unknown expcetion : " << ex.what();
   }

   YADOMS_LOG(information) << "XPL plugin is now stopping";
   //ensure hub is stopped
//   stopHub(hub);
}

void CRfxLanXpl::startHub(xplcore::CXplHub & hub, const std::string & hubFilterConfiguration)
{
   //new one !
   //hub.reset(new xplcore::CXplHub(hubFilterConfiguration));
   hub.start();
}

void CRfxLanXpl::stopHub(xplcore::CXplHub & hub)
{
   hub.stop();
}

void CRfxLanXpl::StartPeripheralListing(xplcore::CXplService & xplService)
{
   //send a hbeat.request to list all available rfxlan
   m_connectedRfxLans.clear();
   xplService.sendMessage(xplcore::CXplMessage::createHeartbeatRequest(xplService.getActor()));
}


//----------------------------------------------
///\brief Function handler when receiving XplMessage
///\param [in] The xpl message received
//----------------------------------------------
void CRfxLanXpl::OnXplMessageReceived(xplcore::CXplMessage & xplMessage, boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(trace) << "Xpl Message received : " << xplMessage.toString();

      //YADOMS_LOG(information) << "Reveived" << std::endl << xplMessage.toString();

      std::string realSource = xplMessage.getSource().toString();
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
         if (rule.get() != NULL)
         {
            //retreeive device identifier
            xplrules::CDeviceIdentifier deviceAddress = rule->getDeviceAddressFromMessage(xplMessage);

            if (!context->deviceExists(deviceAddress.getId()))
            {
               shared::CDataContainer details;
               details.set("readingProtocol", deviceAddress.getReadingXplProtocol().toString());
               details.set("writingProtocol", deviceAddress.getWritingXplProtocol().toString());
               details.set("innerDetails", deviceAddress.getInnerDetails());
               details.set("source", realSource);
               context->declareDevice(deviceAddress.getId(), deviceAddress.getCommercialName(), details.serialize());
            }

            //create message keywords in database
            std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > allKeywords = rule->identifyKeywords(xplMessage);
            for (std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> >::iterator keyword = allKeywords.begin(); keyword != allKeywords.end(); ++keyword)
            {
               if (!context->keywordExists(deviceAddress.getId(), keyword->get()->getKeyword()))
                  context->declareKeyword(deviceAddress.getId(), *(keyword->get()));
            }

            //check if the rule handle reading
            boost::shared_ptr<xplrules::IReadRule> readRule = boost::dynamic_pointer_cast<xplrules::IReadRule>(rule);

            if (readRule)
            {

               //create message to insert in database
               xplrules::MessageContent data = readRule->extractMessageData(xplMessage);

               xplrules::MessageContent::iterator i;
               for (i = data.begin(); i != data.end(); ++i)
               {
                  context->historizeData(deviceAddress.getId(), *(i->get()));
               }
            }


         }
         else
         {
            std::string errorMessage = (boost::format("Unsupported protocol = %1%") % xplMessage.getMessageSchemaIdentifier().toString()).str();
            YADOMS_LOG(error) << errorMessage;
         }
      }
      else
      {
         std::string errorMessage = (boost::format("Unknown xpl source = %1%") % realSource).str();
         YADOMS_LOG(error) << errorMessage;
      }

   }
   catch (std::exception &ex)
   {
      YADOMS_LOG(error) << "xpl plugin fail to treat message : " << ex.what();
   }
}



//----------------------------------------------
///\brief Function handler used to send a command to a device
///\param [in] The command to send
//----------------------------------------------
void CRfxLanXpl::OnSendDeviceCommand(boost::shared_ptr<const yApi::IDeviceCommand> command, boost::shared_ptr<yApi::IYPluginApi> context, xplcore::CXplService & xplService)
{
   try
   {
      YADOMS_LOG(trace) << "Sending message : " << command->toString();
      if (context->deviceExists(command->getTargetDevice()))
      {
         //get device details
         shared::CDataContainer details = context->getDeviceDetails(command->getTargetDevice());
         std::string protocol = details.get<std::string>("writingProtocol");
         std::string source = details.get<std::string>("source");

         shared::CDataContainer innerDetails = shared::CDataContainer::EmptyContainer;
         if(details.hasValue("innerDetails"))
            innerDetails = details.get<shared::CDataContainer>("innerDetails");

         if (m_deviceManager->isHandled(source))
         {
            boost::shared_ptr<xplrules::IRule> rule = m_deviceManager->identifyRule(protocol, m_instanceManager);

            if (rule)
            {
               //check if the rule handle reading
               boost::shared_ptr<xplrules::ICommandRule> commandRule = boost::dynamic_pointer_cast<xplrules::ICommandRule>(rule);

               if (commandRule)
               {
                  boost::shared_ptr< xplcore::CXplMessage > messageToSend = commandRule->createXplCommand(command, source, innerDetails);
                  if (messageToSend)
                  {
                     messageToSend->setSource(xplService.getActor());

                     //YADOMS_LOG(information) << "Sent" << std::endl << messageToSend->toString();
                     xplService.sendMessage(*messageToSend.get());
                  }
                  else
                  {
                     //send result
                     std::string errorMessage = "Fail to create the Xpl message to send to the device";
                     YADOMS_LOG(error) << errorMessage;
                  }

               }
               else
               {
                  std::string errorMessage = (boost::format("The protocol %1% do not support commands") % protocol).str();
                  YADOMS_LOG(error) << errorMessage;
               }
            }
            else
            {
               std::string errorMessage = (boost::format("Unsupported protocol = %1%") % protocol).str();
               YADOMS_LOG(error) << errorMessage;
            }
         }
         else
         {
            std::string errorMessage = (boost::format("Unknown xpl source = %1%") % source).str();
            YADOMS_LOG(error) << errorMessage;
         }

      }

   }
   catch (std::exception &ex)
   {
      std::string errorMessage = (boost::format("xpl plugin fail to send message : %1%") % ex.what()).str();
      YADOMS_LOG(error) << errorMessage;
   }
}


void CRfxLanXpl::OnCreateDeviceRequest(boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data, boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(information) << "Create device request";

      const shared::CDataContainer & deviceCfg = data->getData().getConfiguration();

      YADOMS_LOG(information) << deviceCfg.serialize();

      //std::string a("abcdefg");
      //data->sendError("invalid configuration");


      std::string chosenDeviceType = data->getData().getConfiguration().get<std::string>("type.activeSection");
      std::string internalProtocol;
      if (boost::istarts_with(chosenDeviceType, "x10"))
         internalProtocol = "x10.basic";
      if (boost::istarts_with(chosenDeviceType, "ac"))
         internalProtocol = "ac.basic";

      shared::CDataContainer innerContent = data->getData().getConfiguration().get<shared::CDataContainer>("type.content." + chosenDeviceType + ".content");


      boost::shared_ptr<xplrules::IRule> rule = m_deviceManager->identifyRule(internalProtocol, m_instanceManager);
      if (rule)
      {
         //check if the rule handle reading
         boost::shared_ptr<const xplrules::ISupportManuallyDeviceCreationRule> deviceCreationRule = boost::dynamic_pointer_cast<const xplrules::ISupportManuallyDeviceCreationRule>(rule);

         if (deviceCreationRule)
         {
            //retreeive device identifier
            xplrules::CDeviceContainer deviceContainer = deviceCreationRule->generateDeviceParameters(innerContent);
            const xplrules::CDeviceIdentifier & deviceAddress = deviceContainer.getDeviceIdentifier();
            if (!context->deviceExists(deviceAddress.getId()))
            {
               shared::CDataContainer details;
               details.set("readingProtocol", deviceAddress.getReadingXplProtocol().toString());
               details.set("writingProtocol", deviceAddress.getWritingXplProtocol().toString());
               details.set("source", std::string("yadomssource!"));
               context->declareDevice(deviceAddress.getId(), deviceAddress.getCommercialName(), details.serialize());
            }

            //create message keywords in database
            for (std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> >::const_iterator keyword = deviceContainer.getKeywords().begin(); keyword != deviceContainer.getKeywords().end(); ++keyword)
            {
               if (!context->keywordExists(deviceAddress.getId(), keyword->get()->getKeyword()))
                  context->declareKeyword(deviceAddress.getId(), *(keyword->get()));
            }

            //send created device
            data->sendSuccess(deviceAddress.getId());
         }
         else
         {
            std::string errorMessage = (boost::format("The protocol %1% do not support device creation") % internalProtocol).str();
            data->sendError(errorMessage);
            YADOMS_LOG(error) << errorMessage;
         }
      }
      else
      {
         std::string errorMessage = (boost::format("Unsupported protocol = %1%") % chosenDeviceType).str();
         data->sendError(errorMessage);
         YADOMS_LOG(error) << errorMessage;
      }



   }
   catch (std::exception &ex)
   {
      std::string errorMessage = (boost::format("xpl plugin fail to create device : %1%") % ex.what()).str();
      data->sendError(errorMessage);
      YADOMS_LOG(error) << errorMessage;
   }
}


void CRfxLanXpl::OnBindingQueryRequest(boost::shared_ptr<yApi::IBindingQueryRequest> data, boost::shared_ptr<yApi::IYPluginApi> context)
{
	try
	{
      YADOMS_LOG(information) << "Custom query request";


		if (data->getData().getQuery() == "RfxLanList")
		{
			//send created device
         shared::CDataContainer result;
         for (std::vector<std::string>::iterator i = m_connectedRfxLans.begin(); i != m_connectedRfxLans.end(); ++i)
            result.set(*i, *i, 0);

         data->sendSuccess(result);
		}
		else
		{
			std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
			data->sendError(errorMessage);
			YADOMS_LOG(error) << errorMessage;
		}
	}
	catch (std::exception &ex)
	{
		std::string errorMessage = (boost::format("xpl plugin fail to create device : %1%") % ex.what()).str();
		data->sendError(errorMessage);
		YADOMS_LOG(error) << errorMessage;
	}
}
