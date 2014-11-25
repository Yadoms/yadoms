#include "stdafx.h"
#include "RfxLanXpl.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
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
   kXplMessageReceived = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
};

void print(shared::CDataContainer const& pt)
{
   YADOMS_LOG(debug) << pt.serialize();
}

void CRfxLanXpl::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   boost::shared_ptr<xplcore::CXplHub> hub;
   try
   {

      print(context->getConfiguration());

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      //start ioservice
      xplcore::CXplService xplService(m_xpl_gateway_id, "1", NULL);
      xplService.subscribeForAllMessages(&context->getEventHandler(), kXplMessageReceived);

      //manage xpl hub
      if (m_configuration.getStartXplhub())
      {
         hub.reset(new xplcore::CXplHub(m_configuration.getHubLocalIp()));
         hub->start();

         
      }

      // the main loop
      YADOMS_LOG(debug) << "RfxLanXpl plugin is running...";
      while (1)
      {
         // Wait for an event
         switch (context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
         {
            // Command was received from Yadoms
            boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
            OnSendDeviceCommand(command, context, xplService);
            break;
         }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(debug) << "configuration was updated...";

            // Take into account the new configuration
            // - Restart the plugin if necessary,
            // - Update some resources,
            // - etc...
            m_configuration.initializeWith(newConfiguration);
            break;
         }

         case yApi::IYadomsApi::kEventManuallyDeviceCreation:
         {
            // Yadoms asks for device creation
            boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data = context->getEventHandler().getEventData< boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> >();
            OnCreateDeviceRequest(data, context);
            break;
         }         
          
			case yApi::IYadomsApi::kBindingQuery:
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

   if (hub)
      hub->stop();
}




//----------------------------------------------
///\brief Function handler when receiving XplMessage
///\param [in] The xpl message received
//----------------------------------------------
void CRfxLanXpl::OnXplMessageReceived(xplcore::CXplMessage & xplMessage, boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG(trace) << "Xpl Message received : " << xplMessage.toString();

      std::string realSource = xplMessage.getSource().toString();
      //if incomming message has been sent from me, use target has real source
      if (xplMessage.getSource().getVendorId() == xplcore::CXplConstants::getYadomsVendorId() &&
         xplMessage.getSource().getDeviceId() == m_xpl_gateway_id)
         realSource = xplMessage.getTarget().toString();

      if (m_deviceManager->isHandled(realSource))
      {

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
            std::vector< boost::shared_ptr<shared::plugin::yadomsApi::historization::IHistorizable> > allKeywords = rule->identifyKeywords(xplMessage);
            for (std::vector< boost::shared_ptr<shared::plugin::yadomsApi::historization::IHistorizable> >::iterator keyword = allKeywords.begin(); keyword != allKeywords.end(); ++keyword)
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
void CRfxLanXpl::OnSendDeviceCommand(boost::shared_ptr<const yApi::IDeviceCommand> command, boost::shared_ptr<yApi::IYadomsApi> context, xplcore::CXplService & xplService)
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


void CRfxLanXpl::OnCreateDeviceRequest(boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data, boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG(info) << "Create device request";

      const shared::CDataContainer & deviceCfg = data->getData().getConfiguration();

      YADOMS_LOG(info) << deviceCfg.serialize();

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
            for (std::vector< boost::shared_ptr<shared::plugin::yadomsApi::historization::IHistorizable> >::const_iterator keyword = deviceContainer.getKeywords().begin(); keyword != deviceContainer.getKeywords().end(); ++keyword)
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


void CRfxLanXpl::OnBindingQueryRequest(boost::shared_ptr<yApi::IBindingQueryRequest> data, boost::shared_ptr<yApi::IYadomsApi> context)
{
	try
	{
		YADOMS_LOG(info) << "Custom query request";


		if (data->getData().getQuery() == "RfxLanList")
		{
			//send created device
			std::vector<std::string> allRfxLans;

         shared::CDataContainer result;
         result.set("rfxlan-test1", "rfxlan-test1");
         result.set("rfxlan-test2", "rfxlan-test2");
         result.set("rfxlan-test3", "rfxlan-test3");

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