#include "stdafx.h"
#include "HighPrecisionLiBatteryMonitor.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration CHighPrecisionLiBatteryMonitor::HistorizationPeriod(boost::posix_time::seconds(5));

CHighPrecisionLiBatteryMonitor::CHighPrecisionLiBatteryMonitor(EOneWireFamily family,
                                                               const std::string &id,
                                                               boost::shared_ptr<ioInterfaces::IHighPrecisionLiBatteryMonitor> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2760")),
      m_io(io),
      m_kwIo(boost::make_shared<yApi::historization::CSwitch>("io")),
      m_kwTemperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
      m_kwVis(boost::make_shared<yApi::historization::CVoltage>("vis")),
      m_kwVolt(boost::make_shared<yApi::historization::CVoltage>("volt")),
      m_keywords({m_kwIo, m_kwTemperature, m_kwVis, m_kwVolt})
{
   BOOST_ASSERT_MSG(m_identification->family() == kHighPrecisionLiBatteryMonitor, "Invalid family number");
}

void CHighPrecisionLiBatteryMonitor::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                      const boost::shared_ptr<shared::CDataContainer> &configuration)
{
   YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
}

void CHighPrecisionLiBatteryMonitor::read() const
{
   m_kwIo->set(m_io->readIo());
   m_kwTemperature->set(m_io->readTemperature());
   m_kwVis->set(m_io->readVis());
   m_kwVolt->set(m_io->readVolt());
}

void CHighPrecisionLiBatteryMonitor::write(const std::string &keyword, const std::string &command)
{
   if (m_kwIo->getKeyword() != keyword)
   {
      YADOMS_LOG(error) << "Unknown keyword " << keyword;
      return;
   }

   m_kwIo->setCommand(command);
   m_io->writeIo(m_kwIo->get());
}

bool CHighPrecisionLiBatteryMonitor::hasRelevantValue()
{
   if (m_keywords.empty())
      return false;

   const auto now = shared::currentTime::Provider().now();

   if (now < (m_lastHistorizationDate + HistorizationPeriod))
      return false;

   m_lastHistorizationDate = now;
   return true;
}

} // namespace device
