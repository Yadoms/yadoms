#include "stdafx.h"
#include "Security1X10R.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CSecurity1X10R::CSecurity1X10R()
   :m_statusByte(0), m_panic("panic"), m_armAlarm("armAlarm"), m_light1("light1"), m_light2("light2")
{
}

std::string CSecurity1X10R::getModel() const
{
   return "X10 security remote";
}

void CSecurity1X10R::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_panic))
      context->declareKeyword(deviceName, m_panic);
   if (!context->keywordExists(deviceName, m_armAlarm))
      context->declareKeyword(deviceName, m_armAlarm);
   if (!context->keywordExists(deviceName, m_light1))
      context->declareKeyword(deviceName, m_light1);
   if (!context->keywordExists(deviceName, m_light2))
      context->declareKeyword(deviceName, m_light2);
}

void CSecurity1X10R::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_panic);
   context->historizeData(deviceName, m_armAlarm);
   context->historizeData(deviceName, m_light1);
   context->historizeData(deviceName, m_light2);
}

void CSecurity1X10R::set(const std::string& keyword, const shared::CDataContainer& yadomsCommand)
{
   if (boost::iequals(keyword, m_panic.getKeyword()))
   {
      m_panic.set(yadomsCommand); m_statusByte = m_panic.get() ? sStatusPanic : sStatusPanicOff;
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
   else if (boost::iequals(keyword, m_light1.getKeyword()))
   {
      m_light1.set(yadomsCommand); m_statusByte = m_light1.get() ? sStatusLightOn : sStatusLightOff;
   }
   else if (boost::iequals(keyword, m_light2.getKeyword()))
   {
      m_light2.set(yadomsCommand); m_statusByte = m_light2.get() ? sStatusLight2On : sStatusLight2Off;
   }
}

void CSecurity1X10R::default()
{
   m_statusByte = 0;
   m_panic.set(false);
   m_armAlarm.set(yApi::historization::EArmingAlarmStatus::kDisarmedValue);
   m_light1.set(false);
   m_light2.set(false);
}

void CSecurity1X10R::setFromProtocolState(unsigned char statusByte)
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

   case sStatusLightOff:            m_light1.set(false);                                                          break;
   case sStatusLightOn:             m_light1.set(true);                                                           break;

   case sStatusLight2Off:           m_light2.set(false);                                                          break;
   case sStatusLight2On:            m_light2.set(true);                                                           break;

   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

unsigned char CSecurity1X10R::toProtocolState() const
{
   return m_statusByte;
}

} // namespace rfxcomMessages