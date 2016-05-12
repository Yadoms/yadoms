#include "stdafx.h"
#include "Security1PowerCodeMotion.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity1PowerCodeMotion::CSecurity1PowerCodeMotion()
   :m_alarm("alarm", yApi::EKeywordAccessMode::kGet),
   m_tamper("tamper1" , yApi::EKeywordAccessMode::kGet)
{
}

   CSecurity1PowerCodeMotion::~CSecurity1PowerCodeMotion()
   {
   }
   
std::string CSecurity1PowerCodeMotion::getModel() const
{
   return "Visonic PowerCode motion sensor";
}

void CSecurity1PowerCodeMotion::declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   if (!api->keywordExists(deviceName, m_alarm))
      api->declareKeyword(deviceName, m_alarm);
   if (!api->keywordExists(deviceName, m_tamper))
      api->declareKeyword(deviceName, m_tamper);
}

void CSecurity1PowerCodeMotion::historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   api->historizeData(deviceName, m_alarm);
   api->historizeData(deviceName, m_tamper);
}

void CSecurity1PowerCodeMotion::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1PowerCodeMotion::reset()
{
   m_alarm.set(false);
   m_tamper.set(false);
}

void CSecurity1PowerCodeMotion::setFromProtocolState(unsigned char statusByte)
{
   m_alarm.set(statusByte == sStatusAlarm || statusByte == sStatusAlarmTamper);
   m_tamper.set(statusByte == sStatusNormalTamper || statusByte == sStatusAlarmTamper);
}

unsigned char CSecurity1PowerCodeMotion::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages