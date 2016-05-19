#include "stdafx.h"
#include "Security1PowerCodeMotion.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity1PowerCodeMotion::CSecurity1PowerCodeMotion()
   : m_alarm(boost::make_shared<yApi::historization::CSwitch>("alarm", yApi::EKeywordAccessMode::kGet)),
   m_tamper(boost::make_shared<yApi::historization::CSwitch>("tamper", yApi::EKeywordAccessMode::kGet)),
   m_keywords({ m_alarm, m_tamper })
{
}

   CSecurity1PowerCodeMotion::~CSecurity1PowerCodeMotion()
   {
   }
   
std::string CSecurity1PowerCodeMotion::getModel() const
{
   return "Visonic PowerCode motion sensor";
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CSecurity1PowerCodeMotion::keywords() const
{
   return m_keywords;
}

void CSecurity1PowerCodeMotion::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1PowerCodeMotion::reset()
{
   m_alarm->set(false);
   m_tamper->set(false);
}

void CSecurity1PowerCodeMotion::setFromProtocolState(unsigned char statusByte)
{
   m_alarm->set(statusByte == sStatusAlarm || statusByte == sStatusAlarmTamper);
   m_tamper->set(statusByte == sStatusNormalTamper || statusByte == sStatusAlarmTamper);
}

unsigned char CSecurity1PowerCodeMotion::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages