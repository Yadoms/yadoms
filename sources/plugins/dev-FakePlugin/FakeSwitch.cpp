#include "stdafx.h"
#include "FakeSwitch.h"
#include <shared/tools/Random.h>

CFakeSwitch::CFakeSwitch(const std::string& deviceName,
                         bool isDimmable,
                         bool isSettable)
   : m_deviceName(deviceName),
     m_isDimmable(isDimmable)
{
   if (m_isDimmable)
      m_dimmableSwitch = boost::make_shared<yApi::historization::CDimmable>("DimmableSwitch",
                                                                            isSettable ? yApi::EKeywordAccessMode::kGetSet : yApi::EKeywordAccessMode::kGet);
   else
      m_switch = boost::make_shared<yApi::historization::CSwitch>("Switch",
                                                                  isSettable ? yApi::EKeywordAccessMode::kGetSet : yApi::EKeywordAccessMode::kGet);
}

CFakeSwitch::~CFakeSwitch()
{
}

void CFakeSwitch::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getType(), getModel());

   // Declare associated keywords (= values managed by this device)
   if (m_isDimmable)
   {
      if (!api->keywordExists(m_deviceName, m_dimmableSwitch))
         api->declareKeyword(m_deviceName, m_dimmableSwitch);
   }
   else
   {
      if (!api->keywordExists(m_deviceName, m_switch))
         api->declareKeyword(m_deviceName, m_switch);
   }
}

void CFakeSwitch::read() const
{
   if (m_isDimmable)
   {
      // Generate a random value (0 to 100)
      m_dimmableSwitch->set(shared::tools::CRandom::generate(0, 100));
   }
   else
   {
      // Toggle switch
      m_switch->set(m_switch->get() ? false : true);
   }
}

void CFakeSwitch::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   if (m_isDimmable)
      api->historizeData(m_deviceName, m_dimmableSwitch);
   else
      api->historizeData(m_deviceName, m_switch);
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

const std::string& CFakeSwitch::getType()
{
   static const std::string type("fakeSwitchType");
   return type;
}

