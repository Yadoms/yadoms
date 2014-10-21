#include "stdafx.h"
#include "FakeCounter.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/StringExtension.h>
#include <shared/Log.h>

CFakeCounter::CFakeCounter(const std::string& deviceName)
   :m_deviceName(deviceName), m_incrementCount("incrementCount", yApi::historization::EMeasureType::kIncrement),
   m_totalCount("totalCounter"), m_dist(0, 15)
{
   m_totalCount.set(0);
}

CFakeCounter::~CFakeCounter()
{
}

void CFakeCounter::declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceName, m_incrementCount);
   context->declareKeyword(m_deviceName, m_totalCount);
}

void CFakeCounter::read()
{
   // Generate a random increment (0 to 15)
   m_incrementCount.set(m_dist(m_gen));

   // Generate a random increment (0 to 15)
   m_totalCount.set(m_totalCount.get() + m_dist(m_gen));
}

void CFakeCounter::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_deviceName, m_incrementCount);
   context->historizeData(m_deviceName, m_totalCount);
}

const std::string& CFakeCounter::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeCounter::getModel()
{
   static const std::string model("Fake counter");
   return model;
}