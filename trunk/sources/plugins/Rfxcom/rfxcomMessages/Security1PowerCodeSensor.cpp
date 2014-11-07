#include "stdafx.h"
#include "Security1PowerCodeSensor.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CSecurity1X10PowerCodeSensor::CSecurity1X10PowerCodeSensor(bool secondaryContact)
   :m_alarm(secondaryContact ? "alarm1" : "alarm2", yApi::EKeywordAccessMode::kGet),
   m_tamper(secondaryContact ? "tamper1" : "tamper2" , yApi::EKeywordAccessMode::kGet)
{
}

std::string CSecurity1X10PowerCodeSensor::getModel() const
{
   return "Visonic PowerCode door/window sensor";
}

void CSecurity1X10PowerCodeSensor::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_alarm))
      context->declareKeyword(deviceName, m_alarm);
   if (!context->keywordExists(deviceName, m_tamper))
      context->declareKeyword(deviceName, m_tamper);
}

void CSecurity1X10PowerCodeSensor::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_alarm);
   context->historizeData(deviceName, m_tamper);
}

void CSecurity1X10PowerCodeSensor::set(const std::string& /*keyword*/, const shared::CDataContainer& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1X10PowerCodeSensor::default()
{
   m_alarm.set(false);
   m_tamper.set(false);
}

void CSecurity1X10PowerCodeSensor::setFromProtocolState(unsigned char statusByte)
{
   m_alarm.set(statusByte == sStatusAlarm || statusByte == sStatusAlarmTamper);
   m_tamper.set(statusByte == sStatusNormalTamper || statusByte == sStatusAlarmTamper);
}

unsigned char CSecurity1X10PowerCodeSensor::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages