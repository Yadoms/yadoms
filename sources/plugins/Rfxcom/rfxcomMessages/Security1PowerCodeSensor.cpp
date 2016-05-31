#include "stdafx.h"
#include "Security1PowerCodeSensor.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity1PowerCodeSensor::CSecurity1PowerCodeSensor(bool secondaryContact)
   : m_alarm(boost::make_shared<yApi::historization::CSwitch>(secondaryContact ? "alarm1" : "alarm2", yApi::EKeywordAccessMode::kGet)),
   m_tamper(boost::make_shared<yApi::historization::CSwitch>(secondaryContact ? "tamper1" : "tamper2", yApi::EKeywordAccessMode::kGet)),
   m_keywords({ m_alarm, m_tamper })
{
}
   
CSecurity1PowerCodeSensor::~CSecurity1PowerCodeSensor()
{
}
   
std::string CSecurity1PowerCodeSensor::getModel() const
{
   return "Visonic PowerCode door/window sensor";
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CSecurity1PowerCodeSensor::keywords() const
{
   return m_keywords;
}

void CSecurity1PowerCodeSensor::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1PowerCodeSensor::reset()
{
   m_alarm->set(false);
   m_tamper->set(false);
}

void CSecurity1PowerCodeSensor::setFromProtocolState(unsigned char statusByte)
{
   m_alarm->set(statusByte == sStatusAlarm || statusByte == sStatusAlarmTamper);
   m_tamper->set(statusByte == sStatusNormalTamper || statusByte == sStatusAlarmTamper);
}

unsigned char CSecurity1PowerCodeSensor::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

unsigned long CSecurity1PowerCodeSensor::idFromProtocol( const RBUF& rbuf ) const
{
	return (unsigned long)(rbuf.SECURITY1.id1 << 8) + (rbuf.SECURITY1.id2);
}

} // namespace rfxcomMessages