#include "stdafx.h"
#include "RaspberryPI.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include "RaspberryPITemperatureSensor.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CRaspberryPI)

// Event IDs
enum
{
   kSendTemperatureTimerEventId = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
};

CRaspberryPI::CRaspberryPI()
{
}

CRaspberryPI::~CRaspberryPI()
{
}


void CRaspberryPI::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CRaspberryPI is starting...";

      // Fake temperature sensor
      CRaspberryPITemperatureSensor temperatureSensor("temperatureSensor");
      temperatureSensor.declareDevice(context);
      
      // Timer used to send fake sensor states periodically
      context->getEventHandler().createTimer(kSendTemperatureTimerEventId, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));

      // the main loop
      YADOMS_LOG(debug) << "RaspberryPI plugin is running...";
      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kSendTemperatureTimerEventId:
            {
               // First read the sensor value
               if(temperatureSensor.read())
               {
                  temperatureSensor.historizeData(context);
               }
               break;
            }
            
         case yApi::IYadomsApi::kEventDeviceCommand:
         case yApi::IYadomsApi::kEventUpdateConfiguration:            
            {
               YADOMS_LOG(warning) << "Message not managed";
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
      YADOMS_LOG(info) << "CRaspberryPI is stopping..."  << std::endl;
   }
}


