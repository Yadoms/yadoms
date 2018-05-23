#include "stdafx.h"
#include "WebConnectionQuality.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>

IMPLEMENT_PLUGIN(CWebConnectionQuality)

static const auto DeviceName("Web connection quality");


CWebConnectionQuality::CWebConnectionQuality()
   : m_pingKw(boost::make_shared<const yApi::historization::CDuration>("Ping")),
     m_uploadKw(boost::make_shared<const specificHistorizer::CNetworkBandwith>("Upload")),
     m_downloadKw(boost::make_shared<const specificHistorizer::CNetworkBandwith>("Download")),
     m_keywords({m_pingKw, m_uploadKw, m_downloadKw})
{
}

CWebConnectionQuality::~CWebConnectionQuality()
{
}

// Event IDs
enum
{
   kMeasureTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,
};

void CWebConnectionQuality::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CWebConnectionQuality is starting...";

   m_configuration.initializeWith(api->getConfiguration());

   declareDevice(api);

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // Do the first measure now and start timer for next measures
   doMeasure(api);
   const auto measureTimer = api->getEventHandler().createTimer(kMeasureTimerEventId,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::minutes(m_configuration.periodicityInMinutes()));


   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration...";

            m_configuration.initializeWith(newConfiguration);
            m_configuration.trace();

            measureTimer->stop();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            // Do a measure now and restart timer for next measures
            doMeasure(api);
            measureTimer->start(boost::posix_time::minutes(m_configuration.periodicityInMinutes()));

            break;
         }

      case kMeasureTimerEventId:
         {
            doMeasure(api);
            break;
         }

      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CWebConnectionQuality::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (api->deviceExists(DeviceName))
      return;

   api->declareDevice(DeviceName,
                      DeviceName,
                      DeviceName,
                      m_keywords);
}

void CWebConnectionQuality::doMeasure(boost::shared_ptr<yApi::IYPluginApi> api)
{
}
