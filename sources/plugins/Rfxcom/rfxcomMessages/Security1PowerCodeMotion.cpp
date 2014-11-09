#include "stdafx.h"
#include "Security1PowerCodeMotion.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

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

void CSecurity1PowerCodeMotion::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_alarm))
      context->declareKeyword(deviceName, m_alarm);
   if (!context->keywordExists(deviceName, m_tamper))
      context->declareKeyword(deviceName, m_tamper);
}

void CSecurity1PowerCodeMotion::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_alarm);
   context->historizeData(deviceName, m_tamper);
}

void CSecurity1PowerCodeMotion::set(const std::string& /*keyword*/, const shared::CDataContainer& /*yadomsCommand*/)
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