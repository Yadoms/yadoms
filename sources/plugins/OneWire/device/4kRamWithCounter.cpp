#include "stdafx.h"
#include "4kRamWithCounter.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration C4kRamWithCounter::HistorizationPeriod(boost::posix_time::minutes(5));

C4kRamWithCounter::C4kRamWithCounter(EOneWireFamily family,
                                     const std::string &id,
                                     boost::shared_ptr<ioInterfaces::IMultiCounter> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2423")),
      m_io(io),
      m_countA(boost::make_shared<yApi::historization::CCounter>("counter_A", yApi::EKeywordAccessMode::kGet, yApi::EMeasureType::kCumulative)),
      m_countB(boost::make_shared<yApi::historization::CCounter>("counter_B", yApi::EKeywordAccessMode::kGet, yApi::EMeasureType::kCumulative)),
      m_keywords({m_countA, m_countB})
{
   BOOST_ASSERT_MSG(m_identification->family() == k4kRamWithCounter, "Invalid family number");
}

void C4kRamWithCounter::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const boost::shared_ptr<shared::CDataContainer> &configuration)
{
   YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
}

void C4kRamWithCounter::read() const
{
   m_countA->set(m_io->read(0));
   m_countB->set(m_io->read(1));
}

void C4kRamWithCounter::write(const std::string &keyword, const std::string &command)
{
   YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
}

bool C4kRamWithCounter::hasRelevantValue()
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
