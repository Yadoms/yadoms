#include "stdafx.h"
#include "Security1X10M.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity1X10M::CSecurity1X10M()
   :m_alarm("alarm", yApi::EKeywordAccessMode::kGet), m_tamper("tamper", yApi::EKeywordAccessMode::kGet)
{
}
   
   CSecurity1X10M::~CSecurity1X10M()
   {
   }
   
std::string CSecurity1X10M::getModel() const
{
   return "X10 security motion sensor";
}

void CSecurity1X10M::declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   if (!api->keywordExists(deviceName, m_alarm))
      api->declareKeyword(deviceName, m_alarm);
   if (!api->keywordExists(deviceName, m_tamper))
      api->declareKeyword(deviceName, m_tamper);
}

void CSecurity1X10M::historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   api->historizeData(deviceName, m_alarm);
   api->historizeData(deviceName, m_tamper);
}

void CSecurity1X10M::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
   // Get-only keywords
}

void CSecurity1X10M::reset()
{
   m_alarm.set(false);
   m_tamper.set(false);
}

void CSecurity1X10M::setFromProtocolState(unsigned char statusByte)
{
   m_alarm.set(statusByte == sStatusMotion || statusByte == sStatusMotionTamper);
   m_tamper.set(statusByte == sStatusNoMotionTamper || statusByte == sStatusMotionTamper);
}

unsigned char CSecurity1X10M::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages