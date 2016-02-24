#include "stdafx.h"
#include "FakeSwitch.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/StringExtension.h>
#include <shared/Log.h>

CFakeSwitch::CFakeSwitch(const std::string& deviceName, bool isDimmable, bool isSettable)
   :m_deviceName(deviceName), m_isDimmable(isDimmable), m_dist(0, 100)
{
   if (m_isDimmable)
      m_dimmableSwitch.reset(new yApi::historization::CDimmable("DimmableSwitch", isSettable ? yApi::EKeywordAccessMode::kGetSet : yApi::EKeywordAccessMode::kGet));
   else
      m_switch.reset(new yApi::historization::CSwitch("Switch", isSettable ? yApi::EKeywordAccessMode::kGetSet : yApi::EKeywordAccessMode::kGet));
}

CFakeSwitch::~CFakeSwitch()
{
}

void CFakeSwitch::declareDevice(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (!context->deviceExists(m_deviceName))
      context->declareDevice(m_deviceName, getModel());

   // Declare associated keywords (= values managed by this device)
   if (m_isDimmable)
   {
      if (!context->keywordExists(m_deviceName, *m_dimmableSwitch))
         context->declareKeyword(m_deviceName, *m_dimmableSwitch);
   }
   else
   {
      if (!context->keywordExists(m_deviceName, *m_switch))
         context->declareKeyword(m_deviceName, *m_switch);
   }
}

void CFakeSwitch::read()
{
   if (m_isDimmable)
   {
      // Generate a random value (0 to 100)
      m_dimmableSwitch->set(m_dist(m_gen));
   }
   else
   {
      // Toggle switch
      m_switch->set(m_switch->get() ? false : true);
   }
}

void CFakeSwitch::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   if (!context)
      throw shared::exception::CException("context must be defined");

   if (m_isDimmable)
      context->historizeData(m_deviceName, *m_dimmableSwitch);
   else
      context->historizeData(m_deviceName, *m_switch);
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