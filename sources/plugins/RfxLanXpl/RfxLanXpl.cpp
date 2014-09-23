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
               details.set("source", realSource);
               context->declareDevice(deviceAddress.getId(), deviceAddress.getCommercialName(), details.serialize());
            }

            //create message keywords in database
            std::vector< boost::shared_ptr<shared::plugin::yadomsApi::commands::IHistorizable> > allKeywords = rule->identifyKeywords(xplMessage);
            for (std::vector< boost::shared_ptr<shared::plugin::yadomsApi::commands::IHistorizable> >::iterator keyword = allKeywords.begin(); keyword != allKeywords.end(); ++keyword)
            {
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

         if (m_deviceManager->isHandled(source))
         {
            boost::shared_ptr<xplrules::IRule> rule = m_deviceManager->identifyRule(protocol, m_instanceManager);

            if (rule)
            {
               //check if the rule handle reading
               boost::shared_ptr<xplrules::ICommandRule> commandRule = boost::dynamic_pointer_cast<xplrules::ICommandRule>(rule);

               if (commandRule)
               {
                  boost::shared_ptr< xplcore::CXplMessage > messageToSend = commandRule->createXplCommand(command, source);
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