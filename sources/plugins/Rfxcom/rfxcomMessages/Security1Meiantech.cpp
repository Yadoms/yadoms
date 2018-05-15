#include "stdafx.h"
#include "Security1Meiantech.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CSecurity1Meiantech::CSecurity1Meiantech()
      : m_statusByte(0),
        m_motion(boost::make_shared<yApi::historization::CSwitch>("motion")),
        m_panic(boost::make_shared<yApi::historization::CSwitch>("panic")),
        m_tamper(boost::make_shared<yApi::historization::CSwitch>("tamper")),
        m_armAlarm(boost::make_shared<yApi::historization::CArmingAlarm>("armAlarm", yApi::EKeywordAccessMode::kGetSet)),
        m_keywords({m_motion, m_panic, m_tamper, m_armAlarm})
   {
   }


   CSecurity1Meiantech::~CSecurity1Meiantech()
   {
   }


   std::string CSecurity1Meiantech::getModel() const
   {
      return "Meiantech";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CSecurity1Meiantech::keywords() const
   {
      return m_keywords;
   }

   void CSecurity1Meiantech::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_panic->getKeyword()))
      {
         m_panic->setCommand(yadomsCommand);
         m_statusByte = m_panic.get() ? sStatusPanic : sStatusNormal;
      }
      else if (boost::iequals(keyword, m_armAlarm->getKeyword()))
      {
         m_armAlarm->setCommand(yadomsCommand);
         switch (m_armAlarm->get())
         {
         case yApi::historization::EArmingAlarmStatus::kDisarmedValue: m_statusByte = sStatusDisarm;
            break;
         case yApi::historization::EArmingAlarmStatus::kArmedAtHomeValue: m_statusByte = sStatusArmHome;
            break;
         case yApi::historization::EArmingAlarmStatus::kArmedAwayValue: m_statusByte = sStatusArmAway;
            break;
         default: throw shared::exception::CInvalidParameter("yadomsCommand");
         }
      }
   }

   void CSecurity1Meiantech::reset()
   {
      m_statusByte = 0;
      m_panic->set(false);
      m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kDisarmed);
   }

   void CSecurity1Meiantech::setFromProtocolState(unsigned char statusByte)
   {
      m_statusByte = statusByte;

      switch (m_statusByte)
      {
      case sStatusMotion:
         m_motion->set(true);
         m_tamper->set(false);
         break;

      case sStatusNoMotion:
         m_motion->set(false);
         m_tamper->set(false);
         break;

      case sStatusPanic:
         m_panic->set(true);
         m_tamper->set(false);
         break;

      case sStatusPanicOff:
         m_panic->set(false);
         m_tamper->set(false);
         break;

      case sStatusNormalTamper:
      case sStatusNormalDelayedTamper:
      case sStatusAlarmTamper:
         m_tamper->set(true);
         break;

      case sStatusMotionTamper:
         m_motion->set(true);
         m_tamper->set(true);
         break;

      case sStatusNoMotionTamper:
         m_motion->set(false);
         m_tamper->set(true);
         break;

      case sStatusArmAway:
      case sStatusArmAwayDelayed: m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kArmedAway);
         break;
      case sStatusArmHome:
      case sStatusArmHomeDelayed: m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kArmedAtHome);
         break;
      case sStatusDisarm: m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kDisarmed);
         break;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(static_cast<unsigned int>(m_statusByte)));
      }
   }

   unsigned char CSecurity1Meiantech::toProtocolState() const
   {
      return m_statusByte;
   }

   unsigned long CSecurity1Meiantech::idFromProtocol(const RBUF& rbuf) const
   {
      return static_cast<unsigned long>((rbuf.SECURITY1.id1 << 16) + (rbuf.SECURITY1.id2 << 8) + rbuf.SECURITY1.id3);
   }
} // namespace rfxcomMessages
