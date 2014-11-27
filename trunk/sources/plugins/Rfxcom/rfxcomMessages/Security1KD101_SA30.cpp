#include "stdafx.h"
#include "Security1KD101_SA30.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CSecurity1KD101_SA30::CSecurity1KD101_SA30(const std::string& model)
   :m_model(model), m_alarm("alarm", yApi::EKeywordAccessMode::kGet)
{
}

CSecurity1KD101_SA30::~CSecurity1KD101_SA30()
{
}
   
std::string CSecurity1KD101_SA30::getModel() const
{
   return m_model;
}

void CSecurity1KD101_SA30::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_alarm))
      context->declareKeyword(deviceName, m_alarm);
}

void CSecurity1KD101_SA30::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_alarm);
}

void CSecurity1KD101_SA30::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
{
    // Get-only keywords
}

void CSecurity1KD101_SA30::reset()
{
   m_alarm.set(false);
}

void CSecurity1KD101_SA30::setFromProtocolState(unsigned char statusByte)
{
   m_alarm.set(statusByte == sStatusPanic);
}

unsigned char CSecurity1KD101_SA30::toProtocolState() const
{
   // Get-only keywords
   return 0;
}

} // namespace rfxcomMessages