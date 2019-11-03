#include "stdafx.h"
#include "4kRamWithCounter.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
   C4kRamWithCounter::C4kRamWithCounter(EOneWireFamily family,
                                        const std::string& id,
                                        boost::shared_ptr<ioInterfaces::IMultiCounter> io)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2423")),
      m_io(io),
      m_countA(boost::make_shared<yApi::historization::CCounter>("counter_A", yApi::EKeywordAccessMode::kGet, yApi::EMeasureType::kCumulative)),
      m_countB(boost::make_shared<yApi::historization::CCounter>("counter_B", yApi::EKeywordAccessMode::kGet, yApi::EMeasureType::kCumulative)),
      m_keywords({ m_countA, m_countB })
   {
      BOOST_ASSERT_MSG(m_identification->family() == k4kRamWithCounter, "Invalid family number");
   }

   C4kRamWithCounter::~C4kRamWithCounter()
   {
   }

   void C4kRamWithCounter::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const shared::CDataContainer& configuration)
   {
      YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
   }

   void C4kRamWithCounter::read() const
   {
      m_countA->set(m_io->read(0));
      m_countB->set(m_io->read(1));
   }

   void C4kRamWithCounter::write(const std::string& keyword, const std::string& command)
   {
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
   }
} // namespace device
