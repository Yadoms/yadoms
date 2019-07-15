#include "stdafx.h"
#include "Weather.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>

IMPLEMENT_PLUGIN(CWeather)

static const auto DeviceName("Web connection quality");//TODO virer
const boost::posix_time::time_duration CWeather::RequestPeriodicity(boost::posix_time::hours(2));


CWeather::CWeather()
   : m_connectedKw(boost::make_shared<yApi::historization::CSwitch>("Connected", yApi::EKeywordAccessMode::kGet)),
     m_pingKw(boost::make_shared<yApi::historization::CDuration>("Ping")),
     m_keywords({m_connectedKw, m_pingKw})
{
}

// Event IDs
enum
{
   kMeasureTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,
   kMeasureEndEventId
};

void CWeather::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CWeather is starting...";

   m_configuration.initializeWith(api->getConfiguration());

   declareDevice(api);

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // Start the first measure now and start timer for next measures
   startMeasure(api);
   const auto measureTimer = api->getEventHandler().createTimer(kMeasureTimerEventId,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                RequestPeriodicity);


   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            measureTimer->stop();
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

            // Start a measure now and restart timer for next measures
            startMeasure(api);
            measureTimer->start(RequestPeriodicity);

            break;
         }

      case kMeasureTimerEventId:
         {
            startMeasure(api);
            break;
         }

      case kMeasureEndEventId:
         {
            YADOMS_LOG(information) << "End of measure";
            //m_speedTestProcess.reset();
            //const auto eventData = api->getEventHandler().getEventData<boost::shared_ptr<CSpeedTestEventData>>();
            //if (!eventData->success())
            //{
            //   YADOMS_LOG(warning) << "speedtest returns " << eventData->returnCode() << ", " << eventData->error();
            //   m_connectedKw->set(false);
            //   api->historizeData(DeviceName, m_connectedKw);
            //   break;
            //}

            //processResult(api,
            //              eventData->result());
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

void CWeather::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (api->deviceExists(DeviceName))
      return;

   api->declareDevice(DeviceName,
                      DeviceName,
                      DeviceName,
                      m_keywords);
}

void CWeather::startMeasure(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Start measure...";

   //TODO
}

void CWeather::processResult(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const std::string& result) const
{
   YADOMS_LOG(information) << "Process result...";

   try
   {
      shared::CDataContainer resultContainer(result);

      YADOMS_LOG(information) << "Result file gives :";
      resultContainer.printToLog(YADOMS_LOG(debug));
      
      m_connectedKw->set(true);
      m_pingKw->set(static_cast<unsigned int>(resultContainer.get<double>("ping") / 1000.0));

      YADOMS_LOG(information) << "  - ping : " << m_pingKw->get();

      api->historizeData(DeviceName,
                         m_keywords);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error processing speedtest result : " << e.what();
      m_connectedKw->set(false);
      api->historizeData(DeviceName, m_connectedKw);
   }
}
