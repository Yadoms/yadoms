#include "stdafx.h"
#include "WebConnectionQuality.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/process/Process.h>
#include <shared/process/NativeExecutableCommandLine.h>
#include <shared/Executable.h>
#include "SpeedTestProcessObserver.h"
#include "SpeedTestProcessLogger.h"
#include "SpeedTestEventData.h"

IMPLEMENT_PLUGIN(CWebConnectionQuality)

static const auto DeviceName("Web connection quality");


CWebConnectionQuality::CWebConnectionQuality()
   : m_connectedKw(boost::make_shared<yApi::historization::CSwitch>("Connected", yApi::EKeywordAccessMode::kGet)),
     m_pingKw(boost::make_shared<yApi::historization::CDuration>("Ping")),
     m_uploadKw(boost::make_shared<specificHistorizers::CNetworkBandwithHistorizer>("Upload")),
     m_downloadKw(boost::make_shared<specificHistorizers::CNetworkBandwithHistorizer>("Download")),
     m_keywords({m_connectedKw, m_pingKw, m_uploadKw, m_downloadKw})
{
}

CWebConnectionQuality::~CWebConnectionQuality()
{
}

// Event IDs
enum
{
   kMeasureTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,
   kMeasureEndEventId
};

void CWebConnectionQuality::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CWebConnectionQuality is starting...";

   m_configuration.initializeWith(api->getConfiguration());

   declareDevice(api);

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // Start the first measure now and start timer for next measures
   startMeasure(api);
   const auto measureTimer = api->getEventHandler().createTimer(kMeasureTimerEventId,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::minutes(m_configuration.periodicityInMinutes()));


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
            const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
            YADOMS_LOG(information) << "Update configuration...";

            m_configuration.initializeWith(newConfiguration);
            m_configuration.trace();

            measureTimer->stop();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            // Start a measure now and restart timer for next measures
            startMeasure(api);
            measureTimer->start(boost::posix_time::minutes(m_configuration.periodicityInMinutes()));

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
            m_speedTestProcess.reset();
            const auto eventData = api->getEventHandler().getEventData<boost::shared_ptr<CSpeedTestEventData>>();
            if (!eventData->success())
            {
               YADOMS_LOG(warning) << "speedtest returns " << eventData->returnCode() << ", " << eventData->error();
               m_connectedKw->set(false);
               api->historizeData(DeviceName, m_connectedKw);
               break;
            }

            processResult(api,
                          eventData->result());
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

void CWebConnectionQuality::startMeasure(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Start measure...";

   std::vector<std::string> args;
   args.push_back((api->getInformation()->getPath() / "speedtest.py").string());
   args.push_back("--json");

   const auto commandLine = boost::make_shared<shared::process::CNativeExecutableCommandLine>(
      shared::CExecutable::ToFileName("python"),
      ".",
      args);

   const auto processLogger = boost::make_shared<CSpeedTestProcessLogger>("[speedtest process] ");

   const auto processObserver = boost::make_shared<CSpeedTestProcessObserver>(api->getEventHandler(),
                                                                              kMeasureEndEventId,
                                                                              processLogger);

   m_speedTestProcess = boost::make_shared<shared::process::CProcess>(commandLine,
                                                                      processObserver,
                                                                      processLogger);
}

void CWebConnectionQuality::processResult(boost::shared_ptr<yApi::IYPluginApi> api,
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
      m_downloadKw->set(static_cast<unsigned int>(resultContainer.get<double>("download")));
      m_uploadKw->set(static_cast<unsigned int>(resultContainer.get<double>("upload")));

      YADOMS_LOG(information) << "  - ping : " << m_pingKw->get();
      YADOMS_LOG(information) << "  - download : " << m_downloadKw->get();
      YADOMS_LOG(information) << "  - upload : " << m_uploadKw->get();

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
