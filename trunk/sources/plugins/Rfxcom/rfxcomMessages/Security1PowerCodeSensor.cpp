#include "stdafx.h"
#include "Security1PowerCodeSensor.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CSecurity1PowerCodeSensor::CSecurity1PowerCodeSensor(bool secondaryContact)
   :m_alarm(secondaryContact ? "alarm1" : "alarm2", yApi::EKeywordAccessMode::kGet),
   m_tamper(secondaryContact ? "tamper1" : "tamper2" , yApi::EKeywordAccessMode::kGet)
{
}
   
   CSecurity1PowerCodeSensor::~CSecurity1PowerCodeSensor()
   {
   }
   
std::string CSecurity1PowerCodeSensor::getModel() const
{
   return "Visonic PowerCode door/window sensor";
}

void CSecurity1PowerCodeSensor::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_alarm))
      context->declareKeyword(deviceName, m_alarm);
   if (!context->keywordExists(deviceName, m_tamper))
      context->declareKeyword(deviceName, m_tamper);
}

void CSecurity1PowerCodeSensor::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_alarm);
   context->historizeData(deviceName, m_tamper);
}

void CSecurity1PowerCodeSensor::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1PowerCodeSensor::reset()
{
   m_alarm.set(false);
   m_tamper.set(false);
}

void CSecurity1PowerCodeSensor::setFromProtocolState(unsigned char statusByte)
{
   m_alarm.set(statusByte == sStatusAlarm || statusByte == sStatusAlarmTamper);
   m_tamper.set(statusByte == sStatusNormalTamper || statusByte == sStatusAlarmTamper);
}

unsigned char CSecurity1PowerCodeSensor::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages