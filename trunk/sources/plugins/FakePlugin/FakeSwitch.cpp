#include "stdafx.h"
#include "FakeSwitch.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/StringExtension.h>
#include <shared/Log.h>

CFakeSwitch::CFakeSwitch(const std::string& deviceName, bool isDimmable)
   :m_deviceName(deviceName), m_isDimmable(isDimmable), m_switch("Switch"), m_dist(0, 100)
{
}

CFakeSwitch::~CFakeSwitch()
{
}

void CFakeSwitch::declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceName, m_switch);
}

void CFakeSwitch::read()
{
   if (m_isDimmable)
   {
      // Generate a random value (0 to 100)
      m_switch.set(m_dist(m_gen));
   }
   else
   {
      // Toggle switch
      m_switch.set(m_switch.isOn() ? false : true);
   }
}

void CFakeSwitch::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_deviceName, m_switch);
}

const std::string& CFakeSwitch::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeSwitch::getModel()
{
   static const std::string model("Fake switch");
   return model;
}