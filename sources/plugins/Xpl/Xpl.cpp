#include "stdafx.h"
#include "Xpl.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include "xplcore/XplService.h"
#include "xplcore/XplHub.h"
#include "xplcore/XplConstants.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CXpl)

std::string CXpl::m_xpl_gateway_id = "xplplug";

CXpl::CXpl()
{
}

CXpl::~CXpl()
{
}

// Event IDs
enum
{
   kXplMessageReceived = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
};

void CXpl::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   boost::shared_ptr<xplcore::CXplHub> hub;
   try
   {
      YADOMS_LOG_CONFIGURE(context->getInformation().getName());
      YADOMS_LOG(debug) << "CXpl plugin is starting...";

      // Load configuration values (provided by database)
      m_configuration.set(context->getConfiguration());

      //start ioservice
      xplcore::CXplService xplService(m_xpl_gateway_id, "1", NULL);
      xplService.subscribeForAllMessages(&context->getEventHandler(), kXplMessageReceived);

      //manage xpl hub
      if(m_configuration.getStartXplhub())
      {
         hub.reset(new xplcore::CXplHub(m_configuration.getHubLocalIp()));
         hub->start();
      }

      // the main loop
      YADOMS_LOG(debug) << "CXpl is running...";
      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Command was received from Yadoms
               yApi::IYadomsApi::CDeviceCommand command = context->getEventHandler().getEventData<yApi::IYadomsApi::CDeviceCommand>();
               YADOMS_LOG(debug) << "Command received from Yadoms :" << command.toString();
               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = context->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_configuration.set(newConfiguration);
               break;
            }
        
         case kXplMessageReceived:
            {
               // Xpl message was received
               xplcore::CXplMessage xplMessage = context->getEventHandler().getEventData<xplcore::CXplMessage>();
               OnXplMessageReceived(xplMessage);
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
      YADOMS_LOG(info) << "CXpl plugin is stopping..."  << std::endl;
   }

   if(hub)
      hub->stop();
}




   //----------------------------------------------
   ///\brief Function handler when receiving XplMessage
   ///\param [in] The xpl message received
   //----------------------------------------------
   void CXpl::OnXplMessageReceived(xplcore::CXplMessage & xplMessage)
   {
      try
      {
         YADOMS_LOG(trace) << "Xpl Message received : " << xplMessage.toString();

         std::string realSource = xplMessage.getSource().toString();
         //if incomming message has been sent from me, use target has real source
         if(xplMessage.getSource().getVendorId() == xplcore::CXplConstants::getYadomsVendorId() &&
        		 xplMessage.getSource().getDeviceId() == m_xpl_gateway_id)
            realSource = xplMessage.getTarget().toString();
      
         boost::shared_ptr<xplrules::IRule> rule = m_rulerFactory.identifyRule(realSource, xplMessage.getMessageSchemaIdentifier().toString());
         if(rule.get() != NULL)
         {
            //retreeive device identifier
            xplrules::CDeviceIdentifier deviceAddress = rule->getDeviceAddressFromMessage(xplMessage);

            //try to find device in db
            /*
            boost::shared_ptr<database::entities::CDevice> device = m_dataProvider->getDeviceRequester()->getDevice(deviceAddress.getId(), xplMessage.getMessageSchemaIdentifier().toString(), realSource);
            if(!device)
            {
               //create the device in database
               //TODO : make rule generate a pseudo real name
               device = m_dataProvider->getDeviceRequester()->createDevice(deviceAddress.getId(), xplMessage.getMessageSchemaIdentifier().toString(), realSource, deviceAddress.getCommercialName());
            }

            //create message keywords in database
            std::vector< boost::shared_ptr<database::entities::CKeyword> > allKeywords = rule->identifyKeywords(xplMessage);
            BOOST_FOREACH(boost::shared_ptr<database::entities::CKeyword> keyword, allKeywords)
            {
               keyword->DeviceId = device->Id();
               m_dataProvider->getKeywordRequester()->addKeyword(keyword);
            }

            //create message to insert in database
            database::entities::CMessage msgToInsert;
            msgToInsert.Date = boost::posix_time::second_clock::universal_time();
            msgToInsert.DeviceId = device->Id();

            rules::MessageContent data = rule->extractMessageData(xplMessage);
            std::vector<database::entities::CMessageContent> msgContentEntries;

            rules::MessageContent::iterator i;
            for(i = data.begin(); i!= data.end(); ++i)
            {
               database::entities::CMessageContent item;
               item.Key = i->first;
               item.Value = i->second;
               msgContentEntries.push_back(item);
            }

            m_dataProvider->getMessageRequester()->insertMessage(msgToInsert, msgContentEntries);
            */
         }
      }
      catch(std::exception &ex)
      {
         YADOMS_LOG(error) << "CXplGateway fail to treat message : " << ex.what();
      }
   }