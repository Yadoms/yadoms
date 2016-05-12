#include "stdafx.h"
#include "Security1PowerCodeSensor.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

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

void CSecurity1PowerCodeSensor::declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   if (!api->keywordExists(deviceName, m_alarm))
      api->declareKeyword(deviceName, m_alarm);
   if (!api->keywordExists(deviceName, m_tamper))
      api->declareKeyword(deviceName, m_tamper);
}

void CSecurity1PowerCodeSensor::historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   api->historizeData(deviceName, m_alarm);
   api->historizeData(deviceName, m_tamper);
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