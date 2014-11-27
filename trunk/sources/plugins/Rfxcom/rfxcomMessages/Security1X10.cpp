#include "stdafx.h"
#include "Security1X10.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CSecurity1X10::CSecurity1X10()
   :m_alarm("alarm", yApi::EKeywordAccessMode::kGet), m_tamper("tamper", yApi::EKeywordAccessMode::kGet)
{
}
   CSecurity1X10::~CSecurity1X10()
   {
   }
std::string CSecurity1X10::getModel() const
{
   return "X10 security door/window sensor";
}

void CSecurity1X10::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_alarm))
      context->declareKeyword(deviceName, m_alarm);
   if (!context->keywordExists(deviceName, m_tamper))
      context->declareKeyword(deviceName, m_tamper);
}

void CSecurity1X10::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_alarm);
   context->historizeData(deviceName, m_tamper);
}

void CSecurity1X10::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1X10::reset()
{
   m_alarm.set(false);
   m_tamper.set(false);
}

void CSecurity1X10::setFromProtocolState(unsigned char statusByte)
{
   m_alarm.set(statusByte == sStatusAlarm || statusByte == sStatusAlarmDelayed || statusByte == sStatusAlarmTamper || statusByte == sStatusAlarmDelayedTamper);
   m_tamper.set(statusByte == sStatusNormalTamper || statusByte == sStatusNormalDelayedTamper || statusByte == sStatusAlarmTamper || statusByte == sStatusAlarmDelayedTamper);
}

unsigned char CSecurity1X10::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages