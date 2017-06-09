#include "stdafx.h"
#include "FakeCurtain.h"

CFakeCurtain::CFakeCurtain(const std::string& deviceName)
   : m_deviceName(deviceName)
{
   m_curtain = boost::make_shared<yApi::historization::CCurtain>("Curtain");
}

CFakeCurtain::~CFakeCurtain()
{
}

void CFakeCurtain::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getType(), getModel());

   // Declare associated keywords (= values managed by this device)
   if (!api->keywordExists(m_deviceName, m_curtain))
      api->declareKeyword(m_deviceName, m_curtain);
}

void CFakeCurtain::read()
{
   switch (m_curtain->get())
   {
   case yApi::historization::ECurtainCommand::kStopValue:
      m_curtain->set(yApi::historization::ECurtainCommand::kOpen);
      break;
   case yApi::historization::ECurtainCommand::kOpenValue:
      m_curtain->set(yApi::historization::ECurtainCommand::kClose);
      break;
   case yApi::historization::ECurtainCommand::kCloseValue:
      m_curtain->set(yApi::historization::ECurtainCommand::kStop);
      break;
   }
}

void CFakeCurtain::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");
   api->historizeData(m_deviceName, m_curtain);
}

const std::string& CFakeCurtain::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeCurtain::getModel()
{
   static const std::string model("Fake curtain");
   return model;
}

const std::string& CFakeCurtain::getType()
{
   static const std::string type("fakeCurtainType");
   return type;
}

