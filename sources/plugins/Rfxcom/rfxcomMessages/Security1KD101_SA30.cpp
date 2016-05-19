#include "stdafx.h"
#include "Security1KD101_SA30.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity1KD101_SA30::CSecurity1KD101_SA30(const std::string& model)
   :m_model(model),
   m_alarm(boost::make_shared<yApi::historization::CSwitch>("alarm", yApi::EKeywordAccessMode::kGet)),
   m_keywords({ m_alarm })
{
}

CSecurity1KD101_SA30::~CSecurity1KD101_SA30()
{
}
   
std::string CSecurity1KD101_SA30::getModel() const
{
   return m_model;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CSecurity1KD101_SA30::keywords() const
{
   return m_keywords;
}

void CSecurity1KD101_SA30::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1KD101_SA30::reset()
{
   m_alarm->set(false);
}

void CSecurity1KD101_SA30::setFromProtocolState(unsigned char statusByte)
{
   m_alarm->set(statusByte == sStatusPanic);
}

unsigned char CSecurity1KD101_SA30::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages