#include "stdafx.h"
#include "Rfxcom.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "RfxcomFactory.h"
#include "ProtocolException.hpp"
#include "PortException.hpp"

IMPLEMENT_PLUGIN(CRfxcom)


//TODO dans package.json, compléter la liste manuallyDeviceCreationConfigurationSchema/type (voir Domoticz "Switch_Type_Desc")
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
   kEvtPortDataReceived,
   kprotocolErrorRetryTimer
};

void CRfxcom::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // the main loop
      YADOMS_LOG(debug) << "CRfxcom is running...";

      // Create the connection
      m_transceiver = CRfxcomFactory::constructTransceiver(context, m_configuration, kEvtPortConnection, kEvtPortDataReceived);

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               boost::shared_ptr<yApi::IDeviceCommand> command(context->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceCommand> >());
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               onCommand(command->getBody(), context->getDeviceDetails(command->getTargetDevice()));

               break;
            }
         case yApi::IYadomsApi::kEventManuallyDeviceCreationTest:
            {
               // Yadoms asks for test device parameters to check if it work before creating it. So just send command, don't declare anything.
               boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> data = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> >();
               YADOMS_LOG(debug) << "Test of device request received :" << data->toString();

               onCommand(data->getCommand()->getBody(), data->getDeviceParameters());

               break;
            }
         case yApi::IYadomsApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               boost::shared_ptr<yApi::IManuallyDeviceCreationData> data = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationData> >();
               YADOMS_LOG(debug) << "Manually device creation request received :" << data->toString();

               // Declare the device
               context->declareDevice(data->getDevice(), shared::CStringExtension::EmptyString, data->getParameters());
               // Declare associated keywords (= values managed by this device)
               context->declareKeyword(data->getDevice(), data->getKeyword(), yApi::CStandardCapacities::Switch/* TODO en attendant de récupérer de data->getCapacity()*/);

               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               m_transceiver.reset();

               // Update configuration
               m_configuration.initializeWith(newConfiguration);

               // Create new connection
               m_transceiver = CRfxcomFactory::constructTransceiver(context, m_configuration, kEvtPortConnection, kEvtPortDataReceived);

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
               processRfxcomDataReceived(context, context->getEventHandler().getEventData<CByteBuffer>());
               break;
            }
         case kprotocolErrorRetryTimer:
            {
               m_transceiver = CRfxcomFactory::constructTransceiver(context, m_configuration, kEvtPortConnection, kEvtPortDataReceived);
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

void CRfxcom::onCommand(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   if (m_transceiver)
      m_transceiver->send(command, deviceParameters);
}

void CRfxcom::processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom is now connected";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom is now connected");

   try
   {
      // Reset the RFXCom
      m_transceiver->processReset();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Error resetting RFXCom transceiver : " << e.what();

      // Stop the communication, and try later
      m_transceiver.reset();
      context->getEventHandler().createTimer(kprotocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
   }
   catch (CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to RFXCom transceiver : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom connection was lost";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom connection was lost");

   m_transceiver.reset();
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const CByteBuffer& data)
{
   m_transceiver->historize(data);
}
