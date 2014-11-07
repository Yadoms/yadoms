#include "stdafx.h"
#include "Security1Meiantech.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CSecurity1Meiantech::CSecurity1Meiantech()
   :m_statusByte(0), m_panic("panic"), m_armAlarm("armAlarm")
{
}

std::string CSecurity1Meiantech::getModel() const
{
   return "Meiantech";
}

void CSecurity1Meiantech::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_panic))
      context->declareKeyword(deviceName, m_panic);
   if (!context->keywordExists(deviceName, m_armAlarm))
      context->declareKeyword(deviceName, m_armAlarm);
}

void CSecurity1Meiantech::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_panic);
   context->historizeData(deviceName, m_armAlarm);
}

void CSecurity1Meiantech::set(const std::string& keyword, const shared::CDataContainer& yadomsCommand)
{
   if (boost::iequals(keyword, m_panic.getKeyword()))
   {
      m_panic.set(yadomsCommand); m_statusByte = m_panic.get() ? sStatusPanic : sStatusNormal;
   }
   else if (boost::iequals(keyword, m_armAlarm.getKeyword()))
   {
      m_armAlarm.set(yadomsCommand);
      switch(m_armAlarm.get())
      {
      case yApi::historization::EArmingAlarmStatus::kDisarmedValue : m_statusByte = sStatusDisarm; break;
      case yApi::historization::EArmingAlarmStatus::kArmedAtHomeValue : m_statusByte = sStatusArmHome; break;
      case yApi::historization::EArmingAlarmStatus::kArmedAwayValue : m_statusByte = sStatusArmAway; break;
      default: throw shared::exception::CInvalidParameter("yadomsCommand");
      }
   }
}

void CSecurity1Meiantech::default()
{
   m_statusByte = 0;
   m_panic.set(false);
   m_armAlarm.set(yApi::historization::EArmingAlarmStatus::kDisarmedValue);
}

void CSecurity1Meiantech::setFromProtocolState(unsigned char statusByte)
{
   m_statusByte = statusByte;
   switch(m_statusByte)
   {
   case sStatusPanicOff:            m_panic.set(false);                                                           break;
   case sStatusPanic   :            m_panic.set(true);                                                            break;

   case sStatusArmAway :
   case sStatusArmAwayDelayed :     m_armAlarm.set(yApi::historization::EArmingAlarmStatus::kArmedAwayValue);     break;
   case sStatusArmHome :
   case sStatusArmHomeDelayed :     m_armAlarm.set(yApi::historization::EArmingAlarmStatus::kArmedAtHomeValue);   break;
   case sStatusDisarm :             m_armAlarm.set(yApi::historization::EArmingAlarmStatus::kDisarmedValue);      break;

   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

unsigned char CSecurity1Meiantech::toProtocolState() const
{
   return m_statusByte;
}

} // namespace rfxcomMessages