#include "stdafx.h"
#include "Security1X10R.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CSecurity1X10R::CSecurity1X10R()
      : m_statusByte(0),
      m_panic(boost::make_shared<yApi::historization::CSwitch>("panic")),
      m_armAlarm(boost::make_shared<yApi::historization::CArmingAlarm>("armAlarm", yApi::EKeywordAccessMode::kGetSet)),
      m_light1(boost::make_shared<yApi::historization::CSwitch>("light1")),
      m_light2(boost::make_shared<yApi::historization::CSwitch>("light2")),
      m_keywords({ m_panic, m_armAlarm, m_light1, m_light2 })
   {
   }


   CSecurity1X10R::~CSecurity1X10R()
   {
   }

   std::string CSecurity1X10R::getModel() const
   {
      return "X10 security remote";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CSecurity1X10R::keywords() const
   {
      return m_keywords;
   }

   void CSecurity1X10R::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_panic->getKeyword()))
      {
         m_panic->setCommand(yadomsCommand);
         m_statusByte = m_panic.get() ? sStatusPanic : sStatusPanicOff;
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
      else if (boost::iequals(keyword, m_light1->getKeyword()))
      {
         m_light1->setCommand(yadomsCommand);
         m_statusByte = m_light1.get() ? sStatusLightOn : sStatusLightOff;
      }
      else if (boost::iequals(keyword, m_light2->getKeyword()))
      {
         m_light2->setCommand(yadomsCommand);
         m_statusByte = m_light2.get() ? sStatusLight2On : sStatusLight2Off;
      }
   }

   void CSecurity1X10R::reset()
   {
      m_statusByte = 0;
      m_panic->set(false);
      m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kDisarmed);
      m_light1->set(false);
      m_light2->set(false);
   }

   void CSecurity1X10R::setFromProtocolState(unsigned char statusByte)
   {
      m_statusByte = statusByte;
      switch (m_statusByte)
      {
      case sStatusPanicOff: m_panic->set(false);
         break;
      case sStatusPanic: m_panic->set(true);
         break;

      case sStatusArmAway:
      case sStatusArmAwayDelayed: m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kArmedAway);
         break;
      case sStatusArmHome:
      case sStatusArmHomeDelayed: m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kArmedAtHome);
         break;
      case sStatusDisarm: m_armAlarm->set(yApi::historization::EArmingAlarmStatus::kDisarmed);
         break;

      case sStatusLightOff: m_light1->set(false);
         break;
      case sStatusLightOn: m_light1->set(true);
         break;

      case sStatusLight2Off: m_light2->set(false);
         break;
      case sStatusLight2On: m_light2->set(true);
         break;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(m_statusByte));
      }
   }

   unsigned char CSecurity1X10R::toProtocolState() const
   {
      return m_statusByte;
   }

   unsigned long CSecurity1X10R::idFromProtocol(const RBUF& rbuf) const
   {
      return static_cast<unsigned long>(rbuf.SECURITY1.id1);
   }
} // namespace rfxcomMessages


