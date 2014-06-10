#include "stdafx.h"
#include "Rfxcom.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/EmptyResult.hpp>
#include "RfxcomFactory.h"

IMPLEMENT_PLUGIN(CRfxcom)

//TODO dans package.json, remettre le type de Serial Port à "serialPort" quand supporté par l'ihm
CRfxcom::CRfxcom()
{
}

CRfxcom::~CRfxcom()
{
}

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
   kEvtPortDataReceived
};

void CRfxcom::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      // Load configuration values (provided by database)
      m_configuration.set(context->getConfiguration());

      // Create the port instance
      m_port = CRfxcomFactory::constructPort(m_configuration, context->getEventHandler(), kEvtPortConnection, kEvtPortDataReceived);

      // the main loop
      YADOMS_LOG(debug) << "CRfxcom is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               boost::shared_ptr<yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceCommand> >();

               onCommand(command);

               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = context->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               m_port.reset();

               // Update configuration
               m_configuration.set(newConfiguration);

               // Create new connection
               m_port = CRfxcomFactory::constructPort(m_configuration, context->getEventHandler(), kEvtPortConnection, kEvtPortDataReceived);

               break;
            }
         case kEvtPortConnection:
            {
               if (context->getEventHandler().getEventData<bool>())
                  processRfxcomConnectionEvent(context);
               else
                  processRfxcomUnConnectionEvent(context);

               break;
            }
         case kEvtPortDataReceived:
            {
               processRfxcomDataReceived(context, context->getEventHandler().getEventData<std::string>());
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
         }
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

void CRfxcom::onCommand(boost::shared_ptr<yApi::IDeviceCommand> command)
{
   YADOMS_LOG(debug) << "Command received :" << command->toString();

   if (m_transceiver)
      m_transceiver->send(command);
}

void CRfxcom::processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom is now connected";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom is now connected");

   BOOST_ASSERT_MSG(!m_transceiver, "RFXCom was already connected");
   m_transceiver.reset();

   m_transceiver = CRfxcomFactory::constructTransceiver(m_port);

   // Reset the RFXCom state
   m_transceiver->sendReset();
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom connection was lost";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom connection was lost");

   m_transceiver.reset();
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& data)
{
   YADOMS_LOG(debug) << "RFXCom >>> YADOMS : " << data;         //TODO trace en trop ?

   m_transceiver.reset();
}
