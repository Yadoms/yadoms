#include "stdafx.h"
#include "4kRamWithCounter.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {
   
C4kRamWithCounter::C4kRamWithCounter(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IMultiCounter> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2423")), m_context(context), m_io(io),
   m_countA(boost::make_shared<yApi::historization::CCounter>("counter_A", yApi::EKeywordAccessMode::kGet, yApi::historization::EMeasureType::kCumulative)),
   m_countB(boost::make_shared<yApi::historization::CCounter>("counter_B", yApi::EKeywordAccessMode::kGet, yApi::historization::EMeasureType::kCumulative))
{
   BOOST_ASSERT_MSG(m_identification->family() == k4kRamWithCounter, "Invalid family number");

   m_keywords.push_back(m_countA);
   m_keywords.push_back(m_countB);
}

C4kRamWithCounter::~C4kRamWithCounter()
{
}

boost::shared_ptr<const IIdentification> C4kRamWithCounter::ident() const
{
   return m_identification;
}

void C4kRamWithCounter::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), *m_countA))
      m_context->declareKeyword(m_identification->deviceName(), *m_countA);
   if (!m_context->keywordExists(m_identification->deviceName(), *m_countB))
      m_context->declareKeyword(m_identification->deviceName(), *m_countB);
}

void C4kRamWithCounter::historize()
{
   m_countA->set(m_io->read(0));
   m_countB->set(m_io->read(1));

   m_context->historizeData(ident()->deviceName(), m_keywords);
}

void C4kRamWithCounter::set(const std::string& keyword, const std::string& command)
{
   YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
}

} // namespace device